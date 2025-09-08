#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/tadAVL/AVL.h"

using namespace std;

int main()
{
    AVL a = crearAVL();
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string comando;
        cin >> comando;
        if (comando == "ADD")
        {
            int id;
            string nom;
            int puntaje;
            cin >> id;
            cin >> nom;
            cin >> puntaje;
            add(a, nom, id, puntaje);
        }
        else if (comando == "FIND")
        {
            int id;
            cin >> id;
            string ret = find(a, id);
            cout << ret << endl;
        }
        else if (comando == "RANK")
        {
            int puntaje;
            cin >> puntaje;
            int cant = rankJug(a, puntaje);
            cout << cant << endl;
        }
        else if (comando == "TOP1")
        {
            cout << top1(a) << endl;
        }
        else if (comando == "COUNT")
        {
            cout << count(a) << endl;
        }
    }   
    return 0;
}