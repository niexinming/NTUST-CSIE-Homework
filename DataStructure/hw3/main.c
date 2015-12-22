#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TREE_INDENT_SIZE 4

typedef struct item_ {
    int priority;
    int count;
    struct item_ *left;
    struct item_ *right;
} item;

int DEBUG_FLAG = 1;
item *root = NULL;

int max(int a, int b) { return a > b ? a : b; }

int readint()
{
    int val;
    if(scanf("%d", &val) == EOF)
        exit(0);
    return val;
}

void hang(char *reason)
{
    fprintf(stderr, "[!] Error: %s\n", reason);
    fprintf(stderr, "[+] Ctrl-C to exit or attach debugger...");
    while(DEBUG_FLAG);
}

item *create_node(int priority)
{
    item *node = (item*)malloc(sizeof(item));
    node->priority = priority;
    node->count = 1;
    return node;
}

void destroy_node(item *node)
{
    free(node);
}

void dump(item *node, int depth, int max_depth)
{
    if(!node || (max_depth > 0 && depth > max_depth))
        return;
    printf("%*snode(%d, %d)\n", depth * TREE_INDENT_SIZE, "", node->priority,
            node->count);
    if(node->left || node->right) {
        if(node->left)
            dump(node->left, depth + 1, max_depth);
        else
            printf("%*sNULL\n", depth * TREE_INDENT_SIZE + TREE_INDENT_SIZE, "");
        if(node->right)
            dump(node->right, depth + 1, max_depth);
        else
            printf("%*sNULL\n", depth * TREE_INDENT_SIZE + TREE_INDENT_SIZE, "");
    }
}

int comput_size(item *node)
{
    if(!node) return 0;
    return 1 + comput_size(node->left) + comput_size(node->right);
}

int comput_depth(item *node)
{
    if(node == NULL) return 0;
    return 1 + max(comput_depth(node->left), comput_depth(node->right));
}

int expanse_buffer[1024*1024][2];

void level_order_expanse(item *node, int index)
{
    if(!node) return;
    expanse_buffer[index][0] = node->priority;
    expanse_buffer[index][1] = node->count;
    level_order_expanse(node->left, index * 2);
    level_order_expanse(node->right, index * 2 + 1);
}

void dump_in_level_order(item *node)
{
    memset(expanse_buffer, 0, sizeof(expanse_buffer));
    level_order_expanse(root, 1);

    int max_depth = comput_depth(root);
    int max_i = 1 << max_depth;
    int i;
    for(i = 1; i < max_i; i++) {
        if(expanse_buffer[i][1])
            printf("(%d, %d) ", expanse_buffer[i][0], expanse_buffer[i][1]);
        else
            printf("(NULL) ");
    }
    puts("");
}

item* find_parent(item *root, item *to_find)
{
    item *node;
    if(!root) return NULL;
    if(root->left) {
        if(root->left == to_find)
            return root;
        else
            node = find_parent(root->left, to_find);
        if(node) return node;
    }
    if(root->right) {
        if(root->right == to_find)
            return root;
        else
            node = find_parent(root->right, to_find);
        if(node) return node;
    }
    return NULL;
}

void replace_child(item *parent, item *child, item *replace_to)
{
    if(parent->left == child)
        parent->left = replace_to;
    if(parent->right == child)
        parent->right = replace_to;
}

void remove_child(item *parent, item *child)
{
    replace_child(parent, child, NULL);
}

item* find_node(item *current, int priority)
{
    if(!current || current->priority == priority)
        return current;
    return (current->priority > priority) ?
        find_node(current->left, priority) :
        find_node(current->right, priority);
}

void insert_node(item *where, item *to_place)
{
    if(where->priority > to_place->priority) {
        // put left

        if(where->left) {
            insert_node(where->left, to_place);
        } else {
            where->left = to_place;
        }
    } else if(where->priority < to_place->priority) {
        // put right

        if(where->right) {
            insert_node(where->right, to_place);
        } else {
            where->right = to_place;
        }
    } else {
        hang("invalid action, duplicated node value");
    }
}

void add_item(int priority)
{
    if(!root) {
        root = create_node(priority);
        return;
    }

    item *node = find_node(root, priority);
    if(node) {
        node->count++;
    } else {
        insert_node(root, create_node(priority));
    }
}

void pop_item()
{
    item *node = root;
    while(node && node->right) // find max
        node = node->right;
    if(!node) return;
    printf("Priority #%d just poped\n", node->priority);

    node->count--;

    if (node->count < 0)
        hang("negative count");
    else if (node->count > 0)
        return;

    // need to remove node

    if(node->right) // node must be max value
        hang("invalid state");

    if(node->left == NULL) { // if current node is leaf-node, just break link and free
        if(node == root) { // last node and also root node
            root = NULL;
        } else {
            remove_child(find_parent(root, node), node);
        }
        destroy_node(node);
        return;
    }

    if(node == root) { // node is root and has left-child
        root = root->left;
    } else { // node has left child and also have parent node
        replace_child(find_parent(root, node), node, node->left);
    }
    destroy_node(node);
}

void menu()
{
    while(1) {
        puts("\n 0) exit\n 1) insert\n 2) pop\n 3) show tree\nInput your action:");
        switch(readint()) {
            case 0:
                return;
            case 1:
                puts("Input priority:");
                add_item(readint());
                break;
            case 2:
                if(root)
                    pop_item();
                else
                    puts("Queue is empty");
                break;
            case 3:
                dump(root, 0, 0);
                puts("In level order");
                dump_in_level_order(root);
        }
    }
}

int main()
{
    puts("Input your initalize list.\nAn interger per row, -6666 for ending");

    int val;
    while((val = readint()) != -6666)
        add_item(val);
    menu();

    return 0;
}
