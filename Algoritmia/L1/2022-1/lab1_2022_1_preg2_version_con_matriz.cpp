#include <fstream>
#include <cmath>
#include <iostream>
#include <codecvt>
#define MAX_CURSOS 20
#define MIN_CRED 12
using namespace std; 

struct Semanas{
    int dias[7][24]{ }; 
};

struct Horario
{
    int arrIDs[MAX_CURSOS]{ };
    Semanas arrSemanas[2];
};

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

void InicializarCursos(struct Curso arrCursos[], int& cantCursos)
{
    ifstream archCursos;
    archCursos.open("Cursos.txt", ios::in);
    if (not archCursos.is_open()){
        printf("No se pudo abrir el archivo Cursos.txt");
        exit(1);
    }
    int n = 0;
    while (1){
        archCursos >> arrCursos[n].idCurso;
        if (archCursos.eof()) break;
        archCursos >> arrCursos[n].horario >> arrCursos[n].cantCreditos >> arrCursos[n].diaSesionTeoria
                   >> arrCursos[n].horaInicioTeoria
                   >> arrCursos[n].horaFinTeoria
                   >> arrCursos[n].diaSesionPractica
                   >> arrCursos[n].horaInicioPractica
                   >> arrCursos[n].horaFinPractica
                   >> arrCursos[n].evaluacionSemana;
        n++;
    }
    cantCursos = n;
    archCursos.close();
}

bool addHoursToDay(int inicio, int fin, int horario[][24], int dia)
{
    bool ret = true;
    for (int hora = inicio; hora < fin; hora++){
        if (horario[dia][hora] == 1){
            ret = false;
            return ret;
        }else{
            horario[dia][hora] = 1;
        }
    }
    return ret;
}
//Si bien agregas entre el rango de horas, tambien lo agregas en dias especificos....
bool agregarAmbasSemanas(int inicio, int fin, int dia, Semanas semana[]){
    bool ret;
    //Loopea sobre tipos de 2 semanas: Impar/par, el el dia pasado como argumento.
    for (int i = 0; i < 2; i++){
        ret = addHoursToDay(inicio, fin, semana[i].dias, dia);
        if (not ret) break;
    }
    return ret;
}
bool agregarHorasPractica(Curso arr[], int idx, Horario& horario){
    int jdx = 0;
    switch(arr[idx].evaluacionSemana){
        case 1:{
            jdx = 0;
            break;
        }
        case 2:{
            jdx = 1;
            break;
        }
        case 3:{
            jdx = 2;
            break;
        }
    }
    bool ret1;
    if (jdx == 2){
        //agregar a ambas semanas.
        ret1 = agregarAmbasSemanas(arr[idx].horaInicioPractica, arr[idx].horaFinPractica, arr[idx].diaSesionPractica, horario.arrSemanas);
    }else{
        //Agregar a semana especifica (Par o impar)
        ret1 = addHoursToDay(arr[idx].horaInicioPractica, arr[idx].horaFinPractica, horario.arrSemanas[jdx].dias, arr[idx].diaSesionPractica);
    }
    return ret1;
}

//Pregunta principal para la resolucion del problema:
//1.-Porque se utiliza [7][24] en la matriz anterior?
//      -Revisando la logica, basicamente se crea bits por cada dia (24 horas), para una semana (7 dias)
//      -Por cada tipo de semana (impar, par), se crea una estructura Semana (en este caso solo serian 2).
//      -Luego se intenta setear cada bit, para cada semana, para cada dia.
//      -Si anteriormente se ha sido seteado ese bit en particular, entonces directamente descartamos el horario.

bool agregarVerifCurso(int bitIds[], int id){
    bool result = false;
    if (bitIds[id] == 1){
        return result;
    }else{
        bitIds[id] = 1;
        result = true;
    }
    return result;
}

bool VerificarCursos(Curso arrCur[], int cromo[], int n, double creditosCursos, double L){
    Horario horario;
    bool resState1, resState2, resState3;
    if (creditosCursos >= 12 && creditosCursos <= L){
        for (int i = 0; i < n; i++){
            if (cromo[i] == 1){
                resState3 = agregarVerifCurso(horario.arrIDs, arrCur[i].idCurso);
                //Añade solo bits para teoria
                resState1 = agregarAmbasSemanas(arrCur[i].horaInicioTeoria, arrCur[i].horaFinTeoria, arrCur[i].diaSesionTeoria, horario.arrSemanas);
                //Añade solo bits para practica, dependiendo de las necesidades del curso.
                resState2 = agregarHorasPractica(arrCur, i, horario);
                if (not resState1 || not resState2 || not resState3){
                    return false;
                }
            }
        }
    }else{
        return false;
    }
    return true;
}

void imprimirSolucion(int* cromo, int n, struct Curso* arrCursos)
{
    for (int j = 0; j < n; j++){
        if (cromo[j] == 1){
            printf("%05d-%03d\n", arrCursos[j].idCurso, arrCursos[j].horario);
        }
    }
}

int main(int argc, char** argv) {
    Curso arrCurso[MAX_CURSOS];
    int cantCursos;
    InicializarCursos(arrCurso, cantCursos);
    
    int comb = (int)pow(2, cantCursos);
    int cromo[cantCursos];
    double maximoCreditosCurso = 0;
    int maxCombinacionN = 0;
    double L;
    cout << "La cantidad de cursos es: " << cantCursos<< endl;
    cout << "Ingresar el maximo de creditos a calcular: ";
    cin >> L;
    
    for (int i = 0; i < comb; i++){
        double creditosCursos = 0;
        generarBin(i, cantCursos, cromo);
        for (int j = 0; j < cantCursos; j++){
            if (cromo[j] == 1){
                creditosCursos += arrCurso[j].cantCreditos;
            }
        }
        if (!VerificarCursos(arrCurso, cromo, cantCursos, creditosCursos, L)){
            continue;
        }
        if (creditosCursos > maximoCreditosCurso){
            maxCombinacionN = i;
            maximoCreditosCurso = creditosCursos;
            //if (creditosCursos == L){
            //    cout << "Solucion max credito: " << i << endl;
            //    imprimirSolucion(cromo, cantCursos, arrCurso);
            //    cout << "Fin solucion: "<< i << endl;
            //}
        }	
        //A partir de aqui podemos copiar y pegar de la version 1.
    }
    generarBin(maxCombinacionN, cantCursos, cromo);
    imprimirSolucion(cromo, cantCursos, arrCurso);
    
    //delete[] arrCurso;
    return 0;
}

