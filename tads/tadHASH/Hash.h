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
    NodoHash* sig;
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
    nuevo -> tabla = new NodoHash*[esperados];
    for (int i = 0; i < esperados; i++)
    {
      nuevo -> tabla[i] = NULL;
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

void put(Hash& A, string dom, string path, string titulo, int tiempo){
  int h1 = hash3(dom) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  
  while (A->tabla[pos] != NULL && !esta)
  {
  
    intento++;
    if (A->tabla[pos]->dom == dom ) {
      esta = true;
      NodoHash* actual = A->tabla[pos];
      bool mod = false;
      while(actual!= NULL && !mod){
        if (actual->path == path)
        {
          actual->titulo = titulo;
          actual->tiempo = tiempo;
          mod = true;
        }
        actual = actual->sig;
      }
      if(!mod){
        delete actual;
        NodoHash* nuevo = new NodoHash;
        nuevo->dom = dom;
        nuevo->path = path;
        nuevo->tiempo = tiempo;
        nuevo->titulo = titulo;
        nuevo->sig = A->tabla[pos];
         A->tabla[pos] = nuevo;
      }

    }
    pos = (h1 + h2*intento)%(A->buckets); 
    if (pos<= -1)
    {
      pos = pos * (-1);
    }
  }
  if (!esta){
    
    A->tabla[pos] = new NodoHash;
    A->tabla[pos]->titulo = titulo;
    A->tabla[pos]->tiempo = tiempo;
    A->cantElem+=1;
    A->tabla[pos]->path = path;
    A->tabla[pos]->dom = dom;
  }


}
void get(Hash A, string dom, string path){
  int h1 = hash3(dom) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  while (A->tabla[pos] != NULL && !esta)
  {

    intento++;
    if (A->tabla[pos]->dom == dom ) {
      esta = true;
      NodoHash* actual = A->tabla[pos];
      bool mod = false;
      while(actual!= NULL && !mod){
        if (actual->path == path)
        {
          cout << actual->titulo <<" "<<actual->tiempo <<endl;
          mod = true;
          break;
          
        }
        actual = actual->sig;
      }
      if(!mod){
        cout << "recurso_no_encontrado" <<endl;
        break;
      }

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

void remove(Hash A, string dom, string path){




}