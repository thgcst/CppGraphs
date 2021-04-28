#include <ctime>
#include <iostream>

#include "adjacencyMatrix.cpp"
#include "adjacencyVector.cpp"

using namespace std;

int main() {
    adjacencyVector graph("Graphs/grafo_4.txt");
    // adjacencyMatrix graph("Graphs/grafo_3.txt");

    clock_t start = clock();

    cout << graph.diameter() << endl;

    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC << " segundos" << endl;
    return 0;
}