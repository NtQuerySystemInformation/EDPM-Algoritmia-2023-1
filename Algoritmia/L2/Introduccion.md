# Entendiendo recursion a traves de lenguaje ensamblador y ingenieria reversa.

## Introduccion

Esto lo hago debido a que las explicaciones de los profesores en relacion a la manipulacion de la pila omiten detalles particularmente interesantes en relacion a la ABI,  lo cual convierte el planteamiento de resolucion de problemas recursivos extremedamente confuso y poco intuitivo desde mi perspectiva como alguien acostumbrado a hacer ingenieria reversa como pasatiempo.

Primero comencemos explicando los principales componentes de una funcion recursiva, y luego veamos un ejemplo clasico durante las clases de Algoritmia en el cual estos conceptos se pueden aplicar.

Cabe recalcar que este binario fue generado usando el compilador GCC (el que usamos en Netbeans y DevC++ generalmente). 
Asimismo, optimize los simbolos de los nombres de las funciones (debido al name mangling).

`IMPORTANTE: Es recomendable haber tenido una experiencia previa con x86/x64 y sobre como las funciones trabajan en ensamblador (stack frames),
de lo contrario, la explicacion a continuacion podria ser incluso mas complicada de lo normal.
En cualquier manera, tratare de hacerlo lo mas intuitivo posible, brindando los conceptos necesarios para entender mi perspectiva.`


## Principales detalles de funcion recursiva para resolucion de problemas

| Componentes    |      Descripcion              |
|----------------|-------------------------------|
| Caso base (base case)| `Este es el caso donde la funcion recursiva termina su iteracion.`|
| Argumentos/Parametros |`Estos pueden ser o no ser actualizados durante cada llamada, eso depende del problema`|

# Planteando el problema.

Empecemos describiendo un problema simple: Laboratorio 2015-2 Pregunta 1.
Esto nos plantea generar el siguiente grafico, usando una unica funcion recursiva triangulo, con argumentos 3 y 5.

## Analizando main y el progreso de la funcion recursiva estaticamente y dinamicamente

Si bien podria sonar como una buena idea el describir como encontre esta funcion en el ejecutable (lo cual no es importante desde la perspectiva de un programador), 
considero que es mejor concentrarnos en QUE cosa esta haciendo cada una de las funciones por si mismas.

Antes de que me preguntes que cosas estan pasando aqui, solo tienes que saber lo siguiente (Puedes investigar el resto por tu cuenta y ver videos al respecto):
(Todo esto probablemente es mas entendible si ya has llevado OAC, yo no lo he llevado porque me he atrasado haciendo otras cosas y me he rehuso a llevar ED, lol.)

```
	0.-La pila (o stack) es una estructura de datos FIFO (first in, first out). 
	
	1.-Para preparar el contexto de una funcion, es decir, cuando inicia y acaba una funcion, se emplea lo que se denomina funcion prologue y function epilogue.
		->Para la funcion triangulo aqui particularmente el prologue va desde 'push ebp' hasta 'sub esp, 18h', mientras que su epilogue empieza con 'leave' y 'retn'.
		->Este aspecto se entendera mas adelante para entender como los mecanismos recursivos funcionan.
```

Empecemos visualizando el codigo de lenguaje ensamblador.

Funcion Main:

```
(...)
mov     dword ptr [esp+4], 5 ; segundo argumento.
mov     dword ptr [esp], 3 ; primer argumento
call    triangulo	; equivalente a triangulo(3,5)
(...)
```

Funcion triangulo:
```
push    ebp
mov     ebp, esp
sub     esp, 18h
mov     eax, [ebp+a]
cmp     eax, [ebp+b]
jg      short FinalizandoRecursion
mov     eax, [ebp+a]
mov     [esp], eax      ; a
call    imprimirEstrellas
mov     eax, [ebp+a]
lea     edx, [eax+1]
mov     eax, [ebp+b]
mov     [esp+4], eax    ; b
mov     [esp], edx      ; a
call    triangulo ; 
mov     eax, [ebp+a]
mov     [esp], eax      ; a
call    imprimirEstrellas
FinalizandoRecursion:
nop
leave
retn
```
