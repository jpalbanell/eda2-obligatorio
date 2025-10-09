#include <cassert>
#include <limits>
#include <string>
#include <iostream>
using namespace std;


struct nodoGrafo{
    int vertice;
    nodoGrafo* sig;
    int peso;

};


struct RepresentaciongrafoLA{
    nodoGrafo** tabla;
    int cantV;
    int cantE;
};
typedef RepresentaciongrafoLA* grafoLA;


grafoLA crear(int V, int E){
    grafoLA nuevo = new RepresentaciongrafoLA;
    nuevo->tabla= new nodoGrafo*[V];
    for (int i = 0; i < V; i++) nuevo->tabla[i] = NULL;
    return nuevo;
}


void agregarArista(grafoLA& G,int u, int v, int peso){
    //agregar arista u->v
    nodoGrafo* a = new nodoGrafo;
    a->peso=peso;
    a->vertice=v;
    a->sig = G->tabla[u];
    G->tabla[u] = a;
    //agregar arista v->u
    nodoGrafo* b = new nodoGrafo;
    b->peso=peso;
    b->vertice=u;
    b->sig = G->tabla[v];
    G->tabla[v] = b;
}

nodoGrafo* adyacentes(grafoLA G, int v){
    return G->tabla[v];
}
