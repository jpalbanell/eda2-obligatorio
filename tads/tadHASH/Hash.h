#include <string>
#include <cassert>
#include <iostream>
#include <limits>

using namespace std; 

struct NodoHashDomPath;

struct NodoHashDom
{
    string path;
    string dom;
    string titulo;
    int tiempo;
    NodoHashDom* sig;
    NodoHashDom* prev;
    NodoHashDomPath* nodoDP;
};

struct NodoHashDomPath
{
    string path;
    string dom;
    string titulo;
    int tiempo;
    NodoHashDom* nodoD;
};

struct RepresentacionHash
{
    NodoHashDom** tablaDom;
    NodoHashDomPath** tablaDomPath;
    int cantElem;
    int buckets;
};
typedef RepresentacionHash *Hash; 

Hash crear(int esperados){
    Hash nuevo = new RepresentacionHash;
    nuevo -> cantElem = 0;
    nuevo -> buckets = esperados;
    nuevo -> tablaDom = new NodoHashDom*[esperados];
    nuevo -> tablaDomPath = new NodoHashDomPath*[esperados];
    for (int i = 0; i < esperados; i++)
    {
      nuevo -> tablaDom[i] = NULL;
      nuevo -> tablaDomPath[i] = NULL;
    }
    
    return nuevo;
}

// Ref: https://cseweb.ucsd.edu/~kube/cls/100/Lectures/lec16/lec16-15.html
// diapositivas de la clase
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



void get(Hash A, string dom, string path){
  int h1 = hash3(dom+path) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom+path) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  while (A->tablaDomPath[pos] != NULL && !esta)
  {
    intento++;
    if (A->tablaDomPath[pos]->dom == dom && A->tablaDomPath[pos]->path == path)
    {
      cout << A->tablaDomPath[pos]->titulo << " " << A->tablaDomPath[pos]->tiempo << endl;
      esta = true;
    }
    
    pos = (h1 + h2*intento)%(A->buckets); 
    if (pos<= -1)
    {
      pos = pos * (-1);
    }
  }
  if (!esta){
    cout << "recurso_no_encontrado" <<endl;
  }
}

void remove(Hash& A, string dom, string path){
  int h1 = hash3(dom+path) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom+path) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  while (A->tablaDomPath[pos] != NULL && !esta)
  {
    intento++;
    if (A->tablaDomPath[pos]->dom == dom && A->tablaDomPath[pos]->path == path)
    {
      esta = true;
      NodoHashDom* aBorrar = A->tablaDomPath[pos]->nodoD;
      aBorrar->prev->sig = aBorrar->sig;
      aBorrar->sig->prev = aBorrar->prev;
      delete aBorrar;
      delete A->tablaDomPath[pos];
      A->tablaDomPath[pos] = NULL;
      A->cantElem--;
    }
    pos = (h1 + h2*intento)%(A->buckets); 
    if (pos<= -1)
    {
      pos = pos * (-1);
    }
  }
}

void clear_domain(Hash& A, string dom){
  int h1 = hash3(dom) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  while (A->tablaDom[pos] != NULL && !esta)
  {
    intento++;
    if (A->tablaDom[pos]->dom == dom ) {
      esta = true;
      while(A->tablaDom[pos]->sig != NULL){
        delete A->tablaDom[pos]->nodoDP;
        NodoHashDom* aBorrar = A->tablaDom[pos];
        A->tablaDom[pos]->prev = NULL;
        A->tablaDom[pos] = A->tablaDom[pos] -> sig;
        delete aBorrar;
        A->cantElem--;
      }
      delete A->tablaDom[pos]->nodoDP;
      delete A->tablaDom[pos];
      A->tablaDom[pos] = NULL;
      A->cantElem--;
    }
    pos = (h1 + h2*intento) % (A->buckets); 
    if (pos <= -1)
    {
      pos = pos * (-1);
    }
  }
}

void clear(Hash& A){
  int cant = A -> buckets;
  delete A;
  A = crear(cant);
}

void count_domain(Hash A, string dom){
  int h1 = hash3(dom) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  int cant = 0;
  while (A->tablaDom[pos] != NULL && !esta)
  {
    intento++;
    if (A->tablaDom[pos]->dom == dom ) {
      esta = true;
      NodoHashDom* aux = A->tablaDom[pos];
      while(aux != NULL){
        cant++;
        aux = aux -> sig;
      }
      delete aux;
    }
    pos = (h1 + h2*intento) % (A->buckets); 
    if (pos <= -1)
    {
      pos = pos * (-1);
    }
  }
  cout << cant << endl;
}

void contains(Hash A, string dom, string path){
  int h1 = hash3(dom+path) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom+path) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  int cant = 0;
  while (A->tablaDomPath[pos] != NULL && !esta)
  {
    intento++;
    if (A->tablaDomPath[pos]->dom == dom ) {
      esta = true;
    }
    pos = (h1 + h2*intento) % (A->buckets); 
    if (pos <= -1)
    {
      pos = pos * (-1);
    }
  }
  if (esta)
  {
    cout << "true" << endl;
  } else {
    cout << "false" << endl;
  }
  
}

void list_domain(Hash A, string dom){
  int h1 = hash3(dom) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  while (A->tablaDom[pos] != NULL && !esta)
  {
    intento++;
    if (A->tablaDom[pos]->dom == dom ) {
      esta = true;
      NodoHashDom* aux = A->tablaDom[pos];
      while(aux != NULL){
        cout << aux->path << endl;
        aux = aux -> sig;
      }
      delete aux;
    }
    pos = (h1 + h2*intento) % (A->buckets); 
    if (pos <= -1)
    {
      pos = pos * (-1);
    }
  }
}

void size(Hash A){
  cout << A->cantElem << endl;
}