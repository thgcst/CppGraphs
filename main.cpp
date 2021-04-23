#include <ctime>
#include <iostream>

#include "adjacencyMatrix.cpp"
#include "adjacencyVector.cpp"

using namespace std;

int main() {
    adjacencyMatrix graph("Graphs/grafo_3.txt");
    clock_t start = clock();
    graph.dfs(4);
    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC << " segundos" << endl;
    return 0;
}