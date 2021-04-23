#include <string.h>

#include <algorithm>
// #include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

class adjacencyMatrix {
   private:
    int diameterBFS(int root, int biggestBfsLevel);
    vector<int> bfsCC(int root, vector<int> &discovered, int constant);

   public:
    adjacencyMatrix(string file);
    int numNodes;
    bool **adjMatrix;
    void bfs(int root);
    void dfs(int root);
    int connectedComponents();
    int distanceBetween(int node1, int node2);
    int diameter();
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

void adjacencyMatrix::bfs(int root) {
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
        for (int i = 1; i < numNodes + 1; i++) {
            if (adjMatrix[v][i] && level[i] == -1) {
                parent[i] = v;
                level[i] = level[v] + 1;
                bfsQueue.push(i);
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

void adjacencyMatrix::dfs(int root) {
    int *level;
    level = new int[numNodes + 1];
    int *parent;
    parent = new int[numNodes + 1];
    bool *checked;
    checked = new bool[numNodes + 1];

    for (int i = 0; i < (int)numNodes + 1; i++) {
        level[i] = -1;
    };

    memset(parent, 0, numNodes + 1);
    memset(checked, 0, numNodes + 1);

    stack<int> dfsStack;

    dfsStack.push(root);
    level[root] = 0;
    parent[root] = -1;

    while (!dfsStack.empty()) {
        int u = dfsStack.top();
        dfsStack.pop();
        if (!checked[u]) {
            checked[u] = true;
            for (int i = (int)numNodes; i > 0; i--) {
                if (adjMatrix[u][i] == 1) {
                    if (!checked[i]) {
                        level[i] = level[u] + 1;
                        parent[i] = u;
                    }
                    dfsStack.push(i);
                }
            }
        }
    }

    ofstream dfsFile;

    dfsFile.open("Outputs/dfs.txt");
    for (int i = 1; i < (int)numNodes + 1; i++) {
        dfsFile << "Vértice: " << i << ", Nível: " << level[i] << ", Pai: " << parent[i] << endl;
    }
    dfsFile.close();
}

vector<int> adjacencyMatrix::bfsCC(int root, vector<int> &discovered, int constant) {
    vector<int> components;
    components.push_back(root);

    queue<int> queueBfsCC;

    discovered[root] = constant;

    queueBfsCC.push(root);

    while (!queueBfsCC.empty()) {
        int vertex = queueBfsCC.front();
        queueBfsCC.pop();
        for (int i = 0; i < (int)numNodes + 1; i++) {
            int neighbor = i;
            if (adjMatrix[vertex][i] && !discovered[i]) {
                discovered[i] = constant;
                components.push_back(i);
                queueBfsCC.push(i);
            }
        }
    }

    return components;
}

bool compareMatrixCC(vector<int> first, vector<int> second) {
    return first.size() > second.size();
};

int adjacencyMatrix::connectedComponents() {
    vector<int> discovered(numNodes + 1, 0);

    vector<vector<int> > nodesCC;

    int constant = 0;
    for (int i = 1; i < numNodes + 1; i++) {
        if (!discovered[i]) {
            constant++;
            nodesCC.push_back(bfsCC(i, discovered, constant));
        }
    }

    ofstream saida;
    saida.open("Outputs/connectedComponents.txt");
    saida << "Número de componentes conexas: " << nodesCC.size() << "\n"
          << endl;
    sort(nodesCC.begin(), nodesCC.end(), compareMatrixCC);
    for (int i = 0; i < (int)nodesCC.size(); i++) {
        saida << "Tamanho da " << i + 1 << "ª Componente conexa: " << nodesCC[i].size() << endl;
        saida << "Vértices que a compõem:   ";
        for (int j = 0; j < (int)nodesCC[i].size(); j++) {
            saida << nodesCC[i][j] << " ";
        }
        saida << "\n"
              << endl;
    }

    return constant;
}

int adjacencyMatrix::distanceBetween(int node1, int node2) {
    int *level;
    level = new int[numNodes + 1];

    for (int i = 0; i < (int)numNodes + 1; i++) {
        level[i] = -1;
    };

    queue<int> bfsQueue;

    bfsQueue.push(node1);

    level[node1] = 0;

    while (!bfsQueue.empty()) {
        int v = bfsQueue.front();
        bfsQueue.pop();
        for (int i = 1; i < numNodes + 1; i++) {
            if (adjMatrix[v][i] && level[i] == -1) {
                level[i] = level[v] + 1;
                if (i == node2) {
                    return level[i];
                }
                bfsQueue.push(i);
            }
        }
    };

    return -1;
}

int adjacencyMatrix::diameterBFS(int root, int biggestBfsLevel) {
    int biggest = biggestBfsLevel;

    int *level;
    level = new int[numNodes + 1];

    for (int i = 0; i < (int)numNodes + 1; i++) {
        level[i] = -1;
    };

    queue<int> bfsQueue;

    bfsQueue.push(root);

    level[root] = 0;

    while (!bfsQueue.empty()) {
        int v = bfsQueue.front();
        bfsQueue.pop();
        for (int i = 1; i < numNodes + 1; i++) {
            if (adjMatrix[v][i] && level[i] == -1) {
                level[i] = level[v] + 1;
                if (level[i] > biggest) {
                    biggest = level[i];
                }
                bfsQueue.push(i);
            }
        }
    };

    return biggest;
}

int adjacencyMatrix::diameter() {
    int biggestBfsLevel = 0;

    for (int i = 1; i < numNodes + 1; i++) {
        biggestBfsLevel = diameterBFS(i, biggestBfsLevel);
    }

    return biggestBfsLevel;
}