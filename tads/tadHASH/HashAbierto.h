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

    // Si ya existe, actualizo y salgo
    NodoHashDomPath* actual = A->tablaDomPath[pos];
    while (actual != NULL) {
        if (actual->dom == dom && actual->path == path) {
            actual->titulo = titulo;
            actual->tiempo = tiempo;
            return;
        }
        actual = actual->sig;
    }

    // Si no existe, inserto primero (sin borrar nada)
    NodoHashDomPath* nuevo = new NodoHashDomPath;
    nuevo->path = path;
    nuevo->dom = dom;
    nuevo->titulo = titulo;
    nuevo->tiempo = tiempo;
    nuevo->sig = A->tablaDomPath[pos];
    A->tablaDomPath[pos] = nuevo;
}

void put(Hash& A, string dom, string path, string titulo, int tiempo) {
    int b = A->buckets;
    int h1 = mod(hash3(dom), b);
    int h2 = 1 + mod(hashSec(dom), b - 1);
    int pos = (h1 + h2) % b;

    NodoHashDom* nuevo = new NodoHashDom;
    nuevo->path = path;
    nuevo->dom = dom;
    nuevo->titulo = titulo;
    nuevo->tiempo = tiempo;
    nuevo->sig = A->tablaDom[pos];
    A->tablaDom[pos] = nuevo;

    bool yaExistia = false;
    NodoHashDom* prev = nuevo;
    NodoHashDom* actual  = nuevo->sig;
    while (actual != NULL) {
        if (actual->dom == dom && actual->path == path) {
            yaExistia = true;
            NodoHashDom* aBorrar = actual;
            prev->sig = actual->sig;
            actual = actual->sig;
            delete aBorrar;
        } else {
            prev = actual;
            actual = actual->sig;
        }
    }

    if (!yaExistia) {
        A->cantElem++;
        NodoHashCantDom* cantDom = A->tablaCantDom[pos];
        bool domEncontrado = false;
        while (cantDom != NULL && !domEncontrado) {
            if (cantDom->dom == dom) {
                cantDom->cant++;
                domEncontrado = true;
            }
            cantDom = cantDom->sig;
        }
        if (!domEncontrado) {
            NodoHashCantDom* nuevoC = new NodoHashCantDom;
            nuevoC->cant = 1;
            nuevoC->dom  = dom;
            nuevoC->sig  = A->tablaCantDom[pos];
            A->tablaCantDom[pos] = nuevoC;
        }
    }

    putDomPath(A, dom, path, titulo, tiempo);
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

void remove(Hash& A, string dom, string path){
    int b = A->buckets;
    int h1 = mod(hash3(dom+path), b);
    int h2 = 1 + mod(hashSec(dom+path), b - 1);
    int pos = (h1 + h2) % b;

    // borrar en tablaDomPath
    NodoHashDomPath* aux = A->tablaDomPath[pos];
    NodoHashDomPath* prev2 = NULL;
    bool seBorro = false;
    while (aux != NULL) {
        if (aux->dom == dom && aux->path == path) {
            if (prev2 == NULL) {
                A->tablaDomPath[pos] = aux->sig;
            } else {
                prev2->sig = aux->sig;
            }
            delete aux;
            seBorro = true;
            break;
        }
        prev2 = aux;
        aux = aux->sig;
    }

    if (seBorro) {
        if (A->cantElem > 0) A->cantElem--;

        // borrar en tablaDom 
        b = A->buckets;
        h1 = mod(hash3(dom), b);
        h2 = 1 + mod(hashSec(dom), b - 1);
        pos = (h1 + h2) % b;

        NodoHashDom* auxDom = A->tablaDom[pos];
        NodoHashDom* prev1 = NULL;
        bool seBorro1 = false;
        while (auxDom != NULL) {
            if (auxDom->dom == dom && auxDom->path == path) {
                if (prev1 == NULL) A->tablaDom[pos] = auxDom->sig;
                else               prev1->sig = auxDom->sig;
                delete auxDom;
                seBorro1 = true;
                break;
            }
            prev1 = auxDom;
            auxDom = auxDom->sig;
        }

        // actualizar contador de dominio 
        NodoHashCantDom* auxCant = A->tablaCantDom[pos];
        NodoHashCantDom* prevCant = NULL;
        while (auxCant != NULL) {
            if (auxCant->dom == dom) {
                auxCant->cant--;
                if (auxCant->cant <= 0) {
                    if (prevCant == NULL) A->tablaCantDom[pos] = auxCant->sig;
                    else                  prevCant->sig = auxCant->sig;
                    delete auxCant;
                }
                break;
            }
            prevCant = auxCant;
            auxCant = auxCant->sig;
        }
    }
}

void count_domain(Hash A, string dom) {
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
    int h1 = mod(hash3(dom+path), b);
    int h2 = 1 + mod(hashSec(dom+path), b - 1);
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
    bool primero = true;  // para no dejar espacio al final

    while (aux != NULL) {
        if (aux->dom == dom) {
            if (!primero) cout << " ";
            cout << aux->path;
            primero = false;
        }
        aux = aux->sig;
    }
    cout << endl;
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
            int h11 = mod(hash3(dom + (actual->path)), b);
            int h22 = 1 + mod(hashSec(dom + (actual->path)), b - 1);
            int pos2 = (h11 + h22) % b;

            NodoHashDomPath* actual2 = A->tablaDomPath[pos2];
            NodoHashDomPath* prev2 = NULL;
            while (actual2 != NULL) {
                if (actual2->dom == dom && actual2->path == actual->path) {
                    if (prev2 == NULL) A->tablaDomPath[pos2] = actual2->sig; // <-- pos2
                    else               prev2->sig = actual2->sig;
                    delete actual2;
                    break;
                }
                prev2 = actual2;
                actual2 = actual2->sig;
            }

            NodoHashDom* aBorrar = actual;
            if (prev == NULL) A->tablaDom[pos] = actual->sig;
            else              prev->sig = actual->sig;
            actual = (prev == NULL) ? A->tablaDom[pos] : prev->sig;
            delete aBorrar;
            eliminados++;
        } else {
            prev = actual;
            actual = actual->sig;
        }
    }

    if (eliminados > 0) {
        A->cantElem -= eliminados;
        if (A->cantElem < 0) A->cantElem = 0;

        NodoHashCantDom* actual3 = A->tablaCantDom[pos];
        NodoHashCantDom* prev3   = NULL;
        while (actual3 != NULL) {
            if (actual3->dom == dom) {
                if (prev3 == NULL) A->tablaCantDom[pos] = actual3->sig;
                else               prev3->sig = actual3->sig;
                delete actual3;
                break;
            }
            prev3 = actual3;
            actual3 = actual3->sig;
        }
    }
}

void clear(Hash& A) {
    for (int i = 0; i < A->buckets; ++i) {
        while (A->tablaDom[i] != NULL) {
            string dom = A->tablaDom[i]->dom;
            clear_domain(A, dom);
        }
    }
    for (int i = 0; i < A->buckets; ++i) {
        NodoHashDomPath* p2 = A->tablaDomPath[i];
        while (p2 != NULL) { NodoHashDomPath* q = p2; p2 = p2->sig; delete q; }
        A->tablaDomPath[i] = NULL;

        NodoHashCantDom* c = A->tablaCantDom[i];
        while (c != NULL) { NodoHashCantDom* q = c; c = c->sig; delete q; }
        A->tablaCantDom[i] = NULL;
    }
    A->cantElem = 0;
}

int size(Hash A) { return A->cantElem; }
