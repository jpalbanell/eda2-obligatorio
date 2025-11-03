#include <cassert>
#include <limits>
#include <string>
#include <iostream>
using namespace std;

struct nodoEstudiante {
    string nombre;
    int pos;
};

struct RepresentacionHash {
    nodoEstudiante** tablaEst;
    int cantElem;  
    int buckets;   
};
typedef RepresentacionHash* Hash;

// ---------- hashing  ----------
int hash3(string key) {
  int h = 0;
  for (int i = 0; i < key.length(); i++)
    h = 31 * h + int(key[i]);
  return h;
}

// diapositivas de la clase
int hashSec(string key) {
  int h = 0;
  for (int i = 0; i < key.length(); i++)
    h = (37 * h + int(key[i]))* (i*i);
  return h;
}
int mod(int a, int m) { int r = a % m; return r < 0 ? r + m : r; }

Hash crear(int n) {
    int B = n * 2;
    if (B < 3) B = 3;       
    if ((B & 1) == 0) B++; 

    Hash A = new RepresentacionHash;
    A->buckets = B;
    A->cantElem = 0;

    A->tablaEst = new nodoEstudiante*[B];

    for (int i = 0; i < B; ++i) {
        A->tablaEst[i] = NULL;
    }
    return A;
}


void put(Hash& A, string nombre, int pos) {
    int B  = A->buckets;
    int h1 = mod(hash3(nombre), B);
    int h2 = 1 + mod(hashSec(nombre), B - 1); 

    int intento = 0;
    while (intento < B) {
        int i = (h1 + intento * h2) % B;

        if (A->tablaEst[i] == NULL) {
            nodoEstudiante* n = new nodoEstudiante;
            n->nombre = nombre;
            n->pos    = pos;
            A->tablaEst[i] = n;
            A->cantElem++;
            return;
        }
        if (A->tablaEst[i]->nombre == nombre) {
            // actualizar, aunque no se deberia usar
            A->tablaEst[i]->pos = pos;
            return;
        }
        intento++;
    }

}

int get(Hash A, string nombre) {
    int B  = A->buckets;
    int h1 = mod(hash3(nombre), B);
    int h2 = 1 + mod(hashSec(nombre), B - 1);

    int intento = 0;
    while (intento < B) {
        int i = (h1 + intento * h2) % B;
        nodoEstudiante* est = A->tablaEst[i];

        if (est == NULL) return -1;
        if (est->nombre == nombre) return est->pos;

        intento++;
    }
    return -1;
}

int size(Hash A) { return A->cantElem; }
