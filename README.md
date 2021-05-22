# Trabajo Práctico: Programación de sistemas
## Objetivo

Este trabajo práctico consiste en un conjunto de ejercicios en los que se aplican de forma gradual
los conceptos de System Programming vistos en las clases teóricas y prácticas. Los ejercicios están
inspirados en la serie Rick y Morty, siendo el sistema que desarrollarán un multiverso en “memoria”,
donde el Rick Sánchez de la dimensión C-137 trata de detener a su alter ego Rick de la dimensión
D-248 para que no someta a los habitantes del mundo Cronenberg tomando sus mentes.
Se buscará construir un sistema mínimo que permita correr como máximo 25 tareas concurrentemente a nivel de usuario. Este sistema simulará un juego entre dos pares de tareas,Rick y Morty,
sobre un mundo donde se ubicarán estas y otro conjunto de tareas, denominadas tareas Cronenberg (o
habitantes Cronenberg). Las tareas serán ubicadas incialmente dentro del mundo de forma aleatoria.
Una vez que comience el juego, las tareas pueden vivir o morir, pero una vez muertas no reviven más
(suponemos que Rick no tiene más clones). Cada pares de jugadores Rick y Morty, tratará de tomar
la mayor cantidad de mentes posible. El juego terminará cuando alguno de los Ricks tenga todas
las mentes y el otro ninguna o alguno de los Ricks muera (ya sea porque su mente fue capturada
o porque mataron a Rick). Además, como un Rick no puede estar sin su Morty (por el tema de la
compensación), si su Morty llegará a estirar la pata, entonces el Rick en consecuencia también moriría.
Los Ricks utilizarán su arma de portales, tanto para moverse por el mundo Cronenberg, como para
capturar mentes.
Es importante recordar, que si bien este trabajo práctico esta ilustrado por un juego, los ejercicios
proponen utilizar los mecanismos que posee el procesador para la programación desde el punto de
vista del sistema operativo.

## Introducción

Para este trabajo se utilizará como entorno de pruebas el programa *Bochs*. El mismo permite simular
una computadora IBM-PC compatible desde el inicio, y realizar tareas de debugging. Todo el código
provisto para la realización del presente trabajo está ideado para correr en Bochs de forma sencilla.
Una computadora al iniciar comienza con la ejecución del POST y el BIOS, el cual se encarga
de reconocer el primer dispositivo de booteo. En este caso dispondremos de un *Floppy Disk* como
dispositivo de booteo. En el primer sector de dicho floppy, se almacena el boot-sector. El BIOS
se encarga de copiar a memoria 512 bytes del sector, a partir de la dirección 0x7C00. Luego, se
comienza a ejecutar el código a partir esta dirección. El boot-sector debe encontrar en el *floppy* el
archivo KERNEL.BIN y copiarlo a memoria. Éste se copia a partir de la dirección 0x1200, y luego se
ejecuta a partir de esa misma dirección. En la figura 2 se presenta el mapa de organización de la
memoria utilizada por elkernel.
Es importante tener en cuenta que el código del *boot-sector* se encarga exclusivamente de copiar
el kernel y dar el control al mismo, es decir, no cambia el modo del procesador. El código del *boot-
sector*, como así todo el esquema de trabajo para armar elkernel y correr tareas, es provisto por la
cátedra.
Los archivos a utilizar como punto de partida para este trabajo práctico son los siguientes:

```
− Makefile - encargado de compilar y generar el floppy disk.
− bochsrcybochsdbg - configuración para inicializar Bochs.
− diskette.img - la imagen delfloppyque contiene el *boot-sector* preparado para cargar el kernel.
− kernel.asm - esquema básico del código para el kernel.
− defines.hycolors.h - constantes y definiciones.
− gdt.hygdt.c - definición de la tabla de descriptores globales.
− tss.hytss.c - definición de entradas de TSS.
− idt.hyidt.c - entradas para la IDT y funciones asociadas como *idt_init* para completar entradas
en la IDT.
− isr.hyisr.asm - definiciones de las rutinas para atender interrupciones (Interrupt Service Routines).
− sched.hysched.c - rutinas asociadas alscheduler.
− mmu.hymmu.c - rutinas asociadas a la administración de memoria.
− screen.hyscreen.c - rutinas para pintar la pantalla.
− a20.asm - rutinas para habilitar y deshabilitar A20.
− print.mac - macros útiles para imprimir por pantalla y transformar valores.
− idle.asm - código de la tareaIdle.
− game.hygame.c - implementación de los llamados al sistema y lógica del juego.
− syscalls.h - interfaz a utilizar en C para los llamados al sistema.
− task.h - función de posiciones “random” para tareas.
− taskRickC137.cytaskMortyC137.c - código de las tareas del jugador C-137.
− taskRickD248.cytaskMortyD248.c - código de las tareas del jugador D-
− taskCronenberg.c - código de las tareas Cronenberg (son todas iguales).
− i386.h - funciones auxiliares para utilizar *assembly* desde C.
− pic.cypic.h - funciones *pic_enable*,*pic_disable*,*pic_finish1* y *pic_reset*.
```
Los ejercicios a desarrollar así también como las condiciones del trabajo se encuentran en el archivo `enunciado.pdf`. En el archivo `informe.pdf` se encuentra una explicación detallada de cada uno de los ejercicios, además de consideraciones de diseño e interpretaciones que tuvo el grupo con respecto a las consignas definidas.
