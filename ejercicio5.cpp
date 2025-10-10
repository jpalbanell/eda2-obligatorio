#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/tadMFset/MFset.h"
#include "tads/tadHeap/minHeapAristas.h"

using namespace std;

int main()
{
    int v;
    int e;
    cin >> v;
    cin >> e;
    MFset mfset = crear(v+1);
    MinHeap Heap = crearHeap(e+1);
    for (int i = 0; i < e; i++)
    {
        nodoArista* aux = new nodoArista;
        cin >> aux->v1;
        cin >> aux->v2;
        cin >> aux->peso;
        agregarHeap(Heap, aux);
    }
    int pesoTotal = 0;
    while (!esVacio(Heap))
    {
        int peso = Heap->heap[1]->peso;
        int v1 = Heap->heap[1]->v1;
        int v2 = Heap->heap[1]->v2;
        bool seUnio = unir(mfset, v1,v2);
        if(seUnio) pesoTotal+=peso;
        eliminar(Heap);
    }
    cout << pesoTotal << endl;
    return 0;
}