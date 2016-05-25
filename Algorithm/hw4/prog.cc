#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#define VERTEX_CAP 1000

// VT100 text color definiation, not working under Windows CMD
#define fgreset     "\x1b[m"
#define fgcolor(cl) ("\x1b[38;5;" cl "m")
#define fgblack     fgcolor("0")
#define fgred       fgcolor("1")
#define fggreen     fgcolor("2")
#define fgyellow    fgcolor("3")
#define fgblue      fgcolor("4")
#define fgmagenta   fgcolor("5")
#define fgcyan      fgcolor("6")
#define fgwhite     fgcolor("7")

using namespace std;

#ifdef DEBUG
// Logging purpose
stringstream slog;
#endif

class Node
{
    public:
        int id;
        int visited;
        bool walked;
        vector<Node*> connections;

        Node(int id) : id(id), visited(0), walked(false) { }

        // make a connection to some node
        void connect_to(Node *node)
        {
            this->connections.push_back(node);
        }

        // clear all connections
        void clear_connections()
        {
            this->connections.clear();
        }

        // clear visited counter and walked flag
        void clear()
        {
            this->visited = 0;
            this->walked = false;
        }

        // return if a node has been visited twice with simple path
        bool dfs()
        {
            // only simple path is taken, we leave if this node has been visited
            if(this->walked) {
                return false;
            }

            // Oops, we visited this node before
            if(this->visited >= 1) {
                return true;
            }

            this->walked = true; // set the visited flag for current node
            this->visited++;

            // try every possible path
            for(auto it = this->connections.begin(); it != this->connections.end(); it++) {
#ifdef DEBUG
                slog << this->id << " to " << (*it)->id << endl;
#endif
                // skip the path to self
                if(*it == this)
                    continue;
                // run DFS algorithm recursively
                if((*it)->dfs()) {
                    this->walked = false;
                    return true;
                }
            }

            // okey, we are out. nothing is found
            this->walked = false;
            return false;
        }
};

Node *vertices[VERTEX_CAP];
// C vertices are being used
int C;


// run dfs on every vertices, if exists a node visited twice,
// then this is not a singly connect graph.
bool solve()
{
    for(int i = 0; i < C; i++) {
#ifdef DEBUG
        slog = stringstream();
        slog << "from node " << i << endl;
#endif

        // clear visited counter before running DFS
        for(int j = 0; j < C; j++) {
            vertices[j]->clear();
        }

        // perform DFS from every vertex
        if(vertices[i]->dfs()) {
#ifdef DEBUG
            cout << slog.str();
#endif
            return false; // this is NOT a singly connected graph
        }
    }

    return true; // this is a signly connected graph
}

int main()
{
    // N rounds test data, Es edges, loop veriable
    int N, Es, i;
    int from, to;

    // DEBUG
    int round = 0;

    // initialize all vertices
    for(i = 0; i < VERTEX_CAP; i++) {
        vertices[i] = new Node(i);
    }

    cin >> N;

    // N rounds test data
    while(N-- > 0) {
        // read input
        cin >> C >> Es;
        // clear all vertices which would be used
        for(i = 0; i < C; i++) {
            vertices[i]->clear_connections();
        }

#ifdef DEBUG
        cout << fgyellow << "round #" << ++round << fgreset << endl;
#endif
        // read every edge in this test case
        while(Es--) {
            cin >> from >> to;
            vertices[from]->connect_to(vertices[to]);
        }

        // solve the singly connected graph problem
        cout << (solve() ? "YES" : "NO") << endl;
    }

    // free all vertices
    for(i = 0; i < VERTEX_CAP; i++) {
        delete vertices[i];
    }

    return 0;
}
