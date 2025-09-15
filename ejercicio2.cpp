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
    put(nuevo,nuevo, "google.com", "path1", "titulo2", 25);
    put(nuevo,nuevo, "google.com", "path12", "titulo2", 25323);
    put(nuevo,nuevo, "fb.com", "pathsad1", "titulo2", 25);
    put(nuevo,nuevo, "ig.com", "path1", "titulo45", 25323);
    put(nuevo,nuevo, "mail.com", "path143223", "titulo24232", 2543224);

    contains(nuevo, "a", "c");
    remove(nuevo, "a", "b");
    contains(nuevo, "a", "b");
    get(nuevo, "a", "b");
    // get(nuevo, "a", "g");
    // get(nuevo, "b", "f");
    return 0;
}