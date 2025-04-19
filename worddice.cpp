#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Graph {
   public:
    // constructor
    Graph(int vertices) {
        // adjacency list
        adj = vector<vector<int>>(vertices);
        // capacity matrix
        capacity = vector<vector<int>>(vertices, vector<int>(vertices, 0));
        // parent array
        parent = vector<int>(vertices, -1);
    }

    // adds edges to adjacency list and
    // capacity to capacity matrix
    void addEdge(int u, int v, int cap) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        capacity[u][v] = cap;
    }

    // breadth-first search
    bool bfs(int source, int sink) {
        fill(parent.begin(), parent.end(), -1);
        queue<int> q;
        // pushes source at the beginning to start bfs
        q.push(source);
        parent[source] = -2;

        // pops values off top until queue is empty
        while (!q.empty()) {
            int u = q.front();
            q.pop();

            // loops through adj row
            for (int v : adj[u]) {
                // updates if capacity is greater than zero
                if (parent[v] == -1 && capacity[u][v] > 0) {
                    parent[v] = u;
                    // if you have found the sink, return true
                    if (v == sink) return true;
                    q.push(v);
                }
            }
        }
        // returns false if no path found
        return false;
    }

    // actual network flow algorithm
    int edmondsKarp(int source, int sink) {
        int maxFlow = 0;

        // while valid path is found
        while (bfs(source, sink)) {
            // to take first value as min guaranteed
            int flow = INT_MAX;
            // loop through parent vector until source is found
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                flow = min(flow, capacity[u][v]);
            }
            // loop through parent vector until source is found
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                // inversely adds and subtracts flow
                capacity[u][v] -= flow;
                capacity[v][u] += flow;
            }
            // for final flow return
            maxFlow += flow;
        }
        return maxFlow;
    }

    // clears parent vector, adjacency list and capacity matrix
    vector<int> parent;
    vector<vector<int>> adj, capacity;
};

// determines if word can be spelled then prints
void canSpellWordandPrint(vector<string> dice, string word) {
    // for if a word can be spelled, stores dice in correct order
    vector<int> diceOrder;
    int source = 0;
    // finds max sink
    int sink = word.size() + dice.size() + 1;
    // initialize graph
    Graph graph(sink + 1);

    // connect source to each die
    for (int i = 0; i < dice.size(); i++) {
        graph.addEdge(source, i + 1, 1);
    }

    // connect each letter of the word to sink
    for (int i = 0; i < word.size(); i++) {
        graph.addEdge(dice.size() + 1 + i, sink, 1);
    }

    // connect dice to letters they contain
    for (int i = 0; i < dice.size(); i++) {
        for (int j = 0; j < word.size(); j++) {
            if (dice[i].find(word[j]) != string::npos) {
                graph.addEdge(i + 1, dice.size() + 1 + j, 1);
            }
        }
    }

    // finds max flow
    int maxFlow = graph.edmondsKarp(source, sink);
    // if max flow is word size, then word can be spelled
    if (maxFlow == word.size()) {
        // gives vector size and assigns -1s
        diceOrder.assign(word.size(), -1);
        // pushes back dice order into vector
        for (int i = 0; i < dice.size(); ++i) {
            for (int j = 0; j < word.size(); ++j) {
                // if capacity at given location is 1,then die is valid in dice
                // order
                if (graph.capacity[dice.size() + j + 1][i + 1] == 1) {
                    diceOrder[j] = i;
                }
            }
        }
        // prints out all valid die in order
        for (int i = 0; i < diceOrder.size(); i++) {
            // if die isn't last die for gradescripts
            if (i != diceOrder.size() - 1) {
                cout << diceOrder[i] << ",";
            } else {
                cout << diceOrder[i];
            }
        }
        cout << ": " << word << endl;  // prints word if valid
    } else {
        cout << "Cannot spell " << word << endl;  // prints if word is not valid
    }
}

int main(int argc, char* argv[]) {
    // open file streams
    ifstream diceFile(argv[1]), wordFile(argv[2]);

    string word;
    vector<string> dice;

    while (getline(diceFile, word)) {  // pulls lines from dice file
        dice.push_back(word);
    }
    while (getline(wordFile, word)) {  // pulls lines from word file, and assess
                                       // words until there are none
        canSpellWordandPrint(
            dice, word);  // finds max flow and prints for gradescripts
    }
    // closes fstreams
    diceFile.close();
    wordFile.close();
}