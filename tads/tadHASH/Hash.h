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
/*
void putDomPath(Hash B, string dom, string path, string titulo, int tiempo,NodoHashDom* pHashDom){
  string clave = dom + "-" +path;
  int h1 = hash3(dom) % (B->buckets-1); // [0 .. cantBuckets - 1]
  int h2 = 1+(hashSec(dom) % (B->buckets-2)); //[1 .. cantBuckets - 1] Sacado de wikipedia, enlace de las diapos
  if (h1 < 0) h1 += (B->buckets -1);
  if (h2 < 0) h2 += (B->buckets -2);
  int intento = 0;
  int  pos = (h1 + h2*intento)%(B->buckets); 
  while (B->tablaDomPath[pos] != NULL && !(B->tablaDomPath[pos]->dom ==dom && B->tablaDomPath[pos]->path ==path) )
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
    nuevo->nodoD = pHashDom;
    B->tablaDomPath[pos] = nuevo;
    pHashDom->nodoDP = nuevo;
    
    return;
  }
  B->tablaDomPath[pos]->tiempo = tiempo;
  B->tablaDomPath[pos]->titulo = titulo;
  B->tablaDomPath[pos]->nodoD = pHashDom;
  pHashDom->nodoDP = B->tablaDomPath[pos] ;

  
}

void put(Hash& A, Hash B, string dom, string path, string titulo, int tiempo){
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
      NodoHashDom* actual = A->tablaDom[pos];
      NodoHashDom* previo = NULL;
      bool mod = false;
      while(actual!= NULL){
        if (actual->path == path)
        {
          mod = true;
          actual->titulo = titulo;
          if (actual->tiempo != tiempo) {
              actual->tiempo = tiempo;
              NodoHashDom* primero = A->tablaDom[pos];  

              if (actual->prev != NULL) {
                  actual->prev->sig = actual->sig;
              } else {
                  primero = actual->sig;
              }
              if (actual->sig != NULL) {
                  actual->sig->prev = actual->prev;    
              }
              actual->prev = NULL;
              actual->sig  = NULL;

              NodoHashDom* reinsertar = primero;
              NodoHashDom* reinsertarPrev = NULL;

              while (reinsertar != NULL && reinsertar->tiempo <= tiempo) {
                  reinsertarPrev = reinsertar;
                  reinsertar = reinsertar->sig;
              }

              if (reinsertarPrev == NULL) {
                 
                  actual->sig = primero;
                  if (primero != NULL) primero->prev = actual;
                  primero = actual;
              } else {
                  actual->sig = reinsertar;
                  actual->prev = reinsertarPrev;
                  reinsertarPrev->sig = actual;
                  if (reinsertar != NULL) reinsertar->prev = actual;
              }

              A->tablaDom[pos] = primero; 
          }
          putDomPath(B, dom, path, titulo, tiempo, actual);
          break;
        }
        previo = actual;
        actual = actual->sig;
      }
      if(!mod){
        NodoHashDom* nuevo = new NodoHashDom;
        nuevo->dom = dom;
        nuevo->tiempo = tiempo; 
        nuevo->path = path;
        nuevo->titulo = titulo;
        nuevo->sig = NULL;
        nuevo->prev = NULL;
        nuevo->nodoDP = NULL;

        NodoHashDom* primero2 = A->tablaDom[pos];
        NodoHashDom* actual2 = primero2; 
        NodoHashDom* previo2 = NULL;
        while (actual2 != NULL && actual2->tiempo <= tiempo)
        {
          previo2 = actual2;
          actual2 = actual2->sig;
        }
        if (previo2 == NULL)
        {
          nuevo->sig = primero2;
          if (primero2 != NULL)
          {
            primero2->prev = nuevo;
          }
          A->tablaDom[pos] = nuevo;
        }
        else
        {
          nuevo->sig = previo2->sig;
          if (previo2->sig != NULL) 
          {
            previo2->sig->prev = nuevo;
          }
          previo2->sig = nuevo;
          nuevo->prev = previo2;
        }
        // Consulta a IA: “¿Cómo paso a putDomPath el puntero correcto al nodo recién insertado (si quedó como cabeza usar A->tablaDom[pos], si no usar 'nuevo') usando una sola expresión con el operador ternario (?:)?”
        putDomPath(B, dom, path, titulo, tiempo, (previo2 == NULL ? A->tablaDom[pos] : nuevo));
        A->cantElem++;
        return;
      }
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
    nuevo->prev = NULL;
    nuevo->nodoDP = NULL;
    A->tablaDom[pos] = nuevo;
    putDomPath(B, dom, path, titulo, tiempo, nuevo);
    A->cantElem++;
    return;
  }
}
*/
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
  for (int i = 0; i < cant; i++)
  {
    NodoHashDom* actual =A->tablaDom[i];
    while (actual!=NULL)
    {
      NodoHashDom* siguiente = actual->sig;
      delete actual; 
      actual = siguiente;
    }
    A->tablaDom[i] = NULL;
  }
  for (int i = 0; i < cant; i++)
  {
   delete A->tablaDomPath[i];
   A->tablaDomPath[i] = NULL;
  }
  A->cantElem = 0;
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