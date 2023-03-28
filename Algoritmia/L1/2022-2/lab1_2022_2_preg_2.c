#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_GALLETAS 10

int ContarActivos(int* cromo){
    int cantAct = 0;
    for (int i = 0; i < MAX_GALLETAS; i++){
        if (cromo[i] == 1){
            cantAct++;
        }
    }
    return cantAct;
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

void imprimirActivos(int* ver, char* paquete){
    printf("Posiciones activas: ");
    for (int i = 0; i < MAX_GALLETAS; i++)
    {
        if (ver[i] == 1){
            printf("(%d: %c) ", i + 1, paquete[i]);
        }
    }
    printf("\n");
}

int VerificandoTamano(int* PaquetePos, char* palBuscar)
{
    int cantPal = 0, isValid = 0;
    for (int i = 0; i < MAX_GALLETAS; i++){
        if (PaquetePos[i] == 1){
            cantPal++;
        }
        if (cantPal == strlen(palBuscar)){
            isValid = 1;
            break;
        }
    }
    return isValid;
}

int VerificandoPosLetrasAct(int* BuscarPos, char* palBuscar)
{
    int isValid = 0;
    int cantAct = 0;
    for (int j = 0; j < strlen(palBuscar); j++){
        if (BuscarPos[j] == 1){
            cantAct++;
        }
        if (cantAct == strlen(palBuscar)){
            isValid = 1;
            break;
        }
    }
    return isValid;
}


void EsPalabraValida(int* cromo, char* palBuscar, char* paquete)
{
    int PaquetePos[MAX_GALLETAS] = { 0 };
    int* BuscarPos = (int*)malloc(sizeof(int) * strlen(palBuscar)); 
    for (int i = 0; i < strlen(palBuscar); i++){
        BuscarPos[i] = 0;
    }
    for (int i = 0; i < MAX_GALLETAS; i++)
    {
        for (int j = 0; j < strlen(palBuscar); j++)
        {
            //Bit de posicion de permutacion.
            if (cromo[i] == 1){
                if (palBuscar[j] == paquete[i])
                {
                    PaquetePos[i] = 1;
                    if (BuscarPos[j] == 0){
                        BuscarPos[j] = 1;
                        break;
                    //Si ya se ha seteado, entonces ignoras posicion.
                    }else{ 
                        continue;
                    }
                }
            }
        }
    }
    if (VerificandoTamano(PaquetePos, palBuscar) == 1 && VerificandoPosLetrasAct(BuscarPos, palBuscar) == 1){
        imprimirActivos(PaquetePos, paquete);
    }
    free(BuscarPos);
}

//Esta solucion solo resuelve para palabra de entrada que no repite letras.

int main(int argc, char** argv)
{
    char paquete[MAX_GALLETAS];
    char palBuscar[MAX_GALLETAS];

    printf("Ingrese el paquete:");
    scanf("%s", paquete);

    printf("Ingrese la palabra a buscar:");
    scanf("%s", palBuscar);

    int n = (int)pow(2, MAX_GALLETAS);
    int cromo[MAX_GALLETAS];

    //Bucle de combinatorio.
    for (int i = 0; i < n; i++)
    {
            generarBin(i, MAX_GALLETAS, cromo);
            int numActComb = ContarActivos(cromo);
            if (numActComb == strlen(palBuscar)){
                //if (EsPalabraValida(cromo, palBuscar, paquete) == 1)
                EsPalabraValida(cromo, palBuscar, paquete);
                
            }

    }
    return 0;
}