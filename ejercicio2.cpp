#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/tadHASH/Hash.h"

using namespace std;

int main()
{
    // TODO
    Hash nuevo = crear(103);
    put(nuevo, "a", "b", "titu", 12);
    put(nuevo, "a", "f", "titu2", 14);
    put(nuevo, "b", "b", "titu", 12);
    put(nuevo, "a", "f", "titu2", 14);
    put(nuevo, "d", "b", "titu", 12);
    put(nuevo, "a", "f", "titu2", 14);
    

    return 0;
}