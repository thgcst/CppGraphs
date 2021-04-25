#include <ctime>
#include <iostream>

#include "adjacencyMatrix.cpp"
#include "adjacencyVector.cpp"

using namespace std;

int main() {
    adjacencyVector graph("Graphs/grafo_3.txt");
    // adjacencyMatrix graph("Graphs/grafo_2.txt");

    clock_t start = clock();

    for (int i = 0; i < 1000; i++) {
        graph.bfs((i % graph.numNodes) + 1, true);
    }
    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC << " segundos" << endl;
    return 0;
}