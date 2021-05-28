#include <string.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#define INF 0x3f3f3f3f

using namespace std;

class weightedMatrix {
   public:
    weightedMatrix(string file);
    int numNodes;
    double **adjMatrix;
    void shortestPath(int node1, bool save, int node2);
    double MST(int start, bool save);
    double eccentricity(int start);
};

weightedMatrix::weightedMatrix(string file) {
    int *degree;
    int degreeSum;
    int median = 0;
    int node1, node2;
    double weight;
    ifstream graphTxt(file);

    graphTxt >> numNodes;
    adjMatrix = new double *[numNodes + 1];
    for (int i = 0; i < numNodes + 1; i++) {
        adjMatrix[i] = new double[numNodes + 1];
    }

    for (int i = 0; i <= numNodes; i++) {
        for (int j = 0; j <= numNodes; j++) {
            adjMatrix[i][j] = INF;
        }
    }

    degree = new int[numNodes + 1]();

    while (graphTxt >> node1 >> node2 >> weight) {
        if (adjMatrix[node1][node2] == INF) {
            adjMatrix[node1][node2] = weight;
            degree[node1]++;
        }
        if (adjMatrix[node2][node1] == INF) {
            adjMatrix[node2][node1] = weight;
            degree[node2]++;
        }
    }

    graphTxt.close();

    sort(degree, degree + numNodes + 1);

    degreeSum = accumulate(degree, degree + numNodes + 1, 0);

    if (numNodes % 2 == 0) {
        median = (degree[numNodes / 2] + degree[(numNodes / 2) + 1]) / 2;
    } else {
        median = degree[((int)numNodes / 2) + 1];
    };

    ofstream graphSummary;
    graphSummary.open("Outputs/graphSummary.txt");
    graphSummary << "Number of nodes: " << numNodes << endl;
    graphSummary << "Number of edges: " << degreeSum / 2 << endl;
    graphSummary << "Minimum degree: " << degree[1] << endl;
    graphSummary << "Maximum degree: " << degree[numNodes] << endl;
    graphSummary << "Degree average: " << (float)degreeSum / numNodes << endl;
    graphSummary << "Degree median: " << median << endl;
    graphSummary.close();
};

void weightedMatrix::shortestPath(int node1, bool save = true, int node2 = -1) {
    bool hasNegativeWeight = false;
    vector<double> dist(numNodes + 1, INF);
    int *parent;
    parent = new int[numNodes + 1];
    memset(parent, 0, numNodes + 1);
    dist[node1] = 0;
    parent[node1] = -1;
    set<pair<double, int> > toBeVisited;  // nodes to be visited
    toBeVisited.insert(make_pair(dist[node1], node1));
    while (!toBeVisited.empty()) {
        double current_dist = toBeVisited.begin()->first;
        if (current_dist < 0) {
            cout << "Para executar dijkstra todos os pesos devem ser maiores que 0" << endl;
            hasNegativeWeight = true;
            break;
        }
        int current_vertex = toBeVisited.begin()->second;
        toBeVisited.erase(make_pair(current_dist, current_vertex));
        for (int i = 1; i < numNodes + 1; i++) {
            if (adjMatrix[current_vertex][i] > 0) {
                if (dist[i] > dist[current_vertex] + adjMatrix[current_vertex][i]) {
                    parent[i] = current_vertex;
                    dist[i] = dist[current_vertex] + adjMatrix[current_vertex][i];
                    toBeVisited.insert(make_pair(dist[i], i));
                }
            }
        }
    }
    if (save) {
        ofstream shortestPath;
        shortestPath.open("Outputs/shortestPath.txt");
        streamsize p = shortestPath.precision();
        int x = 0;
        if (node2 == -1 && !hasNegativeWeight) {
            for (int i = 1; i < numNodes + 1; i++) {
                x = i;
                if (x != node1) {
                    shortestPath << "Vértice final: " << x << "; Vértice inicial: " << node1 << "; Caminho: ";
                    if (dist[x] != INF) {
                        shortestPath << "[";
                        while (x != node1) {
                            shortestPath << x << ", ";
                            x = parent[x];
                        }
                        shortestPath << x << "]; "
                                     << "Distância: ";
                        shortestPath.precision(15);
                        shortestPath << dist[i] << endl;
                        shortestPath.precision(p);
                    } else {
                        shortestPath << "[], "
                                     << "Distância: "
                                     << "INFINITO" << endl;
                    }
                }
            }
        } else {
            if (!hasNegativeWeight) {
                x = node2;
                if (x != node1) {
                    shortestPath << "Vértice final: " << x << "; Vértice inicial: " << node1 << "; Caminho: ";
                    if (dist[x] != INF) {
                        shortestPath << "[";
                        while (x != node1) {
                            shortestPath << x << ", ";
                            x = parent[x];
                        }
                        shortestPath << x << "]; "
                                     << "Distância: ";
                        shortestPath.precision(15);
                        shortestPath << dist[node2] << endl;
                        shortestPath.precision(p);
                    } else {
                        shortestPath << "[], "
                                     << "Distância: "
                                     << "INFINITO" << endl;
                    }
                }
            }
        }
        shortestPath.close();
    }
}

double weightedMatrix::MST(int start, bool save = true) {
    vector<double> cost(numNodes + 1, INF);
    cost[start] = 0;
    vector<int> parent(numNodes + 1, -1);
    vector<bool> visited(numNodes + 1, false);
    set<pair<double, int> > toBeVisited;

    toBeVisited.insert(make_pair(cost[start], start));
    parent[start] = 0;

    while (!toBeVisited.empty()) {
        double current_cost = toBeVisited.begin()->first;
        int current_vertex = toBeVisited.begin()->second;
        visited[current_vertex] = true;
        toBeVisited.erase(make_pair(current_cost, current_vertex));

        for (int i = 1; i < numNodes + 1; i++) {
            if (adjMatrix[current_vertex][i] != INF) {
                if (cost[i] > adjMatrix[current_vertex][i] && !visited[i]) {
                    cost[i] = adjMatrix[current_vertex][i];
                    toBeVisited.insert(make_pair(cost[i], i));
                    parent[i] = current_vertex;
                }
            }
        }
    }

    double total_cost = 0;
    for (int i = 1; i <= numNodes; i++) {
        if (cost[i] != INF) {
            total_cost += cost[i];
        }
    }
    if (save) {
        ofstream MST;
        MST.open("Outputs/MST.txt");
        MST.precision(15);
        MST << "total cost: " << total_cost << endl;
        for (int i = 1; i < numNodes + 1; i++) {
            if (parent[i] != 0) {
                MST << i << " " << parent[i] << " " << cost[i] << endl;
            }
        }
        MST.close();
    }
    return total_cost;
}

double weightedMatrix::eccentricity(int start) {
    vector<double> dist(numNodes + 1, INF);
    dist[start] = 0;
    set<pair<double, int> > toBeVisited;
    toBeVisited.insert(make_pair(dist[start], start));
    while (!toBeVisited.empty()) {
        double current_dist = toBeVisited.begin()->first;
        if (current_dist < 0) {
            cout << "Para executar dijkstra todos os pesos devem ser maiores que 0" << endl;
            break;
        }
        int current_vertex = toBeVisited.begin()->second;
        toBeVisited.erase(make_pair(current_dist, current_vertex));
        for (int i = 1; i < numNodes + 1; i++) {
            if (adjMatrix[current_vertex][i] > 0) {
                if (dist[i] > dist[current_vertex] + adjMatrix[current_vertex][i]) {
                    dist[i] = dist[current_vertex] + adjMatrix[current_vertex][i];
                    toBeVisited.insert(make_pair(dist[i], i));
                }
            }
        }
    }
    double max = 0;
    for (int i = 1; i < numNodes + 1; i++) {
        if (dist[i] > max && dist[i] != INF) {
            max = dist[i];
        }
    }
    return max;
}