#include <pthread.h>
#include <curses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

#define TERM_WIDTH  80
#define TERM_HEIGHT 25
#define MAP_WIDTH   (TERM_WIDTH  - 2)
#define MAP_HEIGHT  (TERM_HEIGHT - 5)
#define ROCKS 5

#define FPS 12

enum COLORS {
	CL_NONE,
	CL_SNAKE_BODY,
	CL_BORDER,
	CL_APPLE,
	CL_ROCK
};

struct node {
	struct node *next;
	int x;
	int y;
};

typedef struct node node;

// ncruse WINDOW object
WINDOW *window;

// your score
int score;

// dead or not?
int dead;

// message to show, and message display time (in frame)
char msg[1024] = "paused";

// map to show items
char map[MAP_WIDTH][MAP_HEIGHT];

// snake current length, max snake length
int snake_curr_len, snake_max_len;

// one-way linked list
node *snake_head = NULL;
// circularly linked list
node *apple_head = NULL;
// just a normal array
node rocks[ROCKS];

// exit flag
int game_stop = 0;

// operator (key)
char op = '\0';

// flag: !pause
int game_run = 0;

// add new head node to snake linked list
node * add_snake_node(int x, int y)
{
	node * node = malloc(sizeof(node));

	node->next = snake_head;
	snake_head = node;
	node->x = x;
	node->y = y;

	map[x][y] = 'S';

	snake_curr_len++;

	return node;
}

// remove snake linked list tail node
void remove_snake_tail()
{
	if(!snake_head) return;
	node *p = snake_head, *l;
	while(p->next) {
		l = p;
		p = p->next;
	}
	l->next = NULL;
	assert(map[p->x][p->y] == 'S');
	map[p->x][p->y] = '\0';
	free(p);
	snake_curr_len--;
}

// add an apple on specific position
void add_apple(int x, int y)
{
	node * node = malloc(sizeof(node));

	node->x = x;
	node->y = y;

	map[x][y] = 'A';

	if(!apple_head) {
		apple_head = node->next = node;
	} else {
		node->next = apple_head->next;
		apple_head->next = node;
	}
}

// remove an apple on specific position
void remove_apple(int x, int y)
{
	if(apple_head == apple_head->next) {
		if(apple_head->x == x && apple_head->y == y) {
			assert(map[apple_head->x][apple_head->y] == 'A');
			map[apple_head->x][apple_head->y] = '\0';
			free(apple_head);
			apple_head = NULL;
		}
	} else {
		node *l = apple_head, *p = apple_head->next;
		while(p != apple_head) {
			if(p->x == x && p->y == y) {
				l->next = p->next;
				assert(map[p->x][p->y] == 'A');
				map[p->x][p->y] = '\0';
				free(p);
			}
			l = p;
			p = p->next;
		}
	}
}

// XXX: Use free_list instead
// void remove_all_apple()
// {
// 	if(apple_head == NULL) return;
// 
// 	if(apple_head != apple_head->next) {
// 		node *p = apple_head->next, *n;
// 		while(p && p != apple_head) {
// 			n = p->next;
// 			free(p);
// 			p = n;
// 		}
// 		free(p);
// 	} else {
// 		free(apple_head);
// 	}
// 
// 	apple_head = NULL;
// }

// calculate next movement position, invalid direction returns 0
int calc_move_position(char direction, int srcx, int srcy, int *x, int *y)
{
	int dx = 0, dy = 0;

	switch(direction) {
		case 'U':
			dy = -1;
			break;
		case 'D':
			dy  = 1;
			break;
		case 'L':
			dx = -1;
			break;
		case 'R':
			dx = 1;
			break;
		default:
			return 0;
	}

	*x = (srcx + dx + MAP_WIDTH) % MAP_WIDTH;
	*y = (srcy + dy + MAP_HEIGHT) % MAP_HEIGHT;
	return 1;
}

// calculate new position and add snake node
// valid direction: UDLR
void move_snake(char direction)
{
	int x, y;
	if(!calc_move_position(direction, snake_head->x, snake_head->y, &x, &y)) {
		return;
	}

	add_snake_node(x, y);
	if(snake_curr_len > snake_max_len) {
		remove_snake_tail();
	}
}

// delete entrie snake linked list
void free_list(node **p)
{
	if(!p || !*p) return;
	free_list(&(*p)->next);
	free(*p);
	*p = NULL;
}

// randomly choose a position and put an apple
void put_random_apple()
{
	int x, y;

	while(1) {
		x = rand() % MAP_WIDTH;
		y = rand() % MAP_HEIGHT;
		if(map[x][y] == '\0') {
			add_apple(x, y);
			break;
		}
	}
}

// remove all rocks from map
void remove_rocks()
{
	for(int i = 0; i < ROCKS; i++) {
		map[rocks[i].x][rocks[i].y] = '\0';
		rocks[i].x = -1;
		rocks[i].y = -1;
	}
}

// put rocks to random position
void put_random_rocks()
{
	for(int i = 0; i < ROCKS; i++) {
		int x, y;

		while(1) {
			x = rand() % MAP_WIDTH;
			y = rand() % MAP_HEIGHT;
			if(map[x][y] == '\0') {
				rocks[i].x = x;
				rocks[i].y = y;
				map[x][y] = 'R';
				break;
			}
		}
	}
}

// draw snake on screen
void draw_snake()
{
	for(node *p = snake_head; p; p = p->next) {
		attron(COLOR_PAIR(CL_SNAKE_BODY));
		mvaddch(2 + p->y, 1 + p->x, '#');
		attroff(COLOR_PAIR(CL_SNAKE_BODY));
	}
}

// draw apple on screen
void draw_apple()
{
	node *p = apple_head;
	if(p == NULL) return;
	do {
		attron(COLOR_PAIR(CL_APPLE));
		mvaddch(2 + p->y, 1 + p->x, '@');
		attroff(COLOR_PAIR(CL_APPLE));
	} while((p = p->next) != apple_head);
}

// draw rock
void draw_rock()
{
	for(int i = 0; i < ROCKS; i++) {
		node *p = &rocks[i];
		attron(COLOR_PAIR(CL_ROCK));
		mvaddch(2 + p->y, 1 + p->x, 'X');
		attroff(COLOR_PAIR(CL_ROCK));
	}
}

// initialize game arguments, create first snake node
void init_game()
{
	free_list(&snake_head);
	if(apple_head != NULL) {
		node *p = apple_head->next;
		apple_head->next = NULL;
		free_list(&p);
		apple_head = NULL;
	}

	snake_curr_len = 0;
	snake_max_len = 5;
	bzero(map, sizeof(map));
	add_snake_node(MAP_WIDTH / 2, MAP_HEIGHT / 2);
	put_random_apple();
	put_random_rocks();
	strcpy(msg, "paused");
	game_run = 0;
	score = 0;
	dead = 0;
}

// signal handler, stop game gracefully when Ctrl-C
void sighandler(int n)
{
	game_stop = 1;
}

// when SIGWINCH, reset terminal size
void onreisze(int n)
{
	resizeterm(TERM_HEIGHT, TERM_WIDTH);
}

// convert defined arrow keys to UDLR
char map_control_to_direction(char control)
{
	switch(control) {
		case 'h': case 'w': return 'U';
		case 'b': case 'a': return 'L';
		case 'n': case 's': return 'D';
		case 'm': case 'd': return 'R';
		default: return '\0';
	}
}

// thread to handle user input
void* thread_input(void *arg)
{
	while (!game_stop) {
		char t = getch();
		if('A' <= t && t <= 'Z') t ^= ' '; // to lowercase
		switch(t) {
			case 'h': case 'b': case 'n': case 'm':
			case 'w': case 'a': case 's': case 'd':
			case 'r':
				op = t;
				break;
			case ' ':
				game_run = !game_run;
				strcpy(msg, game_run ? "running" : "paused");
				break;
			case 'q':
				game_stop = 1;
		}
	}
	return NULL;
}

// thread to draw game UI
void* thread_draw(void *arg)
{
	uint32_t tick = 0;
	while (!game_stop) {
		erase();
		draw_snake();
		draw_apple();
		draw_rock();
		attron(COLOR_PAIR(CL_BORDER));
		box(window, '|', '-');
		attroff(COLOR_PAIR(CL_BORDER));

		for(int i = 0; i < MAP_WIDTH; i++) {
			mvaddch(22, i + 1, '-');
		}
		mvprintw(23, 1, "score: %3d, length: %3d, status: %s", score, snake_curr_len, msg);
		usleep(1000000 / FPS);
	}
	return NULL;
}

int main()
{
	pthread_t threads[2];

	srand(time(NULL) ^ ((((int)&threads) >> 12) * getpid()));

	signal(SIGINT, sighandler);
	signal(SIGWINCH, onreisze);

	window = initscr();
	cbreak();          // Ctrl-* keys will send signal to program
	noecho();          // Do not display key stroke on terminal
	keypad(stdscr, 1); // Enable function keys
	curs_set(0);       // Make cursor invisible
	nodelay(window, 1);

	// Make sure terminal size is bigger than (or equal to) TERM_WIDTH x TERM_HEIGHT
	int mcol = getmaxx(window);
	int mrow = getmaxy(window);
	if(mcol < TERM_WIDTH || mrow < TERM_HEIGHT) {
		endwin();
		fprintf(stderr, "Screen too small (%d, %d)\n", mcol, mrow);
		return 1;
	}

	// set terminal size
	resizeterm(TERM_HEIGHT, TERM_WIDTH);

	// enable color
	start_color();
	init_pair(CL_SNAKE_BODY, COLOR_BLUE, COLOR_BLACK);
	init_pair(CL_BORDER, COLOR_RED, COLOR_BLACK);
	init_pair(CL_APPLE, COLOR_YELLOW, COLOR_BLACK);
	init_pair(CL_ROCK, COLOR_BLACK, COLOR_RED);

	// init score, snake body, food, rocks
	init_game();

	// craete worker thread
	if(pthread_create(&threads[0], NULL, thread_input, NULL) != 0 ||
		pthread_create(&threads[1], NULL, thread_draw, NULL) != 0)
	{
		fprintf(stderr, "Can not create thread\n");
		return 1;
	}

	char direction = '\0';
	uint64_t counter = 0;
	while (!game_stop) {
		if(op == 'r') {
			init_game();
			op = 0;
			continue;
		}
		if(game_run) {
			counter++;
			if(op) {
				if(!dead) {
					char d = map_control_to_direction(op);
					if(d) direction = d;
				}
				op = 0;
			}
			int nx, ny;
			int apple_flag = 0;
			if(calc_move_position(direction, snake_head->x, snake_head->y, &nx, &ny)) {
				switch(map[nx][ny]) {
					case 'A':
						remove_apple(nx, ny);
						score++;
						snake_max_len++;
						apple_flag = 1;
					case '\0':
						if(!dead)
						move_snake(direction);
						break;
					case 'S':
					case 'R':
						game_run = 0;
						strcpy(msg, "dead");
						dead = 1;
						break;
				}
				if(apple_flag) {
					put_random_apple();
					remove_rocks();
					put_random_rocks();
				}
			}
		}
		usleep(100 * 1000);
	}

	// cleanup ncurse
	endwin();

	// wait for worker stop
	void *ret;
	pthread_join(threads[0], &ret);
	pthread_join(threads[1], &ret);

	puts("Bye~");
}
