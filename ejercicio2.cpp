#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/tadHASH/Hash.h"

using namespace std;

int main()
{
    // TODO
    int n;
    cin >> n;
    Hash a = crear(n);
    for (int i = 0; i < n; i++)
    {
        string comando;
        cin >> comando;
        if (comando == "PUT")
        {
            string dom;
            string path;
            string tit;
            int tiempo;
            cin >> dom;
            cin >> path;
            cin >> tit;
            cin >> tiempo;
            put(a, dom, path, tit, tiempo);
        }
        else if (comando == "GET")
        {
            string dom;
            string path;
            cin >> dom;
            cin >> path;
            get(a, dom, path);
        }
        else if (comando == "REMOVE")
        {
            string dom;
            string path;
            cin >> dom;
            cin >> path;
            remove(a, dom, path);
        }
        else if (comando == "CONTAINS")
        {
            string dom;
            string path;
            cin >> dom;
            cin >> path;
            if (contains(a, dom, path))
            {
                cout << "true" << endl;
            } else {
                cout << "false" << endl;
            }
            
        }
        else if (comando == "COUNT_DOMAIN")
        {
            string dom;
            cin >> dom;
            cout << count_domain(a, dom) << endl;
        }
        else if (comando == "LIST_DOMAIN")
        {
            string dom;
            cin >> dom;
            list_domain(a, dom);
        }
        else if (comando == "CLEAR_DOMAIN")
        {
            string dom;
            cin >> dom;
            clear_domain(a, dom);
        }
        else if (comando == "SIZE")
        {
            cout << size(a) << endl;
        }
        else if (comando == "CLEAR")
        {
            clear(a);
        }
    }   
    return 0;
}