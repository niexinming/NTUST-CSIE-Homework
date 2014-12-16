#include <stdio.h>

typedef enum GAME_WINNER_ { NO_WIN, O_WIN, X_WIN } GAME_WINNER;

void print_map_split()
{
    printf("+-+-+-+\n");
}

void print_map_row(int *row)
{
    int i;
    print_map_split();
    for (i = 0; i < 3; i++)
        printf((1 <= row[i] && row[i] <= 9) ? "|%d" : "|%c", row[i]);
    printf("|\n");
}

void print_map(int *map)
{
    int i;
    for (i = 0; i < 3; i++)
        print_map_row(map + i * 3);
    print_map_split();
}

GAME_WINNER check_winner_internal(int *map, int a, int b, int c)
{
    if (map[a] == map[b] && map[b] == map[c])
        return map[a] == 'O' ? O_WIN : X_WIN;
    return NO_WIN;
}

GAME_WINNER check_winner(int *map)
{
    int i;
    GAME_WINNER res = NO_WIN;
    for (i = 0; i < 3; i++) {
        res = check_winner_internal(map, i * 3, i * 3 + 1, i * 3 + 2);
        if (res != NO_WIN) return res;
        res = check_winner_internal(map, i, i + 3, i + 6);
        if (res != NO_WIN) return res;
    }
    res = check_winner_internal(map, 0, 4, 8);
    if (res != NO_WIN) return res;
    return check_winner_internal(map, 2, 4, 6);
}

int main()
{
    int i, pos, map[9];
    char turn = 'O';
    for (i = 0; i < 9; i++)
        map[i] = i + 1;
    print_map(map);
    printf("O goes first. which position do you want to place? ");
    while (scanf("%d", &pos) != EOF) {
        if (pos >= 1 && pos <= 9 && map[pos - 1] != 'O' && map[pos - 1] != 'X') {
            map[pos - 1] = turn;
            turn = (turn == 'O') ? 'X' : 'O';

            print_map(map);

            GAME_WINNER result = check_winner(map);
            if (result != NO_WIN) {
                printf("The winner is ... %c!\n", result == O_WIN ? 'O' : 'X');
                return 0;
            } else {
                printf("It's %c 's round now. which position do you want to place? ", turn);
            }
        } else {
            printf("%d is invalid position.\n", pos);
            printf("Choose a position for %c: ", turn);
        }
    }
    return 0;
}
