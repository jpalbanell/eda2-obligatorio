#include <cassert>
#include <limits>
#include <string>
#include <iostream> 
using namespace std;

struct RepresentacionMFset
{
    int* padre;
    int* altura;
    int tam;
};
typedef RepresentacionMFset* MFset;

MFset crear(int n){
    MFset nuevo = new RepresentacionMFset;
    nuevo -> altura = new int[n+1];
    nuevo -> padre = new int[n+1];
    nuevo -> tam = n;
    for (int i = 0; i <= n; i++)
    {
        nuevo -> padre[i] = i;
        nuevo -> altura[i] = 1;
    }
    return nuevo;
}

int buscar(MFset& mf, int dato){
    if (mf->padre[dato] == dato)
    {
        return dato;
    }
    mf->padre[dato] = buscar(mf, mf->padre[dato]);
    return mf->padre[dato];
}

bool unir(MFset& mf, int dato1, int dato2){
    int padreDato1 = buscar(mf, dato1);
    int padreDato2 = buscar(mf, dato2);
    if(padreDato1 == padreDato2) return false;

    if (mf->altura[padreDato1] > mf->altura[padreDato2])
    {
        mf->padre[padreDato2] = padreDato1;
    } else if (mf->altura[padreDato1] < mf->altura[padreDato2])
    {
        mf->padre[padreDato1] = padreDato2;
    } else {
        mf->padre[padreDato2] = padreDato1;
        mf->altura[padreDato1]++;
    }
    return true;
}
