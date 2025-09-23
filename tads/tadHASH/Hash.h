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
};

struct RepresentacionHash {
    NodoHashDom** tablaDom;
    bool* tablaDomSeBorro; 
    int*  cantDom;
    
    NodoHashDomPath** tablaDomPath;
    bool* tablaDomPathSeBorro; 

    int cantElem;  
    int buckets;   
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
    if ((B & 1) == 0) B++; 

    Hash A = new RepresentacionHash;
    A->buckets = B;
    A->cantElem = 0;

    A->tablaDom = new NodoHashDom*[B];
    A->tablaDomSeBorro = new bool[B];
    A->cantDom = new int[B];

    A->tablaDomPath = new NodoHashDomPath*[B];
    A->tablaDomPathSeBorro = new bool[B];

    for (int i = 0; i < B; ++i) {
        A->tablaDom[i] = NULL;
        A->tablaDomSeBorro[i] = false;
        A->cantDom[i] = 0;

        A->tablaDomPath[i] = NULL;
        A->tablaDomPathSeBorro[i] = false;
    }
    return A;
}

// Busca (dom,path)
// Recorre mientras HAY algo o HAY true en seBorro
static int buscarDomPath(Hash A, string& dom, string& path) {
    string key = dom + path;
    int b = A->buckets;
    int h1 = mod(hash3(key), b);
    int h2 = 1 + mod(hashSec(key), b - 1);

    int intento = 0;
    int pos = (h1 + h2 * intento) % b;

    while (A->tablaDomPath[pos] != NULL || A->tablaDomPathSeBorro[pos]) {
        if (A->tablaDomPath[pos] != NULL &&
            A->tablaDomPath[pos]->dom == dom &&
            A->tablaDomPath[pos]->path == path) {
            return pos;
        }
        intento++;
        pos = (h1 + h2 * intento) % b;
    }
    return -1; // no está
}

// Devuelve posición libre para insertar
static int posicionInsercionDomPath(Hash A, string& dom, string& path) {
    string key = dom + path;
    int b = A->buckets;
    int h1 = mod(hash3(key), b);
    int h2 = 1 + mod(hashSec(key), b - 1);

    int intento = 0;
    int pos = (h1 + h2 * intento) % b;
    while (A->tablaDomPath[pos] != NULL || A->tablaDomPathSeBorro[pos]) {
        if (A->tablaDomPathSeBorro[pos] && A->tablaDomPath[pos] == NULL) {
            return pos; 
        }
        intento++;
        pos = (h1 + h2 * intento) % b;
    }
    return pos;
}

static void putDomPath(Hash A, string& dom, string& path, string& titulo, int tiempo) {
    int pos = buscarDomPath(A, dom, path);
    if (pos != -1) {
        // actualizar
        A->tablaDomPath[pos]->titulo = titulo;
        A->tablaDomPath[pos]->tiempo = tiempo;
        return;
    }
    // insertar
    pos = posicionInsercionDomPath(A, dom, path);
    if (pos >= 0) {
        A->tablaDomPath[pos] = new NodoHashDomPath;
        A->tablaDomPath[pos]-> path = path;
        A->tablaDomPath[pos]-> dom = dom;
        A->tablaDomPath[pos]-> titulo = titulo;
        A->tablaDomPath[pos]-> tiempo = tiempo;
        
        A->tablaDomPathSeBorro[pos] = false;
    }
}

static bool removeDomPath(Hash A, string& dom, string& path) {
    int pos = buscarDomPath(A, dom, path);
    if (pos == -1) return false;
    delete A->tablaDomPath[pos];
    A->tablaDomPath[pos] = NULL;
    A->tablaDomPathSeBorro[pos] = true;
    return true;
}

void put(Hash& A, string dom, string path, string titulo, int tiempo) {
    int b = A->buckets;
    int h1 = mod(hash3(dom), b);
    int h2 = 1 + mod(hashSec(dom), b - 1);

    int intento = 0;
    int pos = (h1 + h2 * intento) % b;
    int posDom = -1;

    // buscar dominio o primer hueco
    while (A->tablaDom[pos] != NULL || A->tablaDomSeBorro[pos]) {
        if (A->tablaDom[pos] != NULL && A->tablaDom[pos]->dom == dom) {
            posDom = pos; break;
        }
        intento++;
        pos = (h1 + h2 * intento) % b;
    }
    if (posDom == -1) {
        // dominio no estaba, insertar bucket del dominio 
        posDom = pos;
        A->tablaDom[posDom] = new NodoHashDom;
        A->tablaDom[posDom]-> path = path;
        A->tablaDom[posDom]-> dom = dom;
        A->tablaDom[posDom]-> titulo = titulo;
        A->tablaDom[posDom]-> tiempo = tiempo;
        A->tablaDom[posDom]-> sig = NULL;

        A->tablaDomSeBorro[posDom] = false;
        A->cantDom[posDom] = 1;
        A->cantElem += 1;
        putDomPath(A, dom, path, titulo, tiempo);
        return;
    }

    NodoHashDom* nuevo = new NodoHashDom;
    nuevo-> path = path;
    nuevo-> dom = dom;
    nuevo-> titulo = titulo;
    nuevo-> tiempo = tiempo;
    nuevo-> sig = A->tablaDom[posDom];

    A->tablaDom[posDom] = nuevo;

    // si ya existía ese path quita el viejo de la lista
    bool existia = false;
    NodoHashDom* prev = nuevo;
    NodoHashDom* cur  = nuevo->sig;
    while (cur != NULL) {
        if (cur->path == path) {
            prev->sig = cur->sig;
            delete cur;
            existia = true;
            break;
        }
        prev = cur; cur = cur->sig;
    }

    putDomPath(A, dom, path, titulo, tiempo);
    if (!existia) { A->cantDom[posDom] += 1; A->cantElem += 1; }
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
