#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/tadHeap/maxHeapPot.h"

using namespace std;

struct nodoPozo
{
    int inicio;
    int final;
};


int main()
{
    // TODO
    int potencia = 1;
    int N;
    cin >> N;
    int M;
    cin >> M;
    int F;
    cin >> F;
    nodoPozo** pozos = new nodoPozo*[N];
    nodoPotencia** potencias = new nodoPotencia*[M];
    for (int i = 0; i < N; i++)
    {
        int ini;
        int fin;
        cin >> ini;
        cin >> fin;
        nodoPozo* nodo = new nodoPozo;
        nodo->inicio = ini;
        nodo->final = fin;
        pozos[i] = nodo;
    }
    for (int i = 0; i < M; i++)
    {
        int pos;
        int pot;
        cin >> pos;
        cin >> pot;
        nodoPotencia* nodo = new nodoPotencia;
        nodo->pos = pos;
        nodo->potencia = pot;
        potencias[i] = nodo;
    }
    
    MaxHeap H = crearHeap(M);
    bool esPosible = true;
    int cantPedidos = 0;

    for (int i = 0; i < N && esPosible; i++)
    {
        nodoPozo* aux = pozos[i];
        int inicioPozo = aux -> inicio;
        int distancia = ((aux -> final) - inicioPozo) + 2;
        int ubiPot = 0;
        int contador = 0;
        while (ubiPot < inicioPozo)
        {
            ubiPot = potencias[contador]->pos;
            if (ubiPot < inicioPozo)
            {
                agregarHeap(H, potencias[contador]);
            }
            contador++;
        }
        bool loSalto = false;
        while (!loSalto && !esVacio(H))
        {
            if (potencia < distancia)
            {
                cantPedidos++;
                potencia += eliminar(H);
                if (potencia >= distancia)
                {
                    loSalto = true;
                }
                
            }
            
        }
        if (!loSalto)
        {
            esPosible = false;
        }
    } 
    if (esPosible)
    {
        cout << cantPedidos;
    } else {
        cout << "Imposible";
    }
    return 0;
}