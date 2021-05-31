#include <algorithm>
// #include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <utility>
#include <vector>

#define INF 0x3f3f3f3f

using namespace std;

class weightedVector {
   public:
    weightedVector(string file);
    int numNodes;
    vector<pair<int, double> >* adjVector;
    void shortestPath(int node1, bool save, int node2);
    double MST(int start, bool save);
    double eccentricity(int start);
};

weightedVector::weightedVector(string file) {
    int* degree;
    int degreeSum;
    int median;
    int node1, node2;
    double weight;
    ifstream graphTxt(file);

    graphTxt >> numNodes;
    adjVector = new vector<pair<int, double> >[numNodes + 1];

    degree = new int[numNodes + 1]();

    while (graphTxt >> node1 >> node2 >> weight) {
        adjVector[node1].push_back(make_pair(node2, weight));
        degree[node1]++;
        adjVector[node2].push_back(make_pair(node1, weight));
        degree[node2]++;
    }

    graphTxt.close();

    sort(degree, degree + numNodes + 1);

    for (int i = 0; i < numNodes + 1; i++) {
        sort(adjVector[i].begin(), adjVector[i].end());
    }

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

void weightedVector::shortestPath(int node1, bool save = true, int node2 = -1) {
    bool hasNegativeWeight = false;
    vector<double> distance(numNodes + 1, INF);
    int* parent;
    parent = new int[numNodes + 1];
    memset(parent, 0, numNodes + 1);
    distance[node1] = 0;
    parent[node1] = -1;
    set<pair<double, int> > toBeVisited;  // nodes to be visited
    toBeVisited.insert(make_pair(distance[node1], node1));
    while (!toBeVisited.empty()) {
        double current_distance = toBeVisited.begin()->first;
        if (current_distance < 0) {
            cout << "Para executar dijkstra todos os pesos devem ser maiores que 0" << endl;
            hasNegativeWeight = true;
            break;
        }
        int current_node = toBeVisited.begin()->second;
        toBeVisited.erase(make_pair(current_distance, current_node));
        for (vector<pair<int, double> >::iterator it = adjVector[current_node].begin(); it != adjVector[current_node].end(); ++it) {
            int neighbor = it->first;
            double weight = it->second;

            if (distance[neighbor] > distance[current_node] + weight) {
                parent[neighbor] = current_node;
                distance[neighbor] = distance[current_node] + weight;
                toBeVisited.insert(make_pair(distance[neighbor], neighbor));
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
                    if (distance[x] != INF) {
                        shortestPath << "[";
                        while (x != node1) {
                            shortestPath << x << ", ";
                            x = parent[x];
                        }
                        shortestPath << x << "]; "
                                     << "distância: ";
                        shortestPath.precision(15);
                        shortestPath << distance[i] << endl;
                        shortestPath.precision(p);
                    } else {
                        shortestPath << "[], "
                                     << "distância: "
                                     << "INFINITO" << endl;
                    }
                }
            }
        } else {
            if (!hasNegativeWeight) {
                x = node2;
                if (x != node1) {
                    shortestPath << "Vértice final: " << x << "; Vértice inicial: " << node1 << "; Caminho: ";
                    if (distance[x] != INF) {
                        shortestPath << "[";
                        while (x != node1) {
                            shortestPath << x << ", ";
                            x = parent[x];
                        }
                        shortestPath << x << "]; "
                                     << "distância: ";
                        shortestPath.precision(15);
                        shortestPath << distance[node2] << endl;
                        shortestPath.precision(p);
                    } else {
                        shortestPath << "[], "
                                     << "distância: "
                                     << "INFINITO" << endl;
                    }
                }
            }
        }
        shortestPath.close();
    }
}

double weightedVector::MST(int start, bool save) {
    vector<double> cost(numNodes + 1, INF);
    cost[start] = 0;
    vector<int> parent(numNodes + 1, -1);
    vector<bool> visited(numNodes + 1, false);
    set<pair<double, int> > toBeVisited;

    toBeVisited.insert(make_pair(cost[start], start));
    parent[start] = 0;

    while (!toBeVisited.empty()) {
        double current_cost = toBeVisited.begin()->first;
        int current_node = toBeVisited.begin()->second;

        visited[current_node] = true;
        toBeVisited.erase(make_pair(current_cost, current_node));

        for (vector<pair<int, double> >::iterator it = adjVector[current_node].begin(); it != adjVector[current_node].end(); ++it) {
            int neighbor = it->first;
            double weight = it->second;
            if (cost[neighbor] > weight && !visited[neighbor]) {
                cost[neighbor] = weight;
                parent[neighbor] = current_node;
                toBeVisited.insert(make_pair(cost[neighbor], neighbor));
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

double weightedVector::eccentricity(int start) {
    bool hasNegativeWeight = false;
    vector<double> distance(numNodes + 1, INF);
    int* parent;
    parent = new int[numNodes + 1];
    memset(parent, 0, numNodes + 1);
    distance[start] = 0;
    parent[start] = -1;
    set<pair<double, int> > toBeVisited;  // nodes to be visited
    toBeVisited.insert(make_pair(distance[start], start));
    while (!toBeVisited.empty()) {
        double current_distance = toBeVisited.begin()->first;
        if (current_distance < 0) {
            cout << "Para executar dijkstra todos os pesos devem ser maiores que 0" << endl;
            hasNegativeWeight = true;
            break;
        }
        int current_node = toBeVisited.begin()->second;
        toBeVisited.erase(make_pair(current_distance, current_node));
        for (vector<pair<int, double> >::iterator it = adjVector[current_node].begin(); it != adjVector[current_node].end(); ++it) {
            int neighbor = it->first;
            double weight = it->second;

            if (distance[neighbor] > distance[current_node] + weight) {
                parent[neighbor] = current_node;
                distance[neighbor] = distance[current_node] + weight;
                toBeVisited.insert(make_pair(distance[neighbor], neighbor));
            }
        }
    }

    double max = 0;
    for (int i = 1; i < numNodes + 1; i++) {
        if (distance[i] > max && distance[i] != INF) {
            max = distance[i];
        }
    }
    return max;
}
