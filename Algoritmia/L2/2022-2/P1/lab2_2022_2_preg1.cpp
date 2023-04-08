#include <fstream>
#include <cmath>
#include <iostream>
#define MAX 100

using namespace std;

void inicializarTablero(int tablero[][MAX], int N, int M){
    ifstream archIn;
    archIn.open("Tablero.txt", ios::in);
    if (not archIn.is_open()){
        cout << "No se pudo abrir el tablero\n";
        exit(1);
    }
    int i = 0, j = 0, val;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++){
            archIn >> tablero[i][j];
            cout << "(" << i << "," << j << "): " << tablero[i][j] << endl; 
        }
    }
    archIn.close();
}

void inicializarTablero2(int tablero[][MAX], int N, int M){
    ifstream archIn;
    archIn.open("Tablero2.txt", ios::in);
    if (not archIn.is_open()){
        cout << "No se pudo abrir el tablero\n";
        exit(1);
    }
    int i = 0, j = 0, val;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++){
            archIn >> tablero[i][j];
            //cout << "(" << i << "," << j << "): " << tablero[i][j] << endl; 
        }
    }
    archIn.close();
}


void InsertaProdDerecha(int tablero[][MAX], int z, int x, int y, bool& flagEnd, int& currCantProd, int& targetCantProd)
{
    for (int i = 1; i <= z; i++){
        int jdx2 = y + i;
        if (tablero[x][jdx2] == 0) {
            tablero[x][jdx2] = 5;
            currCantProd++;
        }
        flagEnd = (currCantProd >= targetCantProd) ? true : false;
        if (flagEnd) break;
    }
}

//Esta funcion utiliza Z para iterar tanto por izquierda como por derecha.
//Este approach probablemente se puede optimizar a un approach recursivo, al estilo del "robot".
void InsertaProdIzquierda(int tablero[][MAX], int z, int x, int y, bool& flagEnd, int& currCantProd, int& targetCantProd){
   for (int i = 1; i <= z; i++){
        int jdx = y - i;
        if (tablero[x][jdx] == 0){ 
            tablero[x][jdx] = 5;
            currCantProd++;
        }
        flagEnd = (currCantProd >= targetCantProd) ? true : false;
        if (flagEnd) break;
    }
}

void AgregarCentroProducto(int tablero[][MAX], int x, int y, int opcion, int& currCantProd){
    if (tablero[x][y] == 0){
        tablero[x][y] = 5;
        currCantProd++;
    }
    if (opcion == 2){
        if (tablero[x][y + 1] == 0){
            tablero[x][y + 1] = 5;
            currCantProd++;
        }
    }
}


//Modifcar funcion recursiva para impar/par.
bool ordenarAnaquel(int tablero[][MAX], int x, int y, int z, int n, int& currCantProd, int targetCantProd, int opcion, bool flagEnd){
    if (flagEnd){
        cout << "Cantidad de productos: " << currCantProd << endl;
        return flagEnd;
    }
    if (y + 1 > n){
        return false;
    }
    AgregarCentroProducto(tablero, x, y, opcion, currCantProd);
    if (opcion == 1){
        InsertaProdIzquierda(tablero, z, x, y, flagEnd, currCantProd, targetCantProd);
        InsertaProdDerecha(tablero, z, x, y, flagEnd, currCantProd, targetCantProd);     
    }else{
        InsertaProdIzquierda(tablero, z, x, y, flagEnd, currCantProd, targetCantProd);
        InsertaProdDerecha(tablero, z, x, y+1, flagEnd, currCantProd, targetCantProd);
        
    }
    int currY = (opcion == 1) ? y : y + 1;
    if (currY + z + 1 < n){
        z = z + 1;
    }
    ordenarAnaquel(tablero, x + 1, y, z, n, currCantProd, targetCantProd, opcion, flagEnd);   
}

bool ordenarAnaquel(int tablero[][MAX], int n, int cantProd){
    int pMx, opcion, currCantProd = 0;
    bool res, flagEnd = false;
    if (n % 2 == 0){
        pMx = (n)/2 - 1;
        opcion = 2;
    }else{
        pMx = ((n+1)/2) - 1;
        opcion = 1;
    }
    res = ordenarAnaquel(tablero, 0, pMx, 0, n, currCantProd, cantProd, opcion, flagEnd);
    return res;
}

void imprimirTablero(int tablero[][MAX], int F){
    cout << "Solucion tablero: " << endl;
    for (int i = 0; i < F; i++){
        for (int j = 0; j < F; j++){
            cout << tablero[i][j] << " | ";
        }
        cout << endl;
    }
}

int main(int argc, char** argv) {

    int F, C, P;
    cout << "Ingrese F, C y P: ";
    cin >> F >> C >> P;
    //int tablero[MAX][MAX]{ };
    //inicializarTablero(tablero, F, C); 
    
    int tablero[MAX][MAX]{ };
    inicializarTablero2(tablero, F, C);
    imprimirTablero(tablero, F);
    
    if (F == C){
        if (ordenarAnaquel(tablero, F, P)){
            imprimirTablero(tablero, F);
            cout << "Si es posible ordenar los productos en el anaquel" << endl;
        }else{
            cout << "No es posible utilizar este anaquel para la cantidad de productos solicitados" << endl;
        }
    }else{
        cout << "No se puede procesar tablero de entrada porque no es un cuadrado" << endl;
    }
    return 0;
}

