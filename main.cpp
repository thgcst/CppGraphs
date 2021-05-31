#include <ctime>
#include <iostream>

#include "adjacencyMatrix.cpp"
#include "adjacencyVector.cpp"
#include "weightedMatrix.cpp"
#include "weightedVector.cpp"

using namespace std;

int main() {
    weightedVector graph("Graphs/grafo_W_3.txt");
    clock_t start = clock();

    cout << graph.eccentricity(1) << endl;

    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC << " segundos" << endl;
    return 0;
}