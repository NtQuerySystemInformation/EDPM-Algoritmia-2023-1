/* 
 * File:   main.cpp
 * Author: rodri
 *
 * Created on March 20, 2023, 11:39 AM
 */

#include <iostream>
#include <cmath>
#include <fstream>
#define MAX_PRODUCTOS 15
#define MAX_BAHIAS 3

using namespace std;

struct Bahia
{
    int idBahia;
    int productos[MAX_PRODUCTOS];
    int cantProductos;
};

struct Despacho{
    Bahia bahias[MAX_BAHIAS];
};
    
void leerDespacho(Despacho* tmp, int N)
{
    ifstream archIn;
    archIn.open("Input.txt", ios::in);
    if (!archIn.is_open()){
        printf("No se pudo abrir Input.txt");
        exit(1);
    }
    for (int i = 0; i < MAX_BAHIAS; i++){
        tmp->bahias[i].cantProductos = N;
        tmp->bahias[i].idBahia = i + 1;
        for (int j = 0; j < tmp->bahias[i].cantProductos; j++){
            archIn >> tmp->bahias[i].productos[j];
            if (archIn.eof()) break;
            printf("Se ingreso el dato (%d, %d) = %d\n", i, j, tmp->bahias[i].productos[j]);
        }
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

void obtenerIndices(int j, Despacho& tmp, int& idx, int& jdx){
    idx = -1;
    jdx = -1;
    int cantSec = tmp.bahias[0].cantProductos;
    if (0 <= j && j <= cantSec - 1){
        idx = 0;
    }else if(cantSec <= j && j <= ((2*cantSec) - 1)){
        idx = 1;
    }else if (2*cantSec <= j && j <= ((3*cantSec) - 1)) {
        idx = 2;
    }
    if (idx == -1){
        printf("La posicion encontrada no es valida para j = %d\n", j);
        exit(1);
    }
    j %= cantSec;
    jdx = j;
}

int contarActivos(int* cromo, int n){
    int cant = 0;
    for (int i = 0; i < n; i++){
        if (cromo[i] == 1){
            cant++;
        }
    }
    return cant;
}

void imprimirSolucion(int* cromo, Despacho& tmp, int n){
    for (int i = 0; i < MAX_BAHIAS; i++){
        printf("Bahia %d: ", tmp.bahias[i].idBahia);
        int idx, jdx;
        for (int j = 0; j < n; j++){
            if (cromo[j] == 1){
                obtenerIndices(j, tmp, idx, jdx);
                if (idx == i){
                    printf(" %d ", tmp.bahias[idx].productos[jdx]);
                }
            }
        }
    }
    printf("\n");
}


int main(int argc, char** argv) {
    int N, M, C;
    printf("Ingresar N: ");
    scanf("%d", &N);
    Despacho des;
    printf("Ingresar M: ");
    scanf("%d", &M);
    printf("Ingresar C: ");
    scanf("%d", &C);
    leerDespacho(&des, N);
    int maxComb = 3 * N;
    int comb = (int)pow(2, maxComb);
    int* cromo = new int[maxComb];
    int currC;
    for (int i = 0; i < comb; i++){
        generarBin(i, maxComb, cromo);
        currC = 0;
        int cantAct = contarActivos(cromo, maxComb);
        if (cantAct == M){
            for (int j = 0; j < maxComb; j++){
                if (cromo[j] == 1){
                    int idx, jdx;
                    obtenerIndices(j, des, idx, jdx);
                    currC += des.bahias[idx].productos[jdx];
                }
            }
            if (currC == C){
                imprimirSolucion(cromo, des, maxComb);
            }
        }
    }
    delete[] cromo;
    return 0;
}

