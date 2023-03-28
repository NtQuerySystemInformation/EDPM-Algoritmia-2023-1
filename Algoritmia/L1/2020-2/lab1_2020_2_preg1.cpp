#include <iostream>
#include <cmath>
#include <fstream>
#define MAX_OBREROS 12
using namespace std;

struct Obrero{
    int id;
    int rotura;
    int salario;
};

void inicializarObreros(Obrero arr[], int& n){
    ifstream arch;
    arch.open("Obreros.txt", ios::in);
    if (not arch.is_open()){
        cout << "No se pudo abrir el archivo entrada "<< endl;
        exit(1);
    }
    n = 0;
    while(1){
        arch >> arr[n].id;
        if (arch.eof()) break;
        arch >> arr[n].rotura >> arr[n].salario;
        n++;
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

int contarObreros(int cromo[], int n){
    int c = 0;
    for (int i = 0; i < n; i++){
        if (cromo[i] == 1){
            c++;
        }
    }
    return c;
}
void imprimirSolucion(int* cromo, int n, Obrero arr[], int minRot, int minPrecio){
    cout << "Obreros: "; 
    int cant = 0;
    for (int i = 0; i < n; i++){
        if (cromo[i] == 1){
            cout << arr[i].id << " "; 
            cant++;
        }
    }
    cout << ". Costo de Salarios de " << minPrecio << "(Miles de $) y una rotura promedio de " << (double)minRot/cant << endl;
}

int main(int argc, char** argv) 
{
    struct Obrero arrObreros[MAX_OBREROS];
    int N, M, P, cantObreros;
    cout << "Inicializar N, M y P: ";
    cin >> N >> M >> P;
    inicializarObreros(arrObreros, cantObreros);
    int comb = (int)pow(2, cantObreros);
    int* cromo = new int[cantObreros];

    int currPromRotura, minRotura = 0, currPrecioPresu, minPrecioPresu = 0, minRotIdx;
    for (int i = 0; i < comb; i++){
        currPrecioPresu = 0;
        currPromRotura = 0;
        generarBin(i, cantObreros, cromo);
        int numObreros = contarObreros(cromo, cantObreros); 
        if (numObreros >= M and numObreros <= N){
            for (int j = 0; j < cantObreros; j++){
                if (cromo[j] == 1){
                    currPromRotura += arrObreros[j].rotura;
                    currPrecioPresu += arrObreros[j].salario;
                }
            }
            if (currPrecioPresu <= P){
                if (currPromRotura < minRotura || minRotura == 0){
                    minRotIdx = i;
                    minRotura = currPromRotura;
                    minPrecioPresu = currPrecioPresu;
                    imprimirSolucion(cromo, cantObreros, arrObreros, minRotura, minPrecioPresu);
                }
            }
        }
    }
    //generarBin(minRotIdx, cantObreros, cromo);
    //imprimirSolucion(cromo, cantObreros, arrObreros, minRotura, minPrecioPresu);
    delete[] cromo;
    return 0;
}

