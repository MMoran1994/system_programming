/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/


#ifndef __SCHED_H__
#define __SCHED_H__

#include "stdint.h"
#include "screen.h"
#include "tss.h"
#include "defines.h"



void sched_init();

int16_t sched_nextTask();

void reset_screen();

typedef struct str_sched 
{

	int pos_x;
	int pos_y;
	uint8_t is_alive;
	uint16_t tss_selector;


}__attribute__((__packed__, aligned (8))) sched;

typedef struct str_portal 
{

	int pos_x;
	int pos_y;

}__attribute__((__packed__, aligned (8))) _portal;

typedef struct str_data_ini_tarea 
{

	uint32_t codigo_fuente;
	uint32_t *p_tss;
	uint8_t tag;

}__attribute__((__packed__, aligned (8))) data_tarea;


extern sched sched_task[8];

extern void jump_toIdle();

void print_exception(uint32_t num_excepcion);
void imprimir_registros(uint32_t eip, uint32_t eflags,uint16_t ss,uint16_t gs,uint16_t fs,uint16_t es,uint16_t ds,uint16_t cs,uint32_t edi,uint32_t esi,uint32_t ebp,uint32_t esp,uint32_t ebx,uint32_t edx,uint32_t ecx, uint32_t eax,uint32_t stack1,uint32_t stack2,uint32_t stack3,uint32_t stack4,uint32_t stack5 );
void activar_modo_debug();
int32_t verificar_modo_debug();
int whereIsMorty_posY();
int whereIsMorty_posX();
uint32_t portalCreat_check();
void killcurrent_task();
void portalCreatReset();
uint32_t int137(int desp_x,int desp_y,uint32_t cross,uint32_t withMorty);
void iniciar_pantalla();
int32_t modo_debug;
void activar_desactivar_excepcion();
int32_t verificar_excepcion();
int32_t verificar_fin_del_juego();
int hacer_resto_positivo(int x, int divisor);

void getRandomPosicion(uint8_t index, uint8_t *x, uint8_t *y);

#endif	/* !__SCHED_H__ */
