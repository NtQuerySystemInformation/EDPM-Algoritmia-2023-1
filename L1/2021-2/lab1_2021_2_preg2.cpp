#include <iostream>
#include <cmath>
#include <fstream>
#define MAX_CLIENTES 16
using namespace std;

struct Cliente
{
    int id;
    int cantAcciones;
    double porcentajeComision;
};

void InicializarClientes(Cliente arrClientes[], int& n){
    //int n = 0;
    ifstream archIn("Clientes.txt", ios::in);
    if (not archIn.is_open()){
        cout << "No se pudo abrir el archivo Clientes";
        exit(1);
    }
    for (int i = 0; i < n; i++){
        archIn >> arrClientes[i].id;
        if (archIn.eof()) break;
        archIn >> arrClientes[i].cantAcciones >> arrClientes[i].porcentajeComision;
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

int main(int argc, char** argv) 
{
    int T, P, N;
    cout << "Ingrese T, P y N: "; 
    cin >> T >> P >> N;
    struct Cliente arrClientes[MAX_CLIENTES];
    InicializarClientes(arrClientes, N);
    int comb = pow(2, N);
    int* cromo = new int[N], * maxCromo = new int[N];
    double maxComisiones = 0, currComisiones;
    int maxAcciones = 0, currAcciones;
    
    for (int i = 0; i < comb; i++){
        currAcciones = 0;
        currComisiones = 0;
        generarBin(i, N, cromo);
        for (int j = 0; j < N; j++){
            if (cromo[j] == 1){
                currComisiones += (arrClientes[j].cantAcciones * P * arrClientes[j].porcentajeComision);
                currAcciones += arrClientes[j].cantAcciones;
            }
        }
        if (currComisiones > maxComisiones || fabs(maxComisiones - currComisiones) < 1){
            if (currAcciones <= T){
                maxAcciones = currAcciones;
                maxComisiones = currComisiones;
                if (maxAcciones != 0 && maxComisiones != 0){
                    cout << "La ganancia maxima es: " << maxComisiones << " con " << maxAcciones << " acciones\n";
                }
            }
        }
    }
    delete[] cromo;
    delete[] maxCromo;
    
    return 0;
}

