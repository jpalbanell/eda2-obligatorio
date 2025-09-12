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
    Hash nuevo = crear(N);
    contains(nuevo, "a", "c");
    remove(nuevo, "a", "b");
    contains(nuevo, "a", "b");
    get(nuevo, "a", "b");
    // get(nuevo, "a", "g");
    // get(nuevo, "b", "f");
    return 0;
}