#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/tadHASH/hashCerrado.h"

using namespace std;

int* intercalar(int* ord1, int n1, int* ord2, int n2, Hash criterio, long long& cambios) {
    int* ret = new int[n1 + n2];

    int i = 0;
    int j = 0;
    int k = 0; 
    int cambiosIzq = n1;
    while (i<n1 && j<n2)
    {
        if (ord1[i] > ord2[j])
        {
            ret[k]=ord2[j];
            k++; j++;
            cambios+=cambiosIzq;
        } else {
            ret[k]=ord1[i];
            k++; i++;
            cambiosIzq--;
        }
        
    }

    while (i < n1) {
        ret[k] = ord1[i];
        k++; i++;
    }

    while (j < n2){
        ret[k] = ord2[j];
        k++; j++;
    }

    return ret;
}

int* mergeSortAlumno(int* aOrdenar, int largoCriterio, Hash criterio, int inicio, int fin, long long& cambios){
    if (inicio == fin) {
        int* ret = new int [1];
        ret[0] = aOrdenar[inicio];
        return ret;
    }

    int mitad = (inicio + fin)/2;

    int* ord1 = mergeSortAlumno(aOrdenar, largoCriterio, criterio, inicio, mitad, cambios);
    int* ord2 = mergeSortAlumno(aOrdenar, largoCriterio, criterio, mitad+1, fin, cambios);
    
    int n1 = mitad - inicio + 1;
    int n2 = fin - mitad;

    return intercalar(ord1, n1, ord2, n2, criterio, cambios);
}

int main()
{
    // TODO
    int N;
    cin >> N;
    Hash criterio = crear(N);
    string* alumnosProfe = new string[N];
    for (int i = 0; i < N; i++) {
        string nombre;
        cin >> nombre; 
        put(criterio, nombre, i);
    }
    
    int* alumnosAyudante = new int[N];
    for (int i = 0; i < N; i++) {
        string alumno;
        cin >> alumno;
        int pos = get(criterio, alumno);
        alumnosAyudante[i] = pos;
    }
    
    long long cantSwap = 0;
    //usamos long long porque int genera overflow
    alumnosAyudante = mergeSortAlumno(alumnosAyudante, N, criterio, 0, N-1, cantSwap);
    cout << cantSwap << endl;
    return 0;
}