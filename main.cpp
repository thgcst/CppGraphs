#include <ctime>
#include <iostream>

#include "adjacencyMatrix.cpp"
#include "adjacencyVector.cpp"

using namespace std;

int main() {
    clock_t start = clock();
    // adjacencyVector graph("Graphs/grafo_3.txt");
    adjacencyMatrix graph("Graphs/grafo_3.txt");

    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC << " segundos" << endl;
    return 0;
}