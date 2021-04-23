#include <algorithm>
// #include <ctime>
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
    void dfs(int root);
    vector<int> bfsCC(int root, vector<int> &discovered, int constant);
    int connectedComponents();
    int distanceBetween(int node1, int node2);
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
                level[adjVector[v][i]] = level[v] + 1;
                parent[adjVector[v][i]] = v;
                bfsQueue.push(adjVector[v][i]);
            }
        }
    };

    ofstream bfsFile;

    bfsFile.open("Outputs/bfs.txt");
    for (int i = 1; i < (int)numNodes + 1; i++) {
        bfsFile << "Vértice: " << i << ", Nível: " << level[i] << ", Pai: " << parent[i] << endl;
    }
    bfsFile.close();
}

void adjacencyVector::dfs(int root) {
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

    while (dfsStack.empty() != 1) {
        int u = dfsStack.top();
        dfsStack.pop();
        if (!checked[u]) {
            checked[u] = true;
            for (int i = (int)adjVector[u].size() - 1; i >= 0; i--) {
                if (!checked[adjVector[u][i]]) {
                    level[adjVector[u][i]] = level[u] + 1;
                    parent[adjVector[u][i]] = u;
                }
                dfsStack.push(adjVector[u][i]);
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

vector<int> adjacencyVector::bfsCC(int root, vector<int> &discovered, int constant) {
    vector<int> components;
    components.push_back(root);

    queue<int> queueBfsCC;

    discovered[root] = constant;

    queueBfsCC.push(root);

    while (!queueBfsCC.empty()) {
        int vertex = queueBfsCC.front();
        queueBfsCC.pop();
        for (int i = 0; i < (int)adjVector[vertex].size(); i++) {
            int neighbor = i;
            if (!discovered[adjVector[vertex][neighbor]]) {
                discovered[adjVector[vertex][neighbor]] = constant;
                components.push_back(adjVector[vertex][neighbor]);
                queueBfsCC.push(adjVector[vertex][neighbor]);
            }
        }
    }

    return components;
}

bool compareVectorCC(vector<int> first, vector<int> second) {
    return first.size() > second.size();
};

int adjacencyVector::connectedComponents() {
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
    sort(nodesCC.begin(), nodesCC.end(), compareVectorCC);
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

int adjacencyVector::distanceBetween(int node1, int node2) {
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
        for (int i = 0; i < (int)adjVector[v].size(); i++) {
            if (level[adjVector[v][i]] == -1) {
                level[adjVector[v][i]] = level[v] + 1;
                if (adjVector[v][i] == node2) {
                    return level[adjVector[v][i]];
                }
                bfsQueue.push(adjVector[v][i]);
            }
        }
    };

    return -1;
}