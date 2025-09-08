#include <string>
#include <cassert>
#include <iostream>
#include <limits>

using namespace std;

struct NodoAVL
{
    NodoAVL* izq;
    NodoAVL* der;
    int id;
    int pts;
    string nombre;
    int altura;
};

struct NodoAVLpts
{
    NodoAVLpts* izq;
    NodoAVLpts* der;
    int cant;
    int pts;
    int cantMayorPts;
    int altura;
};

struct RepresentacionAVL
{
  NodoAVL *raizID;
  NodoAVLpts *raizPTS;
  int cantReg;
  string nomMejorJug;
  int ptsMejorJug;
  int idMejorJug;
};
typedef RepresentacionAVL *AVL;

AVL crearAVL()
{
    AVL nuevo = new RepresentacionAVL;
    nuevo->raizID = NULL;
    nuevo->raizPTS = NULL;
    nuevo->cantReg = 0;
    nuevo->nomMejorJug = "sin_jugadores";
    nuevo->ptsMejorJug = 0;
    return nuevo;
}

//FUNCIONES PARA AVL
int alturaSegura(NodoAVL* a){
    if (a == NULL)return 0;
    return a->altura; 
}

int getBalance(NodoAVL* a){
    if (a == NULL)return 0;
    return alturaSegura(a->der) - alturaSegura(a->izq); 
}

void actualizarAltura(NodoAVL*& a){
    if(a != NULL){
        a->altura = 1 + max(alturaSegura(a->der),alturaSegura(a->izq));
    }
}

void rotacionIzquierda(NodoAVL*& A){
    //NodoAVL* T1 = A -> izq;
    NodoAVL* B = A -> der;
    NodoAVL* T2 = B -> izq;
    A -> der = T2;
    B -> izq = A;
    actualizarAltura(A);
    actualizarAltura(B);
    A = B;
}

void rotacionDerecha(NodoAVL*& B){
    NodoAVL* A = B -> izq;
    NodoAVL* T2 = A -> der;
    B -> izq = T2;
    A -> der = B;
    actualizarAltura(A);
    actualizarAltura(B);
    B = A;
}

void addRECid(NodoAVL*& a, string nombre, int id, int pts, bool& seAgrego){
    if (a == NULL)
    {
        a = new NodoAVL;
        a -> nombre = nombre;
        a -> id = id;
        a -> pts = pts;
        a -> altura = 1;
        a->der = NULL;
        a->izq = NULL;
        seAgrego = true;
    } else if(a->id < id)
    {
        addRECid(a->der, nombre, id, pts, seAgrego);
    } else if(a->id > id)
    {
        addRECid(a->izq, nombre, id, pts, seAgrego);
    } else if (a->id == id)
    {
        return;
    }

    int balance = getBalance(a);

    if (balance < -1 && a->izq->id > id) 
    {
       //EXPLICACION SITUACION
       /* 
        a   =>          •
                       /
        a-izq =>      •
                     /
        jugNuevo => •
        */
       rotacionDerecha(a);
    }else if (balance < -1 && a->izq->id < id) {
        //EXPLICACION SITUACION
        /* 
        a   =>          •
                       /
        a-izq =>      •
                       \
        jugNuevo =>     •
        */
       rotacionIzquierda(a->izq);
        //EXPLICACION SITUACION
        /* 
        a   =>          •
                       /
        jugNuevo =>   •
                     /
        a->izq =>   •
        */
       rotacionDerecha(a);
    }else if (balance > 1 && a->der->id < id) {
        //EXPLICACION SITUACION
        /* 
        a   =>    •
                   \
        a-der =>    •
                     \
        jugNuevo =>   •
        */
       rotacionIzquierda(a);
    }else if (balance > 1 && a->der->id > id) {
        //EXPLICACION SITUACION
        /* 
        a   =>         •
                        \
        a-der =>         •
                        /
        jugNuevo =>    •
        */
       rotacionDerecha(a->der);
        //EXPLICACION SITUACION
        /* 
        a   =>       •
                      \
        jugNuevo =>    •
                        \
        a->der =>        •
        */
       rotacionIzquierda(a);
    }
    
    actualizarAltura(a);
    
}

//FUNCIONES PARA AVLpts
int alturaSeguraPTS(NodoAVLpts* a){
    if (a == NULL)return 0;
    return a->altura; 
}

int getBalancePTS(NodoAVLpts* a){
    if (a == NULL)return 0;
    return alturaSeguraPTS(a->der) - alturaSeguraPTS(a->izq); 
}

int cantMayorPTS(NodoAVLpts* a){
    if (a == NULL)
    {
        return 0;
    } else
    {
        return a->cantMayorPts;
    }
    
    
}

void actualizarNodoPTS(NodoAVLpts*& a){
    if(a != NULL){
        a->altura = 1 + max(alturaSeguraPTS(a->der),alturaSeguraPTS(a->izq));
        a->cantMayorPts = a->cant + cantMayorPTS(a->der) + cantMayorPTS(a->izq);
    }
}


void rotacionIzquierdaPTS(NodoAVLpts*& A){
    NodoAVLpts* B = A -> der;
    NodoAVLpts* T2 = B -> izq;
    A -> der = T2;
    B -> izq = A;
    actualizarNodoPTS(A);
    actualizarNodoPTS(B);
    A = B;
}

void rotacionDerechaPTS(NodoAVLpts*& B){
    NodoAVLpts* A = B -> izq;
    NodoAVLpts* T2 = A -> der;
    B -> izq = T2;
    A -> der = B;
    actualizarNodoPTS(A);
    actualizarNodoPTS(B);
    B = A;
}

void addRECpts(NodoAVLpts*& a, int pts){
    if (a == NULL)
    {
        a = new NodoAVLpts;
        a->cant = 1;
        a->pts = pts;
        a->izq = NULL;
        a->der = NULL;
        a->cantMayorPts=0;
        a->altura=1;
    } else if (pts == a->pts)
    {
        a->cant += 1;
        actualizarNodoPTS(a);
        return;
    } else if (a -> pts < pts)
    {
        addRECpts(a->der, pts);
    } else if (a -> pts > pts)
    {
        addRECpts(a->izq, pts);
    }

    int balance = getBalancePTS(a);

    if (balance < -1 && a->izq->pts > pts) 
    {
       //EXPLICACION SITUACION
       /* 
        a   =>          •
                       /
        a-izq =>      •
                     /
        jugNuevo => •
        */
       rotacionDerechaPTS(a);
    }else if (balance < -1 && a->izq->pts < pts) {
        //EXPLICACION SITUACION
        /* 
        a   =>          •
                       /
        a-izq =>      •
                       \
        jugNuevo =>     •
        */
       rotacionIzquierdaPTS(a->izq);
        //EXPLICACION SITUACION
        /* 
        a   =>          •
                       /
        jugNuevo =>   •
                     /
        a->izq =>   •
        */
       rotacionDerechaPTS(a);
    }else if (balance > 1 && a->der->pts < pts) {
        //EXPLICACION SITUACION
        /* 
        a   =>    •
                   \
        a-der =>    •
                     \
        jugNuevo =>   •
        */
       rotacionIzquierdaPTS(a);
    }else if (balance > 1 && a->der->pts > pts) {
        //EXPLICACION SITUACION
        /* 
        a   =>         •
                        \
        a-der =>         •
                        /
        jugNuevo =>    •
        */
       rotacionDerechaPTS(a->der);
        //EXPLICACION SITUACION
        /* 
        a   =>       •
                      \
        jugNuevo =>    •
                        \
        a->der =>        •
        */
       rotacionIzquierdaPTS(a);
    }
    
    actualizarNodoPTS(a);
    
}


//ADD
void add(AVL& a, string nombre, int id, int pts){
    bool seAgrego = false;
    addRECid(a->raizID, nombre, id, pts, seAgrego);
    if (seAgrego) {
        a->cantReg+=1;
        addRECpts(a->raizPTS, pts);
        if(a->nomMejorJug == "sin_jugadores" || a->ptsMejorJug < pts || (a->idMejorJug > id && a->ptsMejorJug == pts))
        {
            a->idMejorJug = id;
            a->nomMejorJug = nombre;
            a->ptsMejorJug = pts;
        }
    }
}

void findREC(NodoAVL* a, int id, string& res){
    if (a == NULL)
    {
        return;
    } else if (a -> id == id)
    {
        res = a->nombre + " " + to_string(a->pts); // funcion que pasa de int a string
    } else if (a -> id > id)
    {
        findREC(a->izq, id, res);
    } else if (a -> id < id)
    {
        findREC(a->der, id, res);
    }
}

//FIND
string find(AVL a, int id){
    string res = "jugador_no_encontrado";
    findREC(a->raizID, id, res);
    return res;
}

int rankREC(NodoAVLpts* a, int p){
    if(!a) return 0;
    if(a->pts >= p){
        return cantMayorPTS(a->der) + a->cant + rankREC(a->izq, p); // O(1) + bajar a izq
    } else {
        return rankREC(a->der, p); // bajar a der
    }
}


//RANK
int rankJug(AVL a, int puntaje){
    return rankREC(a->raizPTS, puntaje);
}

//TOP 1
string top1(AVL a){
    return a->nomMejorJug + " " + to_string(a->ptsMejorJug); 
}

int count(AVL a){
    return a->cantReg;
}