#include <iostream>
#include <vector>
#include <string>

#define VERTEX_CAP 1000

using namespace std;

class Node
{
    public:
        int id;
        bool visited;
        vector<Node*> connections;
        Node(int id) : id(id) { }

        void connect_to(Node *node) {
            this->connections.push_back(node);
        }

        void clear() {
            this->connections.clear();
            this->visited = false;
        }

        // return if a node has been visited twice
        // we must clear the visited flag before returning
        bool dfs()
        {
            if(this->visited) {
                this->visited = false;
                return true; // we found a node visited twice!
            }

            this->visited = true; // set the visited flag for current node

            for(auto it = this->connections.begin(); it != this->connections.end(); it++) {
                if(LOGGING) cout << this->id << " to " << (*it)->id << endl;
                if(*it == this)
                    continue;
                if((*it)->dfs()) {
                    this->visited = false;
                    return true;
                }
            }

            this->visited = false;
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
        if(LOGGING) cout << endl << "from node " << i << endl;
        if(vertices[i]->dfs()) {
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
            vertices[i]->clear();
        }

        if(LOGGING) cout << "round #" << ++round << endl;
        while(Es--) {
            cin >> from >> to;
            vertices[from]->connect_to(vertices[to]);
        }

        // solve the singly connected graph problem
        if(solve()) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }

    // free all vertices
    for(i = 0; i < VERTEX_CAP; i++) {
        delete vertices[i];
    }

    return 0;
}
