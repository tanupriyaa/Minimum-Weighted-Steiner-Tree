#include <bits/stdc++.h>
using namespace std;

const int INF = 9999999;

// Dijkstra's algorithm for finding shortest paths from a single source
vector<int> dijkstra(vector<vector<pair<int, int> > >& adjList, int nodes, int source) {
    vector<int> dist(nodes, INF);
    dist[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    pq.push(make_pair(0, source));

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (int i = 0; i < adjList[u].size(); ++i) {
            int v = adjList[u][i].first;
            int weight = adjList[u][i].second;

            if (dist[v] == INF || dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    return dist;
}

// Construction of the metric closure of the graph based on terminals
vector<vector<int> > metricClosure(vector<vector<pair<int, int> > >& adjList, int nodes, const vector<int>& terminals) {
    vector<vector<int> > metric(nodes, vector<int>(nodes, INF));

    for (int k = 0; k < terminals.size(); ++k) {
        int u = terminals[k];
        vector<int> dist = dijkstra(adjList, nodes, u);
        for (int i = 0; i < nodes; ++i) {
            for (int j = 0; j < nodes; ++j) {
                if (dist[i] != INF && dist[j] != INF) {
                    metric[i][j] = min(metric[i][j], max(dist[i], dist[j]));
                } else {
                    metric[i][j] = 0; // Replace INF with zero
                }
            }
        }
    }
    cout << "Metric Closure Matrix:" << endl;
    for (int i = 0; i < nodes; ++i) {
        for (int j = 0; j < nodes; ++j) {
            cout << metric[i][j] << " ";
        }
        cout << endl;
    }

    return metric;
}

//MST of the metric closure graph
int findMST(vector<vector<int> >& metric, vector<int>& parent) {
    int n = metric.size();
    parent.resize(n, -1);
    vector<int> key(n, INF);
    vector<bool> inMST(n, false);

    key[0] = 0;
    int mstWeight = 0;

    for (int count = 0; count < n - 1; ++count) {
        int u = -1;
        for (int v = 0; v < n; ++v) {
            if (!inMST[v] && (u == -1 || key[v] < key[u])) {
                u = v;
            }
        }

        inMST[u] = true;

        for (int v = 0; v < n; ++v) {
            if (metric[u][v] < key[v] && !inMST[v]) {
                parent[v] = u;
                key[v] = metric[u][v];
            }
        }
    }

    for (int i = 1; i < n; ++i) {
        mstWeight += metric[i][parent[i]];
    }
    cout << "Weight of Minimum Spanning Tree (MST): " << mstWeight << endl;

    return mstWeight;
}

// Construct the Steiner tree from the MST
int constructSteinerTree(vector<vector<int> >& metric, const vector<int>& terminals, const vector<int>& parent) {
    int steinerWeight = 0;
    int n = terminals.size();
    
    for (int i = 1; i < n; ++i) {
        steinerWeight += metric[i][parent[i]];
    }

    for (int i = 0; i < terminals.size(); ++i) {
        int u = terminals[i];
        int minDist = INF;
        for (int j = 0; j < terminals.size(); ++j) {
            int v = terminals[j];
            if (u != v) {
                minDist = min(minDist, metric[u][v]);
            }
        }
        steinerWeight += minDist;
    }

    return steinerWeight;
}

int main() {
    ifstream inputFile("input1.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    string line;
    int nodes = 0;
    vector<vector<pair<int, int> > > adjList;

    vector<int> terminals;

    while (getline(inputFile, line)) {
        if (line == "SECTION Graph") {
            int edges;
            inputFile >> line >> nodes >> line >> edges;
            adjList.resize(nodes);
            for (int i = 0; i < edges; ++i) {
                int u, v, weight;
                inputFile >> line >> u >> v >> weight;
                adjList[u - 1].push_back(make_pair(v - 1, weight));
                adjList[v - 1].push_back(make_pair(u - 1, weight));
            }
        } else if (line == "SECTION Terminals") {
            int terminalsCount;
            inputFile >> line >> terminalsCount;
            terminals.clear();
            for (int i = 0; i < terminalsCount; ++i) {
                int terminal;
                inputFile >> line >> terminal;
                terminals.push_back(terminal - 1);
            }

            vector<vector<int> > metric = metricClosure(adjList, nodes, terminals);
            vector<int> parent;
            int mstWeight = findMST(metric, parent);
            int steinerWeight = constructSteinerTree(metric, terminals, parent);

            cout << "Steiner Tree weight: " << steinerWeight << endl;
        }
    }

    inputFile.close();
    return 0;
}
