#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_COMB 16

struct Piso
{
    int s[4];
    int cantSecciones;
};

struct Bloques {
    struct Piso p[2];	
};

struct Congelador{
    struct Bloques b[2];
};


struct PisoPrecio
{
    int s[4];
    int cantSecciones;
};

struct BloquesPrecio{
    struct PisoPrecio p[2];	
};

struct CongeladorPrecios{
    struct BloquesPrecio b[2];
};



void inicializarCongelador(struct Congelador* tmp, struct CongeladorPrecios* tmp2){
    int N;
    printf("Inserte el numero de secciones: ");
    scanf("%d", &N);
    if (N > 4){
        printf("No se puede tener mas de 4 secciones\n");
        exit(1);
    }
    FILE* archCong = fopen("Congelador.txt","r");
    if (archCong == NULL){
        printf("No se pudo abrir el archivo para inicializar Congelador\n");
        exit(1);
    }
    FILE* archCong2 = fopen("Congelador2.txt", "r");
    if (archCong2 == NULL){
        printf ("No se pudo abrir el archivo para inicializar los precios\n");
        exit(1);
    }
    /*
		tmp->b[0].p[0].cantSecciones = N;
		tmp->b[0].p[1].cantSecciones = N;
		tmp->b[1].p[0].cantSecciones = N;
		tmp->b[1].p[1].cantSecciones = N;
	*/
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			tmp->b[i].p[j].cantSecciones = N;	
		}
	}

	/*
		tmp2->b[0].p[0].cantSecciones = N;
		tmp2->b[0].p[1].cantSecciones = N;
		tmp2->b[1].p[0].cantSecciones = N;
		tmp2->b[1].p[1].cantSecciones = N;
	*/
	
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			tmp2->b[i].p[j].cantSecciones = N;	
		}
	}
    
	//Por cada Bloque 
    for (int i = 0; i < 2; i++)
    {
        scanf("Inicializando bloque %d...\n", i);
		//Por cada Piso
        for (int j = 0; j < 2; j++)
        {
			//Por cada seccion
            for (int k = 0; k < tmp->b[i].p[j].cantSecciones; k++)
            {
                //printf("Inserte el valor para [Bloque: %d, Piso: %d, Secciones: %d]: ", i, j, k);
                fscanf(archCong, "%d", &tmp->b[i].p[j].s[k]);
                if (feof(archCong)) break;
                printf("El valor para [Bloque: %d, Piso: %d, Secciones: %d]: %d\n", i, j, k, tmp->b[i].p[j].s[k]);
            }
        }
    }
    
    for (int i = 0; i < 2; i++)
    {
        scanf("Inicializando bloque de precios %d...\n", i);
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < tmp2->b[i].p[j].cantSecciones; k++)
            {
                //printf("Inserte el valor para [Bloque: %d, Piso: %d, Secciones: %d]: ", i, j, k);
                fscanf(archCong2, "%d", &tmp2->b[i].p[j].s[k]);
                if (feof(archCong2)) break;
                printf("El precio [Bloque: %d, Piso: %d, Secciones: %d] es %d\n", i, j, k, tmp2->b[i].p[j].s[k]);
            }
        }
    }
    fclose(archCong);
    fclose(archCong2);
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
void obtenerIndices(int j, int MaxComb, int* bdx, int* pdx, int* sdx, int cantSecciones)
{
    int SepBloque = MaxComb/cantSecciones*2;
    if (j >= SepBloque){
        *bdx = 1;
    }else{
        *bdx = 0;
    }
    j = j % SepBloque;
    if (j >= SepBloque/2){
        *pdx = 1;
    }else{
        *pdx = 0;
    }
    j = j % cantSecciones;
    *sdx = j;
}


void imprimirSolucion(int idx, int* cromo, struct Congelador* tmp, int MaxComb){
    int bdx, pdx, sdx;
    int cantSecciones = tmp->b[0].p[0].cantSecciones;
    printf("Resultado %d: ", idx);
    for (int i = 0; i < MaxComb; i++){
        if (cromo[i] == 1){
            obtenerIndices(i, MaxComb, &bdx, &pdx, &sdx, cantSecciones);
            printf("%3dk", tmp->b[bdx].p[pdx].s[sdx]);
        }
    }
    printf("%18s", "Ubicaciones:");
     for (int i = 0; i < MaxComb; i++){
        if (cromo[i] == 1){
            obtenerIndices(i, MaxComb, &bdx, &pdx, &sdx, cantSecciones);
            char bloque = (bdx == 0) ? 'I' : 'D';
            char piso = (pdx == 0) ? 'A' : 'B';
            printf(" %c%c%d ", bloque, piso, sdx + 1);
        }
    }
    printf("\n");
}

//J representa la posicion del 1 dentro del cromo, es la ubicacion de la permutacion actual.
int main(int argc, char** argv){

    int kilosPed, bdx, pdx, sdx;
    struct Congelador tmp;
    struct CongeladorPrecios tmp2;
    inicializarCongelador(&tmp, &tmp2);
    
    printf("Ingrese la cantidad de peso a pedir:");
    scanf("%d", &kilosPed);
    int cantSecciones = tmp.b[0].p[0].cantSecciones;
    int MaxComb = 4*cantSecciones; //2 * 2 * numero de secciones.
    int comb = (int)pow(2, MaxComb);
    int* cromo = malloc(sizeof(int)*MaxComb);
    int* cromoMax = malloc(sizeof(int) * MaxComb);

    int currGananciaPeso, currPrecioPeso;
    int maxPrecioPeso = 0;
    for (int i = 0; i < comb; i++){
        currGananciaPeso = 0;
        currPrecioPeso = 0;
        generarBin(i, MaxComb, cromo);
        for (int j = 0; j < MaxComb; j++){
            if (cromo[j] == 1){
                obtenerIndices(j, MaxComb, &bdx, &pdx, &sdx, cantSecciones);
                currGananciaPeso += tmp.b[bdx].p[pdx].s[sdx];
                currPrecioPeso += tmp2.b[bdx].p[pdx].s[sdx];
            }
        } 
        if (currGananciaPeso <= kilosPed){
            if (currPrecioPeso > maxPrecioPeso){
                //printf("Ganancia:%d\n", maxPrecioPeso);
                maxPrecioPeso = currPrecioPeso;
                for (int i = 0; i < MaxComb; i++){
                    cromoMax[i] = 0;
                }
                for (int i = 0; i < MaxComb; i++){
                    cromoMax[i] = cromo[i];
                }
            }
        }
    }
    printf("Ganancia:%d %15s", maxPrecioPeso, "Resultado: ");
    for (int j = 0; j < MaxComb; j++){
        if (cromoMax[j] == 1){
            obtenerIndices(j, MaxComb, &bdx, &pdx, &sdx, cantSecciones);
            printf(" %dk ", tmp.b[bdx].p[pdx].s[sdx]);
        }
    }
    printf("%18s", "Ubicaciones:");
    for (int i = 0; i < MaxComb; i++){
        if (cromoMax[i] == 1){
            obtenerIndices(i, MaxComb, &bdx, &pdx, &sdx, cantSecciones);
            char bloque = (bdx == 0) ? 'I' : 'D';
            char piso = (pdx == 0) ? 'A' : 'B';
            printf(" %c%c%d ", bloque, piso, sdx + 1);
        }
    }
    printf("\n");
    free(cromo);
    free(cromoMax);
    
    return 0;
}