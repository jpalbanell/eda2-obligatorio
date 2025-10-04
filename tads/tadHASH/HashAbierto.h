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

void remove(Hash& A, string dom, string path) {
    
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
