/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"


void mmu_init();

uint32_t mmu_nextFreeKernelPage();

void mmu_mapPage(uint32_t cr3, uint32_t virtual, uint32_t phy, uint32_t attrs);

void mmu_unmapPage(uint32_t cr3, uint32_t virtual);

uint32_t mmu_initKernelDir();

uint32_t mmu_nextFreePage();

uint32_t mmu_initTaskDir(uint32_t virtual, uint32_t tarea,uint32_t posInicial_x, uint32_t posInicial_y);

/* Struct de una entrada de la PDE */
typedef struct str_pd_entry {
    uint32_t present:1;
    uint32_t r_w:1;
    uint32_t u_s:1;
    uint32_t pwt:1;
    uint32_t pcd:1;
    uint32_t a:1;
    uint32_t ignored:1;
    uint32_t ps:1;
    uint32_t g:1;
    uint32_t disponible:3;
    uint32_t dir_base:20;
} __attribute__((__packed__, aligned (4))) pd_entry;


/* Struct de una entrada de la PTE */
typedef struct str_pt_entry {
    uint32_t present:1;
    uint32_t r_w:1;
    uint32_t u_s:1;
    uint32_t pwt:1;
    uint32_t pcd:1;
    uint32_t a:1;
    uint32_t d:1;
    uint32_t pat:1;
    uint32_t g:1;
    uint32_t disponible:3;
    uint32_t dir_base:20;
} __attribute__((__packed__, aligned (4))) pt_entry;



#endif	/* !__MMU_H__ */




