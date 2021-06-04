#include <ctime>
#include <iostream>

#include "adjacencyMatrix.cpp"
#include "adjacencyVector.cpp"
#include "weightedMatrix.cpp"
#include "weightedVector.cpp"

using namespace std;

int main()
{
    weightedVector graph("Graphs/rede_colaboracao.txt");
    clock_t start = clock();

    graph.shortestPath(2722,true,343930);
    //cout << graph.MST(1, true) << endl;
    /*for (int n = 0; n <= 100; n++)
    {
        cout << graph.eccentricity(n) << endl;
    }*/
    //cout << graph.eccentricity(10) << endl;

    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC << " segundos" << endl;
    return 0;
}