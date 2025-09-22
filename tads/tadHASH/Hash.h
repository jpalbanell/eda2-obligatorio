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
};

struct NodoHashDomPath
{
    string path;
    string dom;
    string titulo;
    int tiempo;
};

struct RepresentacionHash
{
    NodoHashDom** tablaDom;
    NodoHashDomPath** tablaDomPath;
    int* cantDom; // pos es con dom+path
    bool* tablaDomSeBorro;
    bool* tablaDomPathSeBorro;
    int cantElem;
    int buckets;
};
typedef RepresentacionHash *Hash; 

Hash crear(int esperados){
    esperados = esperados * 2;
    Hash nuevo = new RepresentacionHash;
    nuevo -> cantElem = 0;
    nuevo -> buckets = esperados;
    nuevo -> tablaDom = new NodoHashDom*[esperados];
    nuevo -> tablaDomPath = new NodoHashDomPath*[esperados];
    nuevo -> tablaDomPathSeBorro = new bool[esperados];
    nuevo -> tablaDomSeBorro = new bool[esperados];
    nuevo -> cantDom = new int[esperados];
    for (int i = 0; i < esperados; i++)
    {
      nuevo -> tablaDom[i] = NULL;
      nuevo -> tablaDomPath[i] = NULL;
      nuevo -> cantDom[i] = 0;
      nuevo -> tablaDomPathSeBorro [i] = false;
      nuevo -> tablaDomSeBorro [i] = false;
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

void putDomPath(Hash B, string dom, string path, string titulo, int tiempo){
  int h1 = hash3(dom+path) % (B->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom+path) % (B->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (B->buckets -1);
  if (h2 < 0) h2 += (B->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(B->buckets); 
  while (B->tablaDomPathSeBorro[pos] || (B->tablaDomPath[pos] != NULL && !(B->tablaDomPath[pos]->dom ==dom && B->tablaDomPath[pos]->path ==path)) )
  {
    intento++;
    pos = (h1 + h2*intento)%(B->buckets); 
    if (pos<= -1)
    {
      pos = pos * (-1);
    }
  }
  if (B->tablaDomPath[pos] == NULL)
  {
    NodoHashDomPath* nuevo = new NodoHashDomPath;
    nuevo->dom = dom;
    nuevo->path = path;
    nuevo->tiempo = tiempo;
    nuevo->titulo = titulo;
    B->tablaDomPath[pos] = nuevo; 
    B->tablaDomPathSeBorro[pos] = false; 
    return;
  }
  B->tablaDomPath[pos]->tiempo = tiempo;
  B->tablaDomPath[pos]->titulo = titulo; 
}

void put(Hash& A, string dom, string path, string titulo, int tiempo){
  int h1 = hash3(dom) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  while (A->tablaDomSeBorro[pos] || (A->tablaDom[pos] != NULL && !esta))
  {
    intento++;
    if (A->tablaDom[pos] != NULL && A->tablaDom[pos]->dom == dom ) {
      esta = true;
      //agregas path al principio de la lista
      NodoHashDom* nuevo = new NodoHashDom;
      nuevo->dom = dom;
      nuevo->path = path;
      nuevo->titulo = titulo;
      nuevo->tiempo = tiempo;
      nuevo->sig = A->tablaDom[pos];
      A->tablaDom[pos] = nuevo;
      // ver si path ya existia
      NodoHashDom* actual = A->tablaDom[pos]->sig;
      NodoHashDom* previo = A->tablaDom[pos];
      bool pathE = false;
      while(actual!= NULL && !pathE){
        if (actual->path == path)
        {
          pathE = true;
          previo->sig = actual->sig; 
          delete actual;
        }
        previo= actual;
        actual = actual->sig;
      }
      if (!pathE)
      {
        A->cantDom[pos] += 1;
      }
      putDomPath(A, dom, path, titulo, tiempo);
      //break;
    }
    pos = (h1 + h2*intento)%(A->buckets); 
    if (pos<= -1)
    {
      pos = pos * (-1);
    }
  }
  
  if (!esta){
    NodoHashDom* nuevo = new NodoHashDom;
    nuevo->dom = dom;
    nuevo->path = path;
    nuevo->titulo = titulo;
    nuevo->tiempo = tiempo;
    nuevo->sig = NULL;
    A->tablaDom[pos] = nuevo;
    A->tablaDomSeBorro[pos] = false; 
    A->cantDom[pos] += 1;
    putDomPath(A, dom, path, titulo, tiempo);
    A->cantElem++;
  }
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
  while (A->tablaDomPathSeBorro[pos] || (A->tablaDomPath[pos] != NULL && !esta))
  {
    intento++;
    if (A->tablaDomPath[pos] != NULL && A->tablaDomPath[pos]->dom == dom && A->tablaDomPath[pos]->path == path)
    {
      esta = true;
      delete A->tablaDomPath[pos];
      A->tablaDomPath[pos] = NULL;
      A->tablaDomPathSeBorro[pos] = true;
    }
    pos = (h1 + h2*intento)%(A->buckets); 
    if (pos<= -1)
    {
      pos = pos * (-1);
    }
  }
  if (esta)
  {
    A->cantElem--;
    h1 = hash3(dom) % (A->buckets-1); // [0 .. cantBuckets - 1]
    h2 = 1+(hashSec(dom) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
    if (h1 < 0) h1 += (A->buckets -1);
    if (h2 < 0) h2 += (A->buckets -2);
    intento = 0;
    pos = (h1 + h2*intento)%(A->buckets); 
    esta = false;
    while (A->tablaDomSeBorro[pos] || (A->tablaDom != NULL && !esta))
    {
      intento++;
      if (A->tablaDom[pos] != NULL && A->tablaDom[pos]->dom == dom)
      {
        esta = true;
        NodoHashDom* aux = A -> tablaDom[pos];
        while (aux->path != path && aux != NULL)
        {
          aux = aux -> sig;
        }
        if (aux != NULL)
        {
          NodoHashDom* aBorrar = aux;
          aux = aux -> sig;
          delete aBorrar;
          A->cantDom[pos] = A->cantDom[pos] - 1;
        }
        A->tablaDomSeBorro[pos] = true;
      }
      pos = (h1 + h2*intento)%(A->buckets); 
      if (pos<= -1)
      {
        pos = pos * (-1);
      }
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
  while (A->tablaDomSeBorro[pos] || (A->tablaDom[pos] != NULL && !esta))
  {
    intento++;
    if (A->tablaDom[pos] != NULL && A->tablaDom[pos]->dom == dom ) {
      esta = true;
      while(A->tablaDom[pos] != NULL){
        int h11 = hash3(dom+(A->tablaDom[pos]->path)) % (A->buckets-1); // [0 .. cantBuckets - 1]
        int h22 = 1+(hashSec(dom+(A->tablaDom[pos]->path)) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
        if (h11 < 0) h1 += (A->buckets -1);
        if (h22 < 0) h2 += (A->buckets -2);
        int intento2 = 0;
        int  pos2 = (h1 + h2*intento2)%(A->buckets); 
        bool esta2 = false;
        while (A->tablaDomPathSeBorro[pos2] || (A->tablaDomPath[pos2] != NULL && !esta2))
        {
          intento2++;
          if (A->tablaDomPath[pos2] != NULL && A->tablaDomPath[pos2]->dom == dom && A->tablaDomPath[pos2]->path == (A->tablaDom[pos]->path))
          {
            esta2 = true;
            delete A->tablaDomPath[pos2];
            A->tablaDomPath[pos2] = NULL;
            A->cantDom[pos] = A->cantDom[pos] - 1;
          }
          pos2 = (h11 + h22*intento2)%(A->buckets); 
          if (pos2<= -1)
          {
            pos2 = pos2 * (-1);
          }
        }
        A->tablaDomPathSeBorro[pos2] = true;
        NodoHashDom* aBorrar = A->tablaDom[pos];
        A->tablaDom[pos] = A->tablaDom[pos] -> sig;
        delete aBorrar;
        A->cantElem--;
      }
      A->tablaDomSeBorro[pos] = true;
    }
    pos = (h1 + h2*intento) % (A->buckets); 
    if (pos <= -1)
    {
      pos = pos * (-1);
    }
  }
}

void clear(Hash& A){
  for (int i = 0; i < A->buckets; i++)
  {
    if (A->tablaDom[i] != NULL)
    {
      string domBorrar = A->tablaDom[i]->dom;
      clear_domain(A, domBorrar);
    }
  }
  
}

int count_domain(Hash A, string dom){
  int h1 = hash3(dom) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  while (A->tablaDomSeBorro[pos] || (A->tablaDom[pos] != NULL && !esta))
  {
    intento++;
    if (A->tablaDom[pos] != NULL && A->tablaDom[pos]->dom == dom ) {
      return A->cantDom[pos];
    }
    pos = (h1 + h2*intento) % (A->buckets); 
    if (pos <= -1)
    {
      pos = pos * (-1);
    }
  }
  return 0;
}

bool contains(Hash A, string dom, string path){
  int h1 = hash3(dom+path) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom+path) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  while (A->tablaDomPathSeBorro[pos] || (A->tablaDomPath[pos] != NULL && !esta))
  {
    intento++;
    if (A->tablaDomPath[pos] != NULL && A->tablaDomPath[pos]->dom == dom && A->tablaDomPath[pos]->path == path ) {
      esta = true;
    }
    pos = (h1 + h2*intento) % (A->buckets); 
    if (pos <= -1)
    {
      pos = pos * (-1);
    }
  }
  return esta;
  
}

void list_domain(Hash A, string dom){
  int h1 = hash3(dom) % (A->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom) % (A->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (A->buckets -1);
  if (h2 < 0) h2 += (A->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(A->buckets); 
  bool esta = false;
  while (A->tablaDomSeBorro[pos] || (A->tablaDom[pos] != NULL && !esta))
  {
    intento++;
    if (A->tablaDom[pos] != NULL && A->tablaDom[pos]->dom == dom ) {
      esta = true;
      NodoHashDom* aux = A->tablaDom[pos];
      while(aux != NULL){
        cout << aux->path << endl;
        aux = aux -> sig;
      }
    }
    pos = (h1 + h2*intento) % (A->buckets); 
    if (pos <= -1)
    {
      pos = pos * (-1);
    }
  }
  if (!esta)
  {
    cout << " " << endl;
  }
  
}

int size(Hash A){
  return A->cantElem;
}