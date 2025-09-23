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
    A->tablaDomPath[pos] = new NodoHashDomPath;
    A->tablaDomPath[pos]-> path = path;
    A->tablaDomPath[pos]-> dom = dom;
    A->tablaDomPath[pos]-> titulo = titulo;
    A->tablaDomPath[pos]-> tiempo = tiempo;
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
    A->tablaDom[pos] = new NodoHashDom;
    A->tablaDom[pos]-> path = path;
    A->tablaDom[pos]-> dom = dom;
    A->tablaDom[pos]-> titulo = titulo;
    A->tablaDom[pos]-> tiempo = tiempo;
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
    bool estaba = removeDomPath(A, dom, path);
    if (!estaba) return;
    A->cantElem -= 1;

    int b = A->buckets;
    int h1 = mod(hash3(dom), b);
    int h2 = 1 + mod(hashSec(dom), b - 1);

    int intento = 0;
    int pos = (h1 + h2 * intento) % b;
    int posDom = -1;

    while (A->tablaDom[pos] != NULL || A->tablaDomSeBorro[pos]) {
        if (A->tablaDom[pos] != NULL && A->tablaDom[pos]->dom == dom) { posDom = pos; break; }
        intento++;
        pos = (h1 + h2 * intento) % b;
    }
    if (posDom == -1) return; // ya no está el dominio

    // quitar el nodo del path en la lista del dominio
    NodoHashDom* cur = A->tablaDom[posDom];
    NodoHashDom* prev = NULL;
    while (cur != NULL && cur->path != path) { 
      prev = cur; 
      cur = cur->sig; }
    if (cur == NULL) return;

    if (prev != NULL) {
      prev->sig = cur->sig;
    }
    else 
    {
      A->tablaDom[posDom] = cur->sig;
    }
    delete cur;

    A->cantDom[posDom] -= 1;
    if (A->tablaDom[posDom] == NULL) {
        A->tablaDomSeBorro[posDom] = true;
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
    int pos = buscarDomPath(A, dom, path);
    return (pos != -1 && A->tablaDomPath[pos] != NULL);
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
    int b = A->buckets;
    int h1 = mod(hash3(dom), b);
    int h2 = 1 + mod(hashSec(dom), b - 1);

    int intento = 0;
    int pos = (h1 + h2 * intento) % b;
    int posDom = -1;

    while (A->tablaDom[pos] != NULL || A->tablaDomSeBorro[pos]) {
        if (A->tablaDom[pos] != NULL && A->tablaDom[pos]->dom == dom) { posDom = pos; break; }
        intento++;
        pos = (h1 + h2 * intento) % b;
    }
    if (posDom == -1 || A->tablaDom[posDom] == NULL) return;

    // borra toda la lista y entradas (dom+path)
    NodoHashDom* p = A->tablaDom[posDom];
    while (p != NULL) {
        removeDomPath(A, dom, p->path);
        A->cantElem -= 1;
        NodoHashDom* nxt = p->sig;
        delete p;
        p = nxt;
    }
    A->tablaDom[posDom] = NULL;
    A->cantDom[posDom] = 0;
    A->tablaDomSeBorro[posDom] = true;
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
