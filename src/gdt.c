/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"
#include "tss.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (uint16_t)    0x0000,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x00,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x00,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },//kernel
      [8] = (gdt_entry) {//data
        (uint16_t)    0x88FF,         /* limit[0:15]  */  //137MB
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x02,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    }, //kernel
      [9] = (gdt_entry) {//code
        (uint16_t)    0x88FF,         /* limit[0:15]  */ //173MB
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x0A,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },//usuario
      [10] = (gdt_entry) {//data
        (uint16_t)    0x88FF,         /* limit[0:15]  */ //137MB
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x02,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x03,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },//usuario
      [11] = (gdt_entry) {//code
        (uint16_t)    0x88FF,         /* limit[0:15]  */ //137MB
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x0A,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x03,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    [12] = (gdt_entry) {//area de la pantalla en  memoria
        (uint16_t)    0x0FFF,         /* limit[0:15]   */ //4KB (Tamano=Limite+1)
        (uint16_t)    0x8000,         /* base[0:15]   */
        (uint8_t)     0x0B,           /* base[23:16]  */
        (uint8_t)     0x0A,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    }
    

    
};


gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint32_t) &gdt
};


void gdt_tss_init() 
{
    
    gdt[13] = (gdt_entry) {//Tarea inicial
        (uint16_t)    0x0067,         /* limit[0:15]  */ //103B
        (uint16_t)    (uint32_t)&tss_initial,         /* base[0:15]   */
        (uint8_t)     ((uint32_t)&tss_initial >>16),           /* base[23:16]  */
        (uint8_t)     0x09,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     ((uint32_t)&tss_initial >>24),           /* base[31:24]  */
    };
    gdt[14] = (gdt_entry) {//Tarea Idle
        (uint16_t)    0x0067,         /* limit[0:15]  */ //103B
        (uint16_t)    (uint32_t)&tss_idle,         /* base[0:15]   */
        (uint8_t)     ((uint32_t)&tss_idle >>16),           /* base[23:16]  */
        (uint8_t)     0x09,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     ((uint32_t)&tss_idle >>24),           /* base[31:24]  */
    };

}

void gdt_tss()
{
    gdt[15] = (gdt_entry) {//Tarea tss_RickC137 
        (uint16_t)    0x0067,         /* limit[0:15]  */ //103B
        (uint16_t)    (uint32_t)&tss_RickC137,         /* base[0:15]   */
        (uint8_t)     ((uint32_t)&tss_RickC137 >>16),           /* base[23:16]  */
        (uint8_t)     0x09,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     ((uint32_t)&tss_RickC137 >>24),           /* base[31:24]  */
    };
    gdt[16] = (gdt_entry) {//Tarea tss_RickD248
        (uint16_t)    0x0067,         /* limit[0:15]  */ //103B
        (uint16_t)    (uint32_t)&tss_RickD248,         /* base[0:15]   */
        (uint8_t)     ((uint32_t)&tss_RickD248 >>16),           /* base[23:16]  */
        (uint8_t)     0x09,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     ((uint32_t)&tss_RickD248 >>24),           /* base[31:24]  */
    };
    gdt[17] = (gdt_entry) {//Tarea tss_MortyC137
        (uint16_t)    0x0067,         /* limit[0:15]  */ //103B
        (uint16_t)    (uint32_t)&tss_MortyC137,         /* base[0:15]   */
        (uint8_t)     ((uint32_t)&tss_MortyC137 >>16),           /* base[23:16]  */
        (uint8_t)     0x09,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     ((uint32_t)&tss_MortyC137 >>24),           /* base[31:24]  */
    };
    gdt[18] = (gdt_entry) {//Tarea tss_MortyD248
        (uint16_t)    0x0067,         /* limit[0:15]  */ //103B
        (uint16_t)    (uint32_t)&tss_MortyD248,         /* base[0:15]   */
        (uint8_t)     ((uint32_t)&tss_MortyD248 >>16),           /* base[23:16]  */
        (uint8_t)     0x09,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     ((uint32_t)&tss_MortyD248 >>24),           /* base[31:24]  */
    };
    gdt[19] = (gdt_entry) {//Tarea tss_Cronenberg1
        (uint16_t)    0x0067,         /* limit[0:15]  */ //103B
        (uint16_t)    (uint32_t)&tss_Cronenberg1,         /* base[0:15]   */
        (uint8_t)     ((uint32_t)&tss_Cronenberg1 >>16),           /* base[23:16]  */
        (uint8_t)     0x09,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     ((uint32_t)&tss_Cronenberg1 >>24),           /* base[31:24]  */
    };
    gdt[20] = (gdt_entry) {//Tarea tss_Cronenberg2
        (uint16_t)    0x0067,         /* limit[0:15]  */ //103B
        (uint16_t)    (uint32_t)&tss_Cronenberg2,         /* base[0:15]   */
        (uint8_t)     ((uint32_t)&tss_Cronenberg2 >>16),           /* base[23:16]  */
        (uint8_t)     0x09,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     ((uint32_t)&tss_Cronenberg2 >>24),           /* base[31:24]  */
    };
    gdt[21] = (gdt_entry) {//Tarea tss_Cronenberg3
        (uint16_t)    0x0067,         /* limit[0:15]  */ //103B
        (uint16_t)    (uint32_t)&tss_Cronenberg3,         /* base[0:15]   */
        (uint8_t)     ((uint32_t)&tss_Cronenberg3 >>16),           /* base[23:16]  */
        (uint8_t)     0x09,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     ((uint32_t)&tss_Cronenberg3 >>24),           /* base[31:24]  */
    };

}