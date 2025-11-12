#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

int max(int a, int b){
    return a > b ? a : b;
    
}

int func(int* numeros, int*** memo, int i, int f, int k){
    if (i == f) return ((k+1)*(k+1));
    if (i > f) return 0;
    if (memo[i][f][k] != -1) return memo[i][f][k];
    

    int max1 = 0;
    int aux1 = 0;
    int aux2 = 0;

    for (int m = i+1; m <= f; m++)
    {
        if (numeros[m] == numeros[i])
        {
            aux1 = func(numeros, memo, i+1, m-1, 0);
            memo[i+1][m-1][0] = aux1;
            aux2 = func(numeros, memo, m, f, k+1);
            memo[m][f][k+1] = aux2;
            if (max1 < aux1 + aux2) max1 = aux1 + aux2;
        }
    }

    int max2 = func(numeros, memo, i+1, f, 0) + ((k+1)*(k+1));

    int res = max(max1, max2);
    memo[i][f][k] = res;
    return res;
}

int main()
{
    // TODO
    int N;
    cin >> N;
    int *numeros = new int[N];
    for (int i = 0; i < N; i++) cin >> numeros[i];

    int ***memo = new int **[N];
    for (int i = 0; i < N; i++){
        memo[i] = new int*[N];
        for (int f = 0; f < N; f++){
            memo[i][f] = new int[N];
            for (int k = 0; k < N; k++){
                 memo[i][f][k] = -1;
            }
        }
    }

    int res = func(numeros, memo, 0, N-1, 0);
    cout << res << endl;
    return 0;
}