#include <string.h>

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

class adjacencyMatrix {
   public:
    adjacencyMatrix(string file);
    int numNodes;
    bool **adjMatrix;
};

adjacencyMatrix::adjacencyMatrix(string file) {
    int *degree;
    int degreeSum;
    int median;
    int node1, node2;
    ifstream graphTxt(file);

    graphTxt >> numNodes;
    adjMatrix = new bool *[numNodes + 1];
    for (int i = 0; i < numNodes + 1; i++) {
        adjMatrix[i] = new bool[numNodes + 1];
    }

    for (int i = 0; i < numNodes + 1; i++) {
        memset(adjMatrix[i], false, numNodes + 1);
    }

    degree = new int[numNodes + 1]();

    while (graphTxt >> node1 >> node2) {
        if (!adjMatrix[node1][node2]) {
            adjMatrix[node1][node2] = true;
            degree[node1]++;
        }
        if (!adjMatrix[node2][node1]) {
            adjMatrix[node2][node1] = true;
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
