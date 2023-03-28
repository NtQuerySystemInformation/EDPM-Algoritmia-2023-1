#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

#define MAX_MAQUINA 10

struct Maquina{
    int id;
    int costo;
    int ganancia;
    int tipo;
};

void inicializarMaquina(Maquina arrMac[], int n){
    ifstream arch;
    arch.open("Maquinas.txt", ios::in);
    if (not arch.is_open()){
        cout << "No se pudo abrir el archivo Maquina";
        exit(1);
    }
    for (int i = 0; i < n;  i++){
        arch >> arrMac[i].id;
        if (arch.eof()) break;
        arch >> arrMac[i].costo;
        arch >> arrMac[i].ganancia >> arrMac[i].tipo;
    }
}

void generarBin(int num, int n, int cromo[])
{
    int i, res;
    for (i = 0; i < n; i++){
        cromo[i] = 0;
    }	
    i = 0;
    while(num > 0){
        res = num % 2;
        num = num / 2;
        cromo[i] = res;
        i++;
    }
}

void imprimirSolucion(Maquina arrMac[], int* cromo, int N, int g, int costo){
    cout << "Maquinas: ";
    for (int i = 0; i < N; i++){
        if (cromo[i] == 1){
            cout << arrMac[i].id << " ";
        }
    }
    cout << ". Costo de máquinas: " << costo << "(Millones de $) y una ganancia de " << g << "(Millones de $)." << endl;
}

int main(int argc, char** argv) {
    struct Maquina arrMac[MAX_MAQUINA];
    int N, P;
    cout << "Ingresar N y P: ";
    cin >> N >> P;
    inicializarMaquina(arrMac, N);
    int comb = (int)pow(2, N);
    int cromo[N];
    int currCosto, currGanancia, maxGanancia = 0, cantTipo1, cantTipo2, cantTipo3;
    
    for (int i = 0; i < comb; i++){
        generarBin(i, N, cromo); 
        cantTipo1 = 0, cantTipo2 = 0, cantTipo3 = 0, currCosto = 0, currGanancia = 0;
        for (int j = 0; j < N; j++){
            if (cromo[j] == 1){
                switch (arrMac[j].tipo){
                    case 1:{
                        cantTipo1++;
                        break;
                    }
                    case 2:{
                        cantTipo2++;
                        break;
                    }
                    case 3:{
                        cantTipo3++;
                        break;
                    }
                }
                currCosto += arrMac[j].costo;
                currGanancia += arrMac[j].ganancia;
            }
        }
        if (cantTipo1 > 0 && cantTipo2 > 0 && cantTipo3 > 0){
            if (currCosto <= P){
                if (currGanancia > maxGanancia){
                    maxGanancia = currGanancia;
                    imprimirSolucion(arrMac, cromo, N, maxGanancia, currCosto);
                }
            }
        }
    }
    system("pause");
    
    
    return 0;
}

