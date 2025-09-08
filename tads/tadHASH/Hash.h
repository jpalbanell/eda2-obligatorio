#include <string>
#include <cassert>
#include <iostream>
#include <limits>

using namespace std; 

struct NodoHash
{
    string path;
    string titulo;
    int tiempo;
    NodoHash* sig;
};

struct RepresentacionHash
{
    NodoHash** tabla;
    int cantidad;
};
typedef RepresentacionHash *Hash; 

Hash crear(){
    Hash nuevo = new RepresentacionHash;
    

}


// Ref: https://cseweb.ucsd.edu/~kube/cls/100/Lectures/lec16/lec16-15.html
// diapositivas de la clase
int hash3(string key) {
  int h = 0;
  for (int i = 0; i < key.length(); i++)
    h = 31 * h + int(key[i]);
  return h;
}

int hashSec(string key2){
  int h = 0;
  for (int i = 0; i < key2.length(); i++) h = 37 * h + int(key2[i]);
  return (h);
}


