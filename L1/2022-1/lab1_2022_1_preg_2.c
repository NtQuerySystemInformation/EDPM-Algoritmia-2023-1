#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define MAX_CURSOS 20

struct Curso
{
	int idCurso;
	int cantCreditos;
	int horario;
	int diaSesionTeoria;
	int horaInicioTeoria;
	int horaFinTeoria;
	int diaSesionPractica;
	int horaInicioPractica;
	int horaFinPractica;
	int evaluacionSemana;
};


void InicializarCursos(struct Curso arrCursos[], int* cantCursos)
{
    FILE* archCursos = fopen("Cursos.txt", "r");
    if (arrCursos == NULL){
        printf("No se pudo abrir el archivo Cursos.txt");
        exit(1);
    }
    int n = 0;
    while (1){
        fscanf(archCursos, "%6d", &arrCursos[n].idCurso);
        if (feof(archCursos)) break;
        fscanf(archCursos, "%4d %d %d %2d %2d %d %2d %2d %d", 
                &arrCursos[n].horario,
                &arrCursos[n].cantCreditos,
                &arrCursos[n].diaSesionTeoria,
                &arrCursos[n].horaInicioTeoria,
                &arrCursos[n].horaFinTeoria,
                &arrCursos[n].diaSesionPractica,
                &arrCursos[n].horaInicioPractica,
                &arrCursos[n].horaFinPractica,
                &arrCursos[n].evaluacionSemana
        );
        n++;
    }
    *cantCursos = n;
    fclose(archCursos);	
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

/*
    Criterios para fuerza bruta.
            1.-No presentan cruces. 
            2.-Al menos 12 creditos (Facil, ya implementado)
            3.-Cada curso debe tener un ID distinto. (Tambien relativamente facil, pero falta resolverlo asi).
*/

int RangoHoraEncontradoPar(struct Curso* arrCursos, int i, int j, int OptionMixed)
{
	int duracionTeoriaJ = arrCursos[j].horaFinTeoria - arrCursos[j].horaInicioTeoria;
	int duracionTeoriaI = arrCursos[i].horaFinTeoria - arrCursos[i].horaInicioTeoria;
	int horaInicioTeoriaI = arrCursos[i].horaInicioTeoria;
	int horaInicioTeoriaJ = arrCursos[j].horaInicioTeoria;
	int duracionPracticaI = arrCursos[i].horaFinPractica - arrCursos[i].horaInicioPractica;
	int duracionPracticaJ = arrCursos[j].horaFinPractica - arrCursos[j].horaInicioPractica;
	int horaInicioPracticaI = arrCursos[i].horaInicioPractica;
	int horaInicioPracticaJ = arrCursos[j].horaInicioPractica;
	
	//Verifica teoriaI
	for (int k = horaInicioTeoriaI; k < (horaInicioTeoriaI + duracionTeoriaI); k++)
	{
            //arrCursos[i].diaSesionTeoria == arrCursos[j].diaSesionTeoria
            if (OptionMixed == 1){
                for (int m = horaInicioTeoriaJ; m < (horaInicioTeoriaJ + duracionTeoriaJ); m++){
                    if (k == m){
                        return 0;
                    }
                }
            }
            //arrCursos[i].diaSesionTeoria == arrCursos[j].diaSesionPractica
            if (OptionMixed == 2){
                for (int z = horaInicioPracticaJ; z < (horaInicioPracticaJ + duracionPracticaJ); z++){
                    if (z == k){
                        return 0;
                    }
                }
            }
	}
        
	for (int k = horaInicioPracticaI; k < (horaInicioPracticaI + duracionPracticaI); k++)
	{
		//arrCursos[i].diaSesionPractica == arrCursos[j].diaSesionPractica
		if (OptionMixed == 0){
			for (int m = horaInicioPracticaJ; m < (horaInicioPracticaJ + duracionPracticaJ); m++){
				if (k == m){
					return 0;
				}
			}
		}
		//arrCursos[i].diaSesionPractica == arrCursos[j].diaSesionTeoria
		if (OptionMixed == 3){
			for (int z = horaInicioTeoriaJ; z < (horaInicioTeoriaJ + duracionTeoriaJ); z++){
				if (k == z){
					return 0;
				}
			}
		}
	}
	
        return 1;
}

int VerificacionSemana(struct Curso* arrCursos, int i, int j){
    if (arrCursos[i].evaluacionSemana == arrCursos[j].evaluacionSemana ||
        arrCursos[i].evaluacionSemana == 3 || arrCursos[j].evaluacionSemana == 3){
        return 0;
    }
    return 1;
}

int VerificacionParCursos(struct Curso* arrCursos, int i, int j){
    int isValid = 1;
    if (arrCursos[i].idCurso == arrCursos[j].idCurso ||
        (arrCursos[i].diaSesionPractica == arrCursos[j].diaSesionPractica 
            && RangoHoraEncontradoPar(arrCursos, i, j, 0) == 0 
            && VerificacionSemana(arrCursos, i, j) == 0) ||
        (arrCursos[i].diaSesionTeoria == arrCursos[j].diaSesionTeoria
            && RangoHoraEncontradoPar(arrCursos, i, j, 1) == 0) ||
        (arrCursos[i].diaSesionPractica == arrCursos[j].diaSesionTeoria 
            && RangoHoraEncontradoPar(arrCursos, i, j, 3) == 0) ||
        (arrCursos[i].diaSesionTeoria == arrCursos[j].diaSesionPractica 
            && RangoHoraEncontradoPar(arrCursos, i, j, 2) == 0))
    {
        isValid = 0;
    }
    return isValid;
}

int VerificarCombinacionCurso(int* cromo, int cantCursos, struct Curso* arrCursos, double* creditosCurso, double L)
{
    double creditosCursos = 0;
    for (int j = 0; j < cantCursos; j++)
    {
        creditosCursos += arrCursos[j].cantCreditos * cromo[j];
    }
    
	if (creditosCursos >= 12 && creditosCursos <= L){
        *creditosCurso = creditosCursos;
    }else{
        return 0;
    }
	
	//Comparas en pares para verificar.
    for (int i = 0; i < cantCursos; i++){
        for (int j = 0; j < cantCursos; j++){
                //Se usa el curso en la combinacion, ademas de verificar que no es el curso.
                if (cromo[i] == 1 && cromo[j] == 1 && i != j){
                    if (VerificacionParCursos(arrCursos, i, j) == 0){
                        return 0;
                    }
                }
        }
    }
    return 1;	
}

void imprimirSolucion(int* cromo, int n, struct Curso* arrCursos)
{
    for (int j = 0; j < n; j++){
        if (cromo[j] == 1){
            printf("%05d-%03d\n", arrCursos[j].idCurso, arrCursos[j].horario);
        }
    }
}


int main(int argc, char** argv)
{
    int cantCursos = 0;
    struct Curso arrCursos[MAX_CURSOS];
    InicializarCursos(arrCursos, &cantCursos);

    double L;
    printf("Indique el maximo de creditos a matricular: ");
    scanf("%lf", &L);

    int comb = (int)pow(2, cantCursos);
    printf("La cantidad de cursos leidos es: %d\n", cantCursos);
    int cromo[cantCursos];

    double maximoCreditosCurso = 0;
    int maxCombinacionN = 0;
    for (int i = 0; i < comb; i++)
    {
        double creditosCursos = 0;
        generarBin(i, cantCursos, cromo);
        //Si la combinacion no cumple los requisitos del problema, 
        //entonces directamente se ignora y se procede con la siguiente combinacion.
        //El error probablemente esta aqui en los criterios de verificacion.
        if(VerificarCombinacionCurso(cromo, cantCursos, arrCursos, &creditosCursos, L) == 0){
            continue;
        }
        //Update only when it is more, and update is required.
        if (creditosCursos > maximoCreditosCurso){
            maxCombinacionN = i;
            maximoCreditosCurso = creditosCursos;
        }	
    }
    generarBin(maxCombinacionN, cantCursos, cromo);
    imprimirSolucion(cromo, cantCursos, arrCursos);
    system("pause");
    return 0;
}
