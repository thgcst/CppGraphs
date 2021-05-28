#include <ctime>
#include <iostream>

#include "adjacencyMatrix.cpp"
#include "adjacencyVector.cpp"
#include "weightedMatrix.cpp"
#include "weightedVector.cpp"

using namespace std;

int main() {
    weightedMatrix graph("Graphs/grafo_W_1.txt");
    clock_t start = clock();

    graph.shortestPath(1, true, 1000);

    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC << " segundos" << endl;
    return 0;
}