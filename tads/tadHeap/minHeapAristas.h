#include <cassert>
#include <limits>
#include <string>
#include <iostream>
using namespace std;

struct nodoArista{
    int v1;
    int v2;
    int peso;
};

struct RepresentacionMinHeapArista
{
    nodoArista** heap;
    int proxPosLibre;
    int limite;
};
typedef RepresentacionMinHeapArista* MinHeap;

MinHeap crearHeap(int E){
    MinHeap H = new RepresentacionMinHeapArista;
    H->limite = E + 1;          
    H->proxPosLibre = 1;
    H->heap = new nodoArista*[E + 1];     
    for (int i = 0; i <= E; i++) H->heap[i] = NULL;

    return H;
}

void swap(MinHeap& H, int padre, int pos){
    nodoArista* aux = H->heap[padre];
    H->heap[padre] = H->heap[pos];
    H->heap[pos] = aux;
}

void flotar(MinHeap& H, int pos){
   if(pos > 1){
    int padre = pos/2;
    if(H->heap[padre]->peso > H->heap[pos]->peso){
        swap(H, padre, pos);
        flotar(H, padre);
    }
   }
}

void agregarHeap(MinHeap& H, nodoArista* nodo){
    if (!nodo) return;           
    if (H->proxPosLibre == H->limite){
        cout << "error" << endl;
        return;
    }
    H->heap[H->proxPosLibre] = nodo;
    flotar (H, H->proxPosLibre);
    H->proxPosLibre++;
}

void hundir(MinHeap& H, int pos){
    int hijoIzq = pos *2;
    int hijoder = (pos *2)+1;
    if (hijoIzq < H->proxPosLibre)
    {
       int menor = hijoIzq;
       if (hijoder < H->proxPosLibre && H->heap[hijoIzq]->peso > H->heap[hijoder]->peso)
       {
        menor = hijoder;
       }
       if(H->heap[menor]->peso < H->heap[pos]->peso) 
       {
        swap(H, pos, menor);
        hundir(H, menor);
       }
    }
}


bool esVacio(MinHeap H){
    return H->proxPosLibre==1;
}

void eliminar(MinHeap& H){
    if (H->heap[1] == NULL) return;

    nodoArista* aBorrar = H->heap[1];

    // mover el último a la raíz y achicar
    H->proxPosLibre--;
    H->heap[1] = H->heap[H->proxPosLibre];
    H->heap[H->proxPosLibre] = NULL;

    delete aBorrar;

    // restaurar propiedad de heap
    if (!esVacio(H)) hundir(H, 1);
}

