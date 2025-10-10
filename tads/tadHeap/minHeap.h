#include <cassert>
#include <limits>
#include <string>
#include <iostream>
#include "../List.h"
using namespace std;

struct nodoLista{
    int dato;
    nodoLista* sig;
    int nroLista;
};

struct lista
{
    nodoLista* raiz;
    nodoLista* final;
};

struct RepresentacionMinHeap
{
    nodoLista** heap;
    int proxPosLibre;
    int limite;
    lista** arrayListas;
};
typedef RepresentacionMinHeap* MinHeap;

MinHeap crearHeap(int K){
    MinHeap H = new RepresentacionMinHeap;
    H->limite = K + 1;
    H->proxPosLibre = 1;

    H->arrayListas = new lista*[K];
    for (int i = 0; i < K; i++) H->arrayListas[i] = NULL;

    H->heap = new nodoLista*[K + 1];
    for (int i = 0; i <= K; i++) H->heap[i] = NULL;

    return H;
}


void agregarFinalLista(MinHeap& H, int l, int e){
    nodoLista* nuevo = new nodoLista;
    nuevo->dato = e;
    nuevo->nroLista = l;
    nuevo->sig = NULL;

    if (H->arrayListas[l] == NULL){
        lista* nueva = new lista;
        nueva->raiz = nuevo;
        nueva->final = nuevo;
        H->arrayListas[l] = nueva;
    } else {
        H->arrayListas[l]->final->sig = nuevo;
        H->arrayListas[l]->final = nuevo;
    }
}


nodoLista* cabezal(MinHeap& H, int l){
    if (H->arrayListas[l] == NULL || H->arrayListas[l]->raiz == NULL) return NULL;

    nodoLista* ret = H->arrayListas[l]->raiz;
    H->arrayListas[l]->raiz = ret->sig;
    if (H->arrayListas[l]->raiz == NULL) {
        H->arrayListas[l]->final = NULL;
    }
    ret->sig = NULL;
    return ret;
}

void swap(MinHeap& H, int padre, int pos){
    nodoLista* aux = H->heap[padre];
    H->heap[padre] = H->heap[pos];
    H->heap[pos] = aux;
}

void flotar(MinHeap& H, int pos){
   if(pos > 1){
    int padre = pos/2;
    if(H->heap[padre]->dato > H->heap[pos]->dato){
        swap(H, padre, pos);
        flotar(H, padre);
    }
   }
}

void agregarHeap(MinHeap& H, nodoLista* nodo){
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
       if (hijoder < H->proxPosLibre && H->heap[hijoIzq]->dato > H->heap[hijoder]->dato)
       {
        menor = hijoder;
       }
       if(H->heap[menor]->dato < H->heap[pos]->dato) 
       {
        swap(H, pos, menor);
        hundir(H, menor);
       }
    }
}


bool esVacio(MinHeap H){
    return H->proxPosLibre==1;
}

int eliminar(MinHeap& H){
    if (H->heap[1] == NULL) return -1;

    int l = H->heap[1]->nroLista;
    int dato = H->heap[1]->dato;

    nodoLista* aBorrar = H->heap[1];

    H->proxPosLibre--;
    H->heap[1] = H->heap[H->proxPosLibre];
    H->heap[H->proxPosLibre] = NULL;

    delete aBorrar;

    if (!esVacio(H)) hundir(H, 1);

    agregarHeap(H, cabezal(H, l));

    return dato;
}

