/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

/* Atributos paginas */
/* -------------------------------------------------------------------------- */
#define PAG_P                   0x00000001
#define PAG_R                   0x00000000
#define PAG_RW                  0x00000002
#define PAG_S                   0x00000000
#define PAG_US                  0x00000004
#define PAGE_SIZE 				4096
#define PD_SIZE 				1024
#define PT_SIZE 				1024
/* Misc */
/* -------------------------------------------------------------------------- */
#define SIZE_N                  40 // X
#define SIZE_M                  80 // Y

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 35

#define GDT_IDX_NULL_DESC           0

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC << 3)


/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */
#define CS_CODIGO_KERNEL         0x0048
#define DS_DATA_KERNEL           0x0040

#define CS_CODIGO_USUARIO        0x005B  
#define DS_DATA_USUARIO          0x0053

#define SS_TAREA_INICIAL         0x0068
#define SS_TAREA_IDLE            0x0070

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR               0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                   0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                    0x000B8000 /* direccion fisica del buffer de video */

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE             0x08000000 /* direccion virtual del codigo */
#define RE_MAP_TASK_CODE	  0x08002000 /* Direccion virtual donde remapear las tareas */

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */
#define TASK_DIR_RICKC137       0X00010000
#define TASK_DIR_MORTYC137      0X00012000
#define TASK_DIR_RICKD248      0X00014000
#define TASK_DIR_MORTYD248      0X00016000
#define TASK_DIR_CRONENBERG     0X00018000

#define INICIO_MUNDO_CRONENBERG 0x400000


/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR          0x00027000
#define KERNEL_PAGE_TABLE_0      0x00028000
#define CRYSTALS_MAP             0x00029000
#define INICIO_DE_PAGINAS_LIBRES 0x100000
#define INICIO_DE_PAGINAS_LIBRES_TAREAS 0x400000

#endif  /* !__DEFINES_H__ */
