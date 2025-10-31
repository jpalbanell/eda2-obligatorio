#include <cassert>
#include <limits>
#include <string>
#include <iostream>
#include "../List.h"
using namespace std;

struct nodoPotencia
{
    int pos;
    int potencia;
};

struct RepresentacionMaxHeap
{
    nodoPotencia **heap;
    int proxPosLibre;
    int limite;
};
typedef RepresentacionMaxHeap *MaxHeap;

MaxHeap crearHeap(int K)
{
    MaxHeap H = new RepresentacionMaxHeap;
    H->limite = K + 1;
    H->proxPosLibre = 1;

    H->heap = new nodoPotencia *[K + 1];
    for (int i = 0; i <= K; i++)
        H->heap[i] = NULL;

    return H;
}

void swap(MaxHeap &H, int padre, int pos)
{
    nodoPotencia *aux = H->heap[padre];
    H->heap[padre] = H->heap[pos];
    H->heap[pos] = aux;
}

void flotar(MaxHeap &H, int pos)
{
    if (pos > 1)
    {
        int padre = pos / 2;
        if (H->heap[padre]->potencia < H->heap[pos]->potencia)
        {
            swap(H, padre, pos);
            flotar(H, padre);
        }
    }
}

void agregarHeap(MaxHeap &H, nodoPotencia *nodo)
{
    if (!nodo)
        return;
    if (H->proxPosLibre == H->limite)
    {
        cout << "error" << endl;
        return;
    }
    H->heap[H->proxPosLibre] = nodo;
    flotar(H, H->proxPosLibre);
    H->proxPosLibre++;
}

void hundir(MaxHeap &H, int pos)
{
    int hijoIzq = pos * 2;
    int hijoder = (pos * 2) + 1;
    if (hijoIzq < H->proxPosLibre)
    {
        int menor = hijoIzq;
        if (hijoder < H->proxPosLibre && H->heap[hijoIzq]->potencia < H->heap[hijoder]->potencia)
        {
            menor = hijoder;
        }
        if (H->heap[menor]->potencia > H->heap[pos]->potencia)
        {
            swap(H, pos, menor);
            hundir(H, menor);
        }
    }
}

bool esVacio(MaxHeap H)
{
    return H->proxPosLibre == 1;
}

int eliminar(MaxHeap &H)
{
    if (H->heap[1] == NULL)
        return 0;

    nodoPotencia *aBorrar = H->heap[1];
    int res = aBorrar->potencia;

    H->proxPosLibre--;
    H->heap[1] = H->heap[H->proxPosLibre];
    H->heap[H->proxPosLibre] = NULL;

    delete aBorrar;

    if (!esVacio(H))
        hundir(H, 1);
    return res;
}
