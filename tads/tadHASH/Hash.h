#include <string>
#include <cassert>
#include <iostream>
#include <limits>

using namespace std; 

struct NodoHash
{
    string path;
    string dom;
    string titulo;
    int tiempo;
};

struct RepresentacionHash
{
    NodoHash** tabla;
    int cantElem;
    int buckets;
};
typedef RepresentacionHash *Hash; 

Hash crear(int esperados){
    Hash nuevo = new RepresentacionHash;
    nuevo -> cantElem = 0;
    nuevo -> buckets = esperados;
    nuevo -> tabla = new NodoHash*[esperados]();
    return nuevo;
}

Hash crear(){
    Hash nuevo = new RepresentacionHash;
    nuevo -> cantElem = 0;
    nuevo -> buckets = 103;
    nuevo -> tabla = new NodoHash*[103]();
    return nuevo;
}

// Ref: https://cseweb.ucsd.edu/~kube/cls/100/Lectures/lec16/lec16-15.html
// diapositivas de la clase
int hash3(string key) {
  int h = 0;
  for (int i = 0; i < key.length(); i++) h = 31 * h + int(key[i]);
  return h;
}

int hashSec(string key2){
  int h = 0;
  for (int i = 0; i < key2.length(); i++) h = 37 * h + int(key2[i]);
  return h;
}


void put(Hash& A, string dom, string path, string titulo, int tiempo){
  int indice = hash3(dom);
  int indice2 = hashSec(dom);
  int intento = 0;
  int pos = (indice + indice2*intento)%(A->buckets);
  bool esta = false;
  while (A->tabla[pos] != NULL && !esta)
  {
    intento+=1;
    if (A->tabla[pos]->path == path && A->tabla[pos]->dom == dom) {
      esta = true;
    }
    pos = abs((indice + indice2*intento)%(A->buckets)); 
  } 
  if (esta)
  {
    A->tabla[pos]->titulo = titulo;
    A->tabla[pos]->tiempo = tiempo;
  } else {
    A->tabla[pos] = new NodoHash;
    A->tabla[pos]->path = path;
    A->tabla[pos]->titulo = titulo;
    A->tabla[pos]->tiempo = tiempo;
    A->tabla[pos]->dom = dom;
    A->cantElem+=1;
  }
}


void rehash(Hash& A){
  if ((float)(A->cantElem % A->buckets) > 0.7)
  {
    int nuevoTam = A->buckets * 2;
    Hash nuevo = crear(nuevoTam);
    for (int i = 0; i < A->buckets; i++)
    {
      if (A->tabla[i] != NULL)
      {
        put(nuevo, A->tabla[i]->dom, A->tabla[i]->path, A->tabla[i]->titulo, A->tabla[i]->tiempo);
        A->tabla[i] = NULL;
      }
    }
    delete A->tabla;
    delete A;
  }
}