#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

int max(int a, int b){
    return a > b ? a : b;
    
}

int main()
{
    // TODO
    int N;
    cin >> N;
    int S;
    cin >> S;
    int L;
    cin >> L;
    int* tams = new int[N+1];
    int* lins = new int[N+1];
    int* pts = new int[N+1];

    for (int i = 0; i <= N; i++)
    {
        cin >> tams[i];
        cin >> lins[i];
        cin >> pts[i];
    }

    int ***tab = new int **[N+1];
    for (int i = 0; i <= N; i++){
        tab[i] = new int*[S+1];
        for (int f = 0; f <= S; f++){
            tab[i][f] = new int[L+1];
            for (int k = 0; k <= L; k++){
                 tab[i][f][k] = 0;
            }
        }
    }

    int maxP = 0;

    for (int i = 1; i <= N; i++)
    {
        int auxT = tams[i];
        int auxL = lins[i];
        int auxPts = pts[i];
        for (int j = 0; j <= S; j++)
        {
            for (int k = 0; k <= L; k++)
            {
                if (auxT > j || auxL > k)
                {
                    tab[i][j][k] = tab[i-1][j][k];
                }
                else
                {
                    int valorTomar = auxPts + tab[i-1][j - auxT][k - auxL];
                    int valorNoTomar = tab[i-1][j][k];
                    int conviene = max(valorTomar, valorNoTomar);
                    tab[i][j][k] = conviene;
                    if (conviene > maxP) maxP = conviene;
                }
                
            }
            
        }
     
    }

    cout << maxP << endl;

    return 0;
}