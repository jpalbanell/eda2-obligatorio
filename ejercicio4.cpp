#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/tadHeap/minHeap.h"

using namespace std;

int main()
{
    // TODO
    int K = 0;
    cin >> K;
    MinHeap H = crearHeap(K);
    for (int i = 0; i < K; i++)
    {
        int N = 0;
        cin >> N;
        for (int j = 0; j < N; j++)
        {
            int num;
            cin >> num;
            agregarFinalLista(H, i, num);
        }
    }
    for (int i = 0; i < K; i++)
    {
        agregarHeap(H, cabezal(H, i));
    }
    
    
    return 0;
}