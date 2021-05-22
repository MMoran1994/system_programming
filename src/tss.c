/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_initial;
tss tss_idle;
tss tss_RickC137;
tss tss_RickD248;
tss tss_MortyC137;
tss tss_MortyD248;
tss tss_Cronenberg1;
tss tss_Cronenberg2;
tss tss_Cronenberg3;

void tss_init() {
	tss_idle = (tss) {0};
	tss_idle.eip = 0x1A000;
	tss_idle.cr3 = KERNEL_PAGE_DIR;
	tss_idle.esp0 = KERNEL_PAGE_DIR;
	tss_idle.esp = KERNEL_PAGE_DIR;
	tss_idle.cs = CS_CODIGO_KERNEL;
	tss_idle.ds = DS_DATA_KERNEL;
	tss_idle.eflags = 0x202;
	tss_idle.ss = DS_DATA_KERNEL;
	tss_idle.es = DS_DATA_KERNEL;
	tss_idle.ds = DS_DATA_KERNEL;
	tss_idle.fs = DS_DATA_KERNEL;
	tss_idle.gs = DS_DATA_KERNEL;

	tss_initial = (tss) {0};

}



void iniciar_tss(uint32_t tarea,tss* new_tss,uint32_t posInicial_x, uint32_t posInicial_y)
{
	
	*new_tss = (tss) {0};
	new_tss->eip = TASK_CODE;
	new_tss->cr3 = mmu_initTaskDir(TASK_CODE,tarea,posInicial_x,posInicial_y);
	new_tss->esp = TASK_CODE + 2*PAGE_SIZE;
	new_tss->esp0 = mmu_nextFreeKernelPage() + PAGE_SIZE;
	new_tss->ss0 = DS_DATA_KERNEL;
	new_tss->cs = CS_CODIGO_USUARIO;
	new_tss->ds = DS_DATA_USUARIO;
	new_tss->eflags = 0x202;
	new_tss->ss = DS_DATA_USUARIO;
	new_tss->es = DS_DATA_USUARIO;
	new_tss->ds = DS_DATA_USUARIO;
	new_tss->fs = DS_DATA_USUARIO;
	new_tss->gs = DS_DATA_USUARIO;
	new_tss->ebp = TASK_CODE + 2*PAGE_SIZE;
}

