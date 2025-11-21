#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

bool podemosPodar(int paso, int mejorPaso)
{
    return paso > mejorPaso;
}

bool esSolucion(char **mat, int fila, int col, char prod)
{
    return mat[fila][col] == prod;
}

bool mejorSolucion(int paso, int mejorPaso)
{
    return paso < mejorPaso;
}

bool puedoAplicarMovimiento(int nuevaFila, int nuevaCol, int M, int N, bool **sandbox, char **mat, char prod)
{
   
    if (nuevaFila < 0 || nuevaCol < 0 || nuevaFila >= M || nuevaCol >= N)
        return false;
   
    if (sandbox[nuevaFila][nuevaCol])
        return false;
    return mat[nuevaFila][nuevaCol] == 'C' || mat[nuevaFila][nuevaCol] == prod;
}

void aplicarMovimiento(int nuevaFila, int nuevaCol, bool **&sandbox)
{
    sandbox[nuevaFila][nuevaCol] = true;
}

void deshacerMovimiento(int nuevaFila, int nuevaCol, bool **&sandbox)
{
    sandbox[nuevaFila][nuevaCol] = false;
}

void backtracking(char **mat, bool **sandbox, int M, int N, int paso, int fila, int col, int &mejorPasoLocal, int mejorPaso, char prod)
{
    int limite = min(mejorPasoLocal, mejorPaso);
    if (!podemosPodar(paso, limite))
    {
        if (esSolucion(mat, fila, col, prod) && mejorSolucion(paso, mejorPasoLocal)){
            mejorPasoLocal = paso;
        }
        else
        {
            int df[4] = {0, 1, 0, -1};
            int dc[4] = {1, 0, -1, 0};
            for (int i = 0; i < 4; i++)
            {
                int nuevaFila = fila + df[i];
                int nuevaCol = col + dc[i];
                if (puedoAplicarMovimiento(nuevaFila, nuevaCol, M, N, sandbox, mat, prod))
                {
                    aplicarMovimiento(nuevaFila, nuevaCol, sandbox);
                    backtracking(mat, sandbox, M, N, paso + 1, nuevaFila, nuevaCol, mejorPasoLocal, mejorPaso, prod);
                    deshacerMovimiento(nuevaFila, nuevaCol, sandbox);
                }
            }
        }
    }
}

int main()
{
    char prod;
    cin >> prod;
    int P;
    cin >> P;
    int M;
    int N;
    int mejorPaso = INT_MAX;
    int Fc = -1;
    for (int i = 0; i < P; i++)
    {
        cin >> M;
        cin >> N;
        char **mat = new char *[M];
        bool **sandbox = new bool *[M];
        for (int m = 0; m < M; m++)
        {
            mat[m] = new char[N];
            sandbox[m] = new bool[N]();
            for (int n = 0; n < N; n++)
            {
                cin >> mat[m][n];
            }
        }
        int paso = 0;
        int mejorPasoLocal = INT_MAX;
        sandbox[0][0] = true;
        backtracking(mat, sandbox, M, N, paso, 0, 0, mejorPasoLocal, mejorPaso, prod);
        if (mejorPasoLocal < mejorPaso)
        {
            mejorPaso = mejorPasoLocal;
            Fc = i + 1;
        }
    
    }
    cout << Fc << " " << mejorPaso << endl;
    return 0;
}