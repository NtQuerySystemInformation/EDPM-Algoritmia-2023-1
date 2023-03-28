#include <iostream>
#include <cmath>
#include <fstream>
#define MAX_VEHICULO 4
using namespace std;

struct TipoVehiculo{
    int tipo;
    int capacidad;
    int rendimiento;
    int costoMantenimiento;
    int cantTipoVehiculo;

};

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

void inicializarTipoVehiculo(TipoVehiculo arrVehiculos[]){
    ifstream archIn;
    archIn.open("Vehiculos.txt", ios::in);
    if (not archIn.is_open()){
        cout << "No se pudo abrir el archivo";
        exit(1);
    }
    for (int i = 0; i < 4; i++){
        archIn >> arrVehiculos[i].tipo;
        if (archIn.eof()) break;
        archIn >> arrVehiculos[i].capacidad >> arrVehiculos[i].rendimiento
               >> arrVehiculos[i].costoMantenimiento >> arrVehiculos[i].cantTipoVehiculo;
    }
}

//Generamos un cromo que va a contener todos los vehiculos en un solo cromo
//En la funcion obtener indices vamos a separar cada parte 
void generarCromo(int*& cromo, TipoVehiculo arrVehiculo[], int& cantVehiculos)
{
    cantVehiculos = 0;
    for (int i = 0; i < MAX_VEHICULO; i++){
        cantVehiculos += arrVehiculo[i].cantTipoVehiculo;
    }
    cromo = new int [cantVehiculos];
}

int obtenerIndices(int j, TipoVehiculo arrTipoVehiculos[])
{
    int* rango = new int[MAX_VEHICULO];
    for (int i = 0; i < MAX_VEHICULO; i++){
        rango[i] = arrTipoVehiculos[i].cantTipoVehiculo;
    }
    /*
    int idx = (j >= 0 && j < rango[0])*0 +  //Comparacion i == 0;
               (j >= rango[0] && j < rango[1] + rango[0])*1 + //comparacion i == 1
               (j >= rango[1] + rango[0] && j < rango[0] + rango[1] + rango[2])*2 + //comparacion i == 2 
               (j>= rango[0] + rango[1] + rango[2] && j < rango[0] + rango[1] + rango[2] + rango[3])*3;
    */
    int idx = -1;
    for (int i = 0, k = 0; i < MAX_VEHICULO; i++){
        int currCheck1 = k; //0, 
        int currCheck2 = k + rango[i];//7
        //std::printf("Check %d: [%d, %d]\n", i+1, currCheck1, currCheck2);        
	if (j >= currCheck1 && j < currCheck2){
            idx = i;
            break;
	}
        k += rango[i];
    }    
    delete[] rango;
    return idx;
}

void imprimirSolucion(int minCosto, int minComb, TipoVehiculo arrTipoVehiculos[], int cantVehiculos){
    int* cromo = new int [cantVehiculos];
    int cantTipo1 = 0, cantTipo2 = 0, cantTipo3 = 0, cantTipo4 = 0;
    generarBin(minComb, cantVehiculos, cromo);
    for (int i= 0; i<cantVehiculos; i++){
        if (cromo[i] == 1){
            int idx = obtenerIndices(i, arrTipoVehiculos);
            switch (idx + 1){
                case 1:{
                    cantTipo1++;
                    break;
                }
                case 2: {
                    cantTipo2++;
                    break;
                }
                case 3: {
                    cantTipo3++;
                    break;
                }
                case 4: {
                    cantTipo4++;
                    break;
                }
                default: {
                    continue;
                }
            };
        }
    }
    cout << "El costo minimo es: " << minCosto << endl;
    cout << "Cantidad de vehiculos por tipo: \n";
    cout << "Tipo 1: " << cantTipo1 << endl << "Tipo 2: " << cantTipo2 << endl
         << "Tipo 3: " << cantTipo3 << endl << "Tipo 4: " << cantTipo4 << endl;

    delete[] cromo;
}


//13 7000 13
int main(int argc, char** argv) {
    int M, D, P;
    cout << "Ingresa M, D y P: ";
    cin >> M >> D >> P;
    TipoVehiculo arrTipoVehiculos[MAX_VEHICULO];
    inicializarTipoVehiculo(arrTipoVehiculos);
    if (D < 1000){
        cout << "Ingrese un valor mayor a una tonelada..";
        exit(1);
    }
    int* cromo = nullptr, cantVehiculos;
    generarCromo(cromo, arrTipoVehiculos, cantVehiculos);
    int comb = (int)pow(2, cantVehiculos);
	
    int minCosto = 0, currCosto, currCargaPeso, minComb; 
    for (int i = 0; i < comb; i++){
        currCargaPeso = 0;
        currCosto = 0;
        generarBin(i, cantVehiculos, cromo);
        for (int j = 0; j < cantVehiculos; j++){
            if (cromo[j] == 1){
                int idx = obtenerIndices(j, arrTipoVehiculos);
                double costoRecorrido = D * (1.0/arrTipoVehiculos[idx].rendimiento) * P;
                double costoMantenimiento = arrTipoVehiculos[idx].costoMantenimiento * (D/5000);
                currCosto += costoRecorrido + costoMantenimiento;
                currCargaPeso += arrTipoVehiculos[idx].capacidad * 1000;
            }
        }
        if (currCargaPeso >= M*1000){
            if (minCosto > currCosto || minCosto == 0){
                minCosto = currCosto;
                minComb = i;
                imprimirSolucion(minCosto, minComb, arrTipoVehiculos, cantVehiculos);
            }
        }
    }
    return 0;
}