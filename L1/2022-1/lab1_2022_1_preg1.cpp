/* 
 * File:   main.cpp
 * Author: rodri
 *
 * Created on March 22, 2023, 11:52 AM
 */

#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

struct Canal{
    int codigo;
    char canal[30];
    int inversion;
    double roas;
    int recordacionMarca;
};

void inicializarCanales(Canal arrCanales[], int N){
    ifstream archCanal;
    archCanal.open("Canales.txt", ios::in);
    if (!archCanal.is_open()){
        cout << "No se pudo abrir el archivo Canales\n";
        exit(1);
    }
    int n = 0; 
    while(1){
        archCanal >> arrCanales[n].codigo;
        if (archCanal.eof()) break;
        archCanal >> arrCanales[n].canal;
        archCanal >> arrCanales[n].inversion >> arrCanales[n].roas
                  >> arrCanales[n].recordacionMarca;
        n++;
    }
    if (n != N){
        cout << "Error: La cantidad de canales leida no es igual a N\n";
        exit(1);
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

void imprimirSolucionRec(int cromoMax[], int N, Canal arrCanales[]){
    cout << "Inicio soluciones Recordacion: \n";
    for (int j = 0; j < N; j++){
        if (cromoMax[j] == 1){
            cout << "Canal " << arrCanales[j].codigo << "-" << arrCanales[j].canal
                 << ", Recordacion marca: " << arrCanales[j].recordacionMarca << endl;
        }
    }
    cout << endl;
} 
void imprimirSolucionIng(int cromoMax[], int N, Canal arrCanales[]){
    cout << "Inicio soluciones Ingresos: \n";
    for (int j = 0; j < N; j++){
        if (cromoMax[j] == 1){
            cout << "Canal " << arrCanales[j].codigo << "-" << arrCanales[j].canal
                 << ", Ingreso: " << (arrCanales[j].roas * (double)arrCanales[j].inversion) << endl;
        }
    }
    cout << endl;
}

void copiarCromo(int* cromoI, int* cromoJ, int n){
    for (int i = 0; i < n; i++){
        cromoI[i] = 0;
    } 
    for (int i = 0; i < n; i++){
        cromoI[i] = cromoJ[i];
    }
}

int main(int argc, char** argv) 
{
    int N, P;
    cout << "Ingrese N: ";
    cin >> N;
    cout << "Ingrese P: ";
    cin >> P;
    Canal* arrCanales = new Canal[N];
    inicializarCanales(arrCanales, N);
    int comb = (int)pow(2, N);
    int* cromo = new int[N], * cromoMaxRec = new int[N], * cromoMaxIng = new int [N];
    int maxRecordacion = 0, currRecordacion, currInvPubli;
    double currIngreso, maxIngreso = 0;
    
    for (int i = 0; i < comb; i++){
       currIngreso = 0;
       currInvPubli = 0;
       currRecordacion = 0;
       generarBin(i, N, cromo);
       for (int j = 0; j < N; j++){
           if (cromo[j] == 1){
               currIngreso += (arrCanales[j].roas * (double)arrCanales[j].inversion);
               currRecordacion += arrCanales[j].recordacionMarca;
               currInvPubli += arrCanales[j].inversion;
           }
        }
        if (currInvPubli <= P){
            if (currRecordacion > maxRecordacion){
                copiarCromo(cromoMaxRec, cromo, N);
                maxRecordacion = currRecordacion;
                //imprimirSolucionRec(cromo, N, arrCanales);
            }
            if (currIngreso > maxIngreso){
                copiarCromo(cromoMaxIng, cromo, N);          
                maxIngreso = currIngreso;
            }
        }
    }
    cout << "Max recordacion marca: " << maxRecordacion << endl;
    cout << "Max Ingreso: " << maxIngreso << endl;
    
    imprimirSolucionRec(cromoMaxRec, N, arrCanales);
    imprimirSolucionIng(cromoMaxIng, N, arrCanales);
    delete[] arrCanales;
    delete[] cromo;
    delete[] cromoMaxRec;
    delete[] cromoMaxIng;
    
    return 0;
}

