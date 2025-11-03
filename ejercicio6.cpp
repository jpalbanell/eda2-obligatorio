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
    cin >> N; //1
    int M;
    cin >> M; //2
    int F;
    cin >> F; //10
    nodoPozo **pozos = new nodoPozo *[N];
    nodoPotencia **potencias = new nodoPotencia *[M];
    for (int i = 0; i < N; i++)
    {
        int ini;
        int fin;
        cin >> ini; //5
        cin >> fin; //9
        nodoPozo *nodo = new nodoPozo;
        nodo->inicio = ini;
        nodo->final = fin;
        pozos[i] = nodo; /// [(5,9)]
    }
    for (int i = 0; i < M; i++)
    {
        int pos;
        int pot;
        cin >> pos; // 2, 2
        cin >> pot; // 2, 3
        nodoPotencia *nodo = new nodoPotencia;
        nodo->pos = pos;
        nodo->potencia = pot;
        potencias[i] = nodo; // [(2,2);(2,3)]
    }

    MaxHeap H = crearHeap(M);
    bool esPosible = true;
    int cantPedidos = 0;
    int contador = 0;
    for (int i = 0; i < N && esPosible; i++)
    {
        nodoPozo *aux = pozos[i]; 
        int inicioPozo = aux->inicio; // 5
        int distancia = ((aux->final) - inicioPozo) + 2; //6
        int ubiPot = 0;
        while (ubiPot < inicioPozo && contador < M)
        {
            ubiPot = potencias[contador]->pos;
            if (ubiPot < inicioPozo)
            {
                agregarHeap(H, potencias[contador]);
                contador++;
            }
        }
        bool loSalto = false;
        while (!loSalto && !esVacio(H))
        {
            if (potencia < distancia)
            {
                cantPedidos++;
                potencia += eliminar(H);
            }
                if (potencia >= distancia)
                    loSalto = true;
        }
        if (!loSalto)
            esPosible = false;
    }
    if (esPosible)
    {
        cout << cantPedidos << endl;
    }
    else
    {
        cout << "Imposible" << endl;
    }
    return 0;
}