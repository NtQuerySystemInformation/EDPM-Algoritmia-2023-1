/* 
 * File:   main.cpp
 * Author: rodri
 *
 * Created on March 22, 2023, 11:23 AM
 */

#include <iostream>
#include <cmath>
#include <fstream>
#define MAX_RENOVAR 10

struct Herramienta{
    int tipo;
    int precio;
    int cantRenovar;
};


using namespace std;

void inicializarHerramienta(Herramienta arr[], int& n){
    int i = 0;
    ifstream archIn;
    archIn.open("Tools.txt", ios::in);
    if (!archIn.is_open()){
        cout << "No se pudo abrir herramientas.txt" << endl;
        exit(1);
    }
    while(1){
        archIn >> arr[i].tipo;
        if (archIn.eof()) break;
        archIn >> arr[i].precio >> arr[i].cantRenovar;
        i++;
    }
    n = i;
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

int contarUnos(int cromo[], int n)
{
    int j = 0;
    for (int i = 0; i < n; i++){
        if (cromo[i] == 1){
            j++;
        }
    }
    return j;
}

int main(int argc, char** argv) {
    
    Herramienta arrTools[MAX_RENOVAR];
    int cantTools;
    inicializarHerramienta(arrTools, cantTools);
    int M, P;
    cout << "Ingrese M: ";
    cin >> M;
    cout << "Ingrese P: ";
    cin >> P;
    int comb = (int)pow(2, cantTools);
    int currSuma, currCantTool, maxSuma = 0, maxComb, maxCantTool;
    int* cromo = new int[cantTools];
    for (int i = 0; i < comb; i++){
        currSuma = 0;
        currCantTool = 0;
        generarBin(i, cantTools, cromo);
        if (contarUnos(cromo, cantTools) == M){
            for (int j = 0; j < cantTools; j++){
                if (cromo[j] == 1){
                    currSuma += arrTools[j].precio * arrTools[j].cantRenovar;
                    currCantTool += arrTools[j].cantRenovar;
                }
            }
            if (currSuma > maxSuma && currSuma <= P){
                maxComb = i;
                maxSuma = currSuma;
                maxCantTool = currCantTool;
            }
        }
    }
    generarBin(maxComb, cantTools, cromo);
    for (int j = 0; j < cantTools; j++){
        if (cromo[j] == 1){
            cout << arrTools[j].tipo << " ";
        }
    }
    cout << endl;
    cout << "Monto Total: " << maxSuma << endl;
    cout << "Cantidad de herramientas: " << maxCantTool << endl;
    
    delete[] cromo;
    
    return 0;
}

