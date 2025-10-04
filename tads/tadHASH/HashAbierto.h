#include <cassert>
#include <limits>
#include <string>
#include <iostream>
using namespace std;

struct NodoHashDomPath;

struct NodoHashDom {
    string path;
    string dom;
    string titulo;
    int tiempo;
    NodoHashDom* sig;
};

struct NodoHashDomPath {
    string path;
    string dom;
    string titulo;
    int tiempo;
    NodoHashDomPath* sig;
};

struct NodoHashCantDom{
    int cant;
    string dom;
    NodoHashCantDom* sig;
};

struct RepresentacionHash {
    NodoHashDom** tablaDom; 
    NodoHashDomPath** tablaDomPath;
    int cantElem;  
    int buckets; 
    NodoHashCantDom** tablaCantDom;  
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

    Hash A = new RepresentacionHash;
    A->buckets = B;
    A->cantElem = 0;

    A->tablaDom = new NodoHashDom*[B];

    A->tablaDomPath = new NodoHashDomPath*[B];
    A->tablaCantDom = new NodoHashCantDom*[B];

    for (int i = 0; i < B; ++i) {
        A->tablaDom[i] = NULL;
        A->tablaDomPath[i] = NULL;
        A->tablaCantDom[i] = NULL;
    }
    return A;
}

void putDomPath(Hash A, string& dom, string& path, string& titulo, int tiempo) {
    int b = A->buckets;
    int h1 = mod(hash3(dom+path), b);
    int h2 = 1 + mod(hashSec(dom+path), b - 1);
    int pos = (h1 + h2) % b;
    NodoHashDomPath* nuevo = new NodoHashDomPath;
    nuevo-> path = path;
    nuevo-> dom = dom;
    nuevo-> titulo = titulo;
    nuevo-> tiempo = tiempo;
    nuevo-> sig = A->tablaDomPath[pos];
    A->tablaDomPath[pos] = nuevo;
    NodoHashDomPath* actual = A->tablaDomPath[pos] ->sig;
    while (actual!=NULL)
    {
        if (actual->dom == dom && actual->path == path)
        {
            NodoHashDomPath* aBorrar = actual;
            actual = actual->sig;
            delete aBorrar;
        }
        actual = actual->sig; 
    }
}

void put(Hash& A, string dom, string path, string titulo, int tiempo) {
    int b = A->buckets;
    int h1 = mod(hash3(dom), b);
    int h2 = 1 + mod(hashSec(dom), b - 1);
    int pos = (h1 + h2) % b;
    NodoHashDom* nuevo = new NodoHashDom;
    nuevo-> path = path;
    nuevo-> dom = dom;
    nuevo-> titulo = titulo;
    nuevo-> tiempo = tiempo;
    nuevo-> sig = A->tablaDom[pos];
    A->tablaDom[pos] = nuevo;
    NodoHashDom* actual = A->tablaDom[pos]->sig;
    bool actualizado = false;
    while (actual!=NULL)
    {
        if (actual->dom == dom && actual->path == path)
        {
            actualizado = true;
            NodoHashDom* aBorrar = actual;
            actual = actual->sig;
            delete aBorrar;
        }
        actual = actual->sig; 
    }  
    if (!actualizado)
    {
        A->cantElem++;
        NodoHashCantDom* cantDom = A->tablaCantDom[pos];
        bool domEncontrado = false;
        while (cantDom!=NULL)
        {
            if (cantDom->dom == dom) {  
                cantDom->cant ++;
                domEncontrado = true;
            } 
            cantDom = cantDom->sig;
        }
        if (!domEncontrado)
        {
            NodoHashCantDom* nuevo = new NodoHashCantDom;
            nuevo->cant = 1;
            nuevo->dom = dom;
            nuevo->sig = A->tablaCantDom[pos];
            A->tablaCantDom[pos] = nuevo;
        }
 
    }
    putDomPath(A, dom,path, titulo, tiempo);    
}

void get(Hash A, string dom, string path) {
    int b = A->buckets;
    int h1 = mod(hash3(dom+path), b);
    int h2 = 1 + mod(hashSec(dom+path), b - 1);
    int pos = (h1 + h2) % b;
    NodoHashDomPath* aux = A->tablaDomPath[pos];
    while (aux != NULL)
    {
        if (aux->dom == dom && aux->path == path)
        {
            cout << aux -> titulo << " " << aux -> tiempo << endl;
            return;
        }
        else
        {
            aux = aux -> sig;
        }

    }
    cout << "recurso_no_encontrado" << endl;

}


int count_domain(Hash A, string dom) {
    int b = A->buckets;
    int h1 = mod(hash3(dom), b);
    int h2 = 1 + mod(hashSec(dom), b - 1);
    int pos = (h1 + h2) % b;
    NodoHashCantDom* aux = A->tablaCantDom[pos];
    while (aux != NULL)
    {
        if (aux->dom == dom)
        {
            cout << aux ->cant << endl;
            return;
        }
        else
        {
            aux = aux -> sig;
        }
        
    }
    cout << 0 << endl;
}

bool contains(Hash A, string dom, string path) {
    int b = A->buckets;
    int h1 = mod(hash3(dom), b);
    int h2 = 1 + mod(hashSec(dom), b - 1);
    int pos = (h1 + h2) % b;
    NodoHashDomPath* actual = A->tablaDomPath[pos];
    while (actual!=NULL)
    {
        if (actual->dom == dom && actual->path == path) return true;
        actual = actual->sig;
    }
    return false;
}

void list_domain(Hash A, string dom) {
    int b = A->buckets;
    int h1 = mod(hash3(dom), b);
    int h2 = 1 + mod(hashSec(dom), b - 1);
    int pos = (h1 + h2) % b;
    NodoHashDom* aux = A->tablaDom[pos];
    bool tienePath = false;

    while (aux != NULL) {
        if (aux->dom == dom) {
            tienePath = true;
            cout << aux -> path << " ";
        }
        aux = aux ->sig;
    }

    if(!tienePath) cout << " " << endl;
}

void clear_domain(Hash& A, string dom) {
    int b  = A->buckets;
    int h1 = mod(hash3(dom), b);
    int h2 = 1 + mod(hashSec(dom), b - 1);
    int pos = (h1 + h2) % b;

    int eliminados = 0;

   
    NodoHashDom* actual = A->tablaDom[pos];
    NodoHashDom* prev   = NULL;

    while (actual != NULL) {
        if (actual->dom == dom) {
            NodoHashDom* aBorrar = actual;
            if (prev == NULL) {
                A->tablaDom[pos] = actual->sig;   
            } else {
                prev->sig = actual->sig;          
            }
            actual = actual->sig;                 
            delete aBorrar;
            eliminados++;
            
        } else {
            prev = actual;
            actual = actual->sig;
        }
    }

    
    NodoHashDomPath* actual2 = A->tablaDomPath[pos];
    NodoHashDomPath* prev2   = NULL;

    while (actual2 != NULL) {
        if (actual2->dom == dom) {
            NodoHashDomPath* aBorrar2 = actual2;
            if (prev2 == NULL) {
                A->tablaDomPath[pos] = actual2->sig;
            } else {
                prev2->sig = actual2->sig;
            }
            actual2 = actual2->sig;
            delete aBorrar2;
            
        } else {
            prev2 = actual2;
            actual2 = actual2->sig;
        }
    }

    
    if (eliminados > 0) {
        
        A->cantElem -= eliminados;
        if (A->cantElem < 0) A->cantElem = 0; 

        
        NodoHashCantDom* actual3 = A->tablaCantDom[pos];
        NodoHashCantDom* prev3   = NULL;

        while (actual3 != NULL) {
            if (actual3->dom == dom) {
                NodoHashCantDom* aBorrar3 = actual3;
                if (prev3 == NULL) {
                    A->tablaCantDom[pos] = actual3->sig;
                } else {
                    prev3->sig = actual3->sig;
                }
                actual3 = actual3->sig;
                delete aBorrar3;
                break;
            } else {
                prev3 = actual3;
                actual3 = actual3->sig;
            }
        }
    }
}


void clear(Hash& A) {
    for (int i = 0; i < A->buckets; ++i) {
        if (A->tablaDom[i] != NULL) {
            string dom = A->tablaDom[i]->dom;
            clear_domain(A, dom);
        }
        A->tablaCantDom[i] = 0;
    }
}

int size(Hash A) { return A->cantElem; }
