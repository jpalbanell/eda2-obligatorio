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
    int cantElem;
    lista** arrayListas;
};
typedef RepresentacionMinHeap* MinHeap;

MinHeap crearHeap(int K){
    MinHeap H = new RepresentacionMinHeap;
    H -> cantElem = K;
    H -> arrayListas = new lista*[K];
    H -> heap = new nodoLista*[K+1];
    for (int i = 0; i < K; i++)
    {
        H->arrayListas[i] = NULL;
        H->heap[i+1] = NULL;
    }
    H->heap[0] = NULL;
    
}

void agregarFinalLista(MinHeap& H, int l, int e){
    nodoLista* nuevo = new nodoLista;
    nuevo->dato = e;
    nuevo->nroLista = l;
    nuevo->sig=NULL;
    if(H->arrayListas[l] == NULL){
        H->arrayListas[l]->final = H->arrayListas[l]->raiz = nuevo;
    } else {
        H->arrayListas[l]->final->sig = nuevo;
        H->arrayListas[l]->final = nuevo;
    }
}

nodoLista* cabezal(MinHeap& H, int l){
    nodoLista* ret;
    ret = H->arrayListas[l]->raiz;
    nodoLista* aBorrar = H->arrayListas[l]->raiz;
    H->arrayListas[l]->raiz = H->arrayListas[l]->raiz->sig;
    delete aBorrar;
    return ret;
}

void agregarHeap(MinHeap& H, nodoLista* nodo){
    if ()
    {
        /* code */
    }
    
}