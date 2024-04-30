#include <bits/stdc++.h>
using namespace std;

const int INF = 99999;

void floydWarshall(vector<vector<int>>& graph) {
    int n = graph.size();
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (graph[i][k] != INF && graph[k][j] != INF) {
                    graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
                }
            }
        }
    }
}

int steinerTree(const vector<vector<int>>& graph, const vector<int>& terminals) {
    int n = graph.size();
    int numTerminals = terminals.size();
    vector<vector<int>> dp(1 << numTerminals, vector<int>(n, INF));
    for (int i = 0; i < numTerminals; ++i) {
        dp[1 << i][terminals[i]] = 0;
    }
    
    for (int mask = 1; mask < (1 << numTerminals); ++mask) {
        for (int u = 0; u < n; ++u) {
            for (int submask = mask; submask > 0; submask = (submask - 1) & mask) {
                dp[mask][u] = min(dp[mask][u], dp[submask][u] + dp[mask ^ submask][u]);
            }
        }
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                dp[mask][v] = min(dp[mask][v], dp[mask][u] + graph[u][v]);
            }
        }
    }
    
    int minCost = INF;
    for (int u = 0; u < n; ++u) {
        minCost = min(minCost, dp[(1 << numTerminals) - 1][u]);
    }
    
    return minCost;
}

int main() {
    ifstream infile("input1.txt");
    string section, line;
    unordered_map<int, vector<pair<int, int>>> edges;
    vector<int> terminals;

    while (infile >> section) {
        if (section == "SECTION") {
            string type;
            infile >> type;
            if (type == "Graph") {
                int nodes, edges_count;
                infile >> line >> nodes >> line >> edges_count;
                edges.clear();
                for (int i = 0; i < edges_count; ++i) {
                    char edge;
                    int from, to, weight;
                    infile >> edge >> from >> to >> weight;
                    edges[from].push_back({to, weight});
                    edges[to].push_back({from, weight});
                }
            } else if (type == "Terminals") {
                int terminals_count;
                infile >> line >> terminals_count;
                terminals.clear();
                for (int i = 0; i < terminals_count; ++i) {
                    char terminal;
                    int term;
                    infile >> terminal >> term;
                    terminals.push_back(term);
                }
            }
        } else if (section == "EOF") {
            int n = edges.size();
            vector<vector<int>> graph(n, vector<int>(n, INF));
            for (auto& edge : edges) {
                int from = edge.first;
                for (auto& e : edge.second) {
                    int to = e.first;
                    int weight = e.second;
                    graph[from - 1][to - 1] = weight;
                    graph[to - 1][from - 1] = weight;
                }
            }
            
            floydWarshall(graph); 
            int minCost = steinerTree(graph, terminals);
            if (minCost == INF) {
                cout << "No solution found" << endl;
            } else {
                cout << "Minimum cost of the Steiner tree: " << minCost << endl;
            }

            return 0;
        }
    }

    return 0;
}
