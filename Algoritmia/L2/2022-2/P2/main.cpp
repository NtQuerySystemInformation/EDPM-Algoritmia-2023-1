#include <iostream>
#include <cmath>
#include <fstream>
#define MAX 100

using namespace std;

void inicializarCampo(int campo[][MAX], int n, int m){
    ifstream archIn; 
    archIn.open("Campo.txt", ios::in);
    if (not archIn.is_open()){
        cout << "No se pudo abrir el archivo Campo";
        exit(1);
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            archIn >> campo[i][j];
        }
    }
    archIn.close();
}

//Pendiente por programar: pero basicamente verificas para las posiciones descritas abajo.
void deteccionMinasEnPosicion(int campo[][MAX], int i, int j, int n, int m)
{
    
}

/*
    Explicacion de solucion:
    -En un inicio pensaba que lo ideal era hacer backtracking para ir comparando los valores de la derecha,
    izquierda y diagonal, pero al final me di cuenta que esto era una perdida de tiempo, y no era la perspectiva correcta
    para la solucion.
 
    -Lo unico que falta por implementar es el verificador de minas por cada posicion posible, pero eso es facil
    simplemente verificas para campo[posX][posY] == 1, donde posX puede ser: x+1, x-1 y x, mientras que posY es y + 1, y, y-1.
 
 */
void buscaPasos(int campo[][MAX],int  i, int j, int n, int m,
                int endX, int endY, int currPasos, int& endResult)
{
    if (i == endY and j == endX){
        if (endResult > currPasos || endResult == -1){
            endResult = currPasos;
        }
        return;
    }
    deteccionMinasEnPosicion(campo, i, j, n, m);
    if (i + 1 < n and campo[i+1][j+1] == 0){
        buscaPasos(campo, i + 1, j, n, m, endX, endY, currPasos + 1, endResult);
    }
    if (j + 1 < m and campo[i][j+1] == 0){
        buscaPasos(campo, i, j + 1, n, m, endX, endY, currPasos + 1, endResult);
    }
    if (i + 1 < n && j + 1 < m and campo[i+1][j+1] == 0){
        buscaPasos(campo, i + 1, j + 1, n, m, endX, endY, currPasos + 1, endResult);
    }    
}

void imprimirCampo(int campo[][MAX], int n, int m){
    cout << "Impresion campo: " << endl;
    char c;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (campo[i][j] == 1){
                c = '*';
            }else{
                c = ' ';
            }
            cout << c << " | ";
        }
        cout << endl;
    }
}


int main(void) {
    int campo[MAX][MAX], n, m, endX, endY, x = 0, y = 0, currPasos = 0, cantPasos = -1;
    cout << "Inicializar n y m: ";
    cin >> n >> m;
    inicializarCampo(campo, n, m);
    imprimirCampo(campo, n, m);
    cout << "Inicializa hasta donde llegar (y,x): ";
    cin >> endY >> endX;
    buscaPasos(campo, x, y, n, m, endX, endY, currPasos, cantPasos);
    if (cantPasos > 0){
        cout << "El robot llega al punto final en " << cantPasos << " pasos" << endl;
    }else{
        cout << "No es posible llegar el punto final" << endl;
    }
    return 0;
}

