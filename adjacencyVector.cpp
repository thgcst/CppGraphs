#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

class adjacencyVector {
   public:
    adjacencyVector(string file);
    int numNodes;
    vector<int> *adjVector;
    void bfs(int root);
};

adjacencyVector::adjacencyVector(string file) {
    int *degree;
    int degreeSum;
    int median;
    int node1, node2;
    ifstream graphTxt(file);

    graphTxt >> numNodes;
    adjVector = new vector<int>[numNodes + 1];

    degree = new int[numNodes + 1]();

    while (graphTxt >> node1 >> node2) {
        if (find(adjVector[node1].begin(), adjVector[node1].end(), node2) == adjVector[node1].end()) {
            adjVector[node1].push_back(node2);
            degree[node1]++;
        }
        if (find(adjVector[node2].begin(), adjVector[node2].end(), node1) == adjVector[node2].end()) {
            adjVector[node2].push_back(node1);
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

void adjacencyVector::bfs(int root) {
    int *level;
    level = new int[numNodes + 1];
    int *parent;
    parent = new int[numNodes + 1];

    for (int i = 0; i < (int)numNodes + 1; i++) {
        level[i] = -1;
    };

    memset(parent, 0, numNodes + 1);

    queue<int> bfsQueue;

    bfsQueue.push(root);

    level[root] = 0;
    parent[root] = -1;

    while (!bfsQueue.empty()) {
        int v = bfsQueue.front();
        bfsQueue.pop();
        for (int i = 0; i < (int)adjVector[v].size(); i++) {
            if (level[adjVector[v][i]] == -1) {
                parent[adjVector[v][i]] = v;
                level[adjVector[v][i]] = level[v] + 1;
                bfsQueue.push(adjVector[v][i]);
            }
        }
    };

    ofstream bfsFile;

    bfsFile.open("Outputs/bfs.txt");
    for (int i = 1; i < (int)numNodes; i++) {
        bfsFile << "Vértice: " << i << ", Nível: " << level[i] << ", Pai: " << parent[i] << endl;
    }
    bfsFile.close();
}