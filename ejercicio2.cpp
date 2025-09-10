#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/tadHASH/Hash.h"

using namespace std;

int main()
{
    // TODO
    int N;
    cin >> N;
    Hash nuevo = crear(N*2);
    put(nuevo, "a", "b", "titu", 12);
    put(nuevo, "b", "f", "titu2", 14);
    put(nuevo, "g", "b", "titu", 12);
    put(nuevo, "y", "f", "titu2", 14);
    put(nuevo, "o", "b", "titu", 12);
    put(nuevo, "p", "f", "titu2", 14);
    // get(nuevo, "a", "b");
    // get(nuevo, "a", "g");
    // get(nuevo, "b", "f");
    return 0;
}