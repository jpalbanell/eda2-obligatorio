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
    int pos = buscarDomPath(A, dom, path);
    if (pos != -1 && A->tablaDomPath[pos] != NULL) {
        cout << A->tablaDomPath[pos]->titulo << " " << A->tablaDomPath[pos]->tiempo << "\n";
    } else {
        cout << "recurso_no_encontrado\n";
    }
}
void remove(Hash& A, string dom, string path) {
    int b = A->buckets;
    int h1 = mod(hash3(dom), b);
    int h2 = 1 + mod(hashSec(dom), b - 1);
    int pos = (h1 + h2) % b;

    // --- tablaDom ---
    NodoHashDom* actual = A->tablaDom[pos];
    NodoHashDom* prev = NULL;
    bool borrado = false;

    while (actual != NULL) {
        if (actual->dom == dom && actual->path == path) {
            NodoHashDom* aBorrar = actual;
            
            if (prev == NULL) {
                A->tablaDom[pos] = actual->sig;
            } else {
                prev->sig = actual->sig;
            }
            actual = actual->sig;
            delete aBorrar;
            borrado = true;
            break; 
        }
        prev = actual;
        actual = actual->sig;
    }

    NodoHashDomPath* actual2 = A->tablaDomPath[pos];
    NodoHashDomPath* prev2 = NULL;

    while (actual2 != NULL) {
        if (actual2->dom == dom && actual2->path == path) {
            NodoHashDomPath* aBorrar2 = actual2;
            if (prev2 == NULL) {
                A->tablaDomPath[pos] = actual2->sig;
            } else {
                prev2->sig = actual2->sig;
            }
            actual2 = actual2->sig; 
            delete aBorrar2;
            borrado = true;
            break;
        }
        prev2 = actual2;
        actual2 = actual2->sig;
    }

    if (borrado) {
        if (A->cantElem > 0) A->cantElem--;

        NodoHashCantDom* actual3 = A->tablaCantDom[pos];
        NodoHashCantDom* prev3 = NULL;

        while (actual3 != NULL) {
            if (actual3->dom == dom) {
                actual3->cant--;
                if (actual3->cant <= 0) {
                    NodoHashCantDom* aBorrar3 = actual3;
                    if (prev3 == NULL) {
                        A->tablaCantDom[pos] = actual3->sig;
                    } else {
                        prev3->sig = actual3->sig;
                    }
                    actual3 = actual3->sig;
                    delete aBorrar3;
                }
                break;
            }
            prev3 = actual3;
            actual3 = actual3->sig;
        }
    }
}


int count_domain(Hash A, string dom) {
    int b = A->buckets;
    int h1 = mod(hash3(dom), b);
    int h2 = 1 + mod(hashSec(dom), b - 1);

    int intento = 0;
    int pos = (h1 + h2 * intento) % b;

    while (A->tablaDom[pos] != NULL || A->tablaDomSeBorro[pos]) {
        if (A->tablaDom[pos] != NULL && A->tablaDom[pos]->dom == dom) {
            return A->cantDom[pos];
        }
        intento++;
        pos = (h1 + h2 * intento) % b;
    }
    return 0;
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

    int intento = 0;
    int pos = (h1 + h2 * intento) % b;
    NodoHashDom* head = NULL;

    while (A->tablaDom[pos] != NULL || A->tablaDomSeBorro[pos]) {
        if (A->tablaDom[pos] != NULL && A->tablaDom[pos]->dom == dom) {
            head = A->tablaDom[pos];
            break;
        }
        intento++;
        pos = (h1 + h2 * intento) % b;
    }

    bool first = true;
    for (NodoHashDom* p = head; p != NULL; p = p->sig) {
        if (!first) cout << ' ';
        cout << p->path;
        first = false;
    }
    cout << "\n";
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
            // NO movemos prev cuando borramos
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
    }
}

int size(Hash A) { return A->cantElem; }
