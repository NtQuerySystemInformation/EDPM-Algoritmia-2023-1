# Entendiendo recursion a traves de lenguaje ensamblador y ingenieria reversa.

## Introduccion

Esto lo hago debido a que las explicaciones de los profesores en relacion a la manipulacion de la pila omiten detalles particularmente interesantes en relacion a la ABI,  lo cual convierte el planteamiento de resolucion de problemas recursivos extremedamente confuso y poco intuitivo desde mi perspectiva como alguien acostumbrado a hacer ingenieria reversa como pasatiempo.

Primero comencemos explicando los principales componentes de una funcion recursiva, y luego veremos un ejemplo clasico durante las clases de Algoritmia en el cual estos conceptos se pueden aplicar. <br >

Cabe recalcar que este binario fue generado usando el compilador GCC (el que usamos en Netbeans y DevC++ generalmente, el cual es el mas comun en el curso).<br /> 
Asimismo, optimize los simbolos de los nombres de las funciones en el codigo de abajo (debido al name mangling).

`IMPORTANTE: Es recomendable haber tenido una experiencia previa con x86/x64 y sobre como las funciones trabajan en ensamblador (stack frames),
de lo contrario, la explicacion a continuacion podria ser incluso mas complicada de lo normal.<br />
En cualquier manera, tratare de hacerlo lo mas intuitivo posible, brindando los conceptos necesarios para entender mi perspectiva.`


## Principales detalles de funcion recursiva para resolucion de problemas

| Componentes    |      Descripcion              |
|----------------|-------------------------------|
| Caso base (base case)| `Este es el caso donde la funcion recursiva termina su iteracion.`|
| Argumentos/Parametros |`Estos pueden ser o no ser actualizados durante cada llamada, eso depende del problema`|

# Planteando el problema y la solucion propuesta:

Empecemos describiendo un problema simple: Laboratorio 2015-2 Pregunta 1. <br />
Esto nos plantea lo siguiente:

```
Problema: Usando una unica funcion recursiva, triangulo(3, 5), genere el siguiente grafico, teniendo en cuenta que a <= b en todo momento:

									* * *
									* * * *
									* * * * *
									* * * * *
									* * * * 
									* * *

```
Por otro lado, una de las soluciones del profesor se plantea de la siguiente manera:

```
						void triangulo(int a, int b){    
							if (a <= b){
								imprimirEstrellas(a);
								triangulo(a + 1, b);
								imprimirEstrellas(a);
							}
						}

```

¿Como exactamente funciona la pila en este sentido, para generar el grafico deseado?

## Analizando la solucion de la funcion recursiva en bajo nivel y a nivel de codigo fuente:

Primero que todo es importante considerar que y como los argumentos de la funcion recursiva cambian (a cambia y b no).
En este sentido, considerando el inicio de triangulo(3,5), tendremos en cuenta la llamada
hacia imprimirEstrellas(a+1,b).

```
llamada 2: a = 4, b = 5
llamada 3: a = 5, b = 5
llamada 5: a = 6, b = 5 
```

Claramente cuando la variable a llega hasta 6, la condicional "if (a <= b)" no se cumple, lo cual implicaria el fin de la recursion (caso base).
Pero que ocurre en el fin de la recursion como tal?.

Para esto es necesario explicar en bajo nivel como funcionan las cosas, especificamente QUE ocurre una vez llegado al caso base.

Debido a este motivo, es necesario destacar los siguientes conceptos (Puedes investigar el resto por tu cuenta y ver videos al respecto):<br />
(Todo esto probablemente es mas entendible si ya has llevado OAC, yo no lo he llevado porque me he atrasado haciendo otras cosas y me he rehuso a llevar ED, lol.)

```
	Conceptos:
	
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
call    triangulo	; equivalente en C a triangulo(3,5)
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
mov     [esp], eax      
call    imprimirEstrellas ; Equivalente en C a imprimirEstrellas(a);
mov     eax, [ebp+a]
lea     edx, [eax+1]
mov     eax, [ebp+b]
mov     [esp+4], eax    
mov     [esp], edx     
call    triangulo ; Equivalente en C a triangulo(a + 1, b)
mov     eax, [ebp+a]
mov     [esp], eax      
call    imprimirEstrellas ; Equivalente en C a imprimirEstrellas(a);

FinalizandoRecursion:
nop
leave
retn
```
De todas estas instrucciones, debemos concentrarnos en como la pila va cambiando a lo largo de cada llamda en bajo nivel,
por esta razon, nos enfocaremos en la instruccion "call". 

