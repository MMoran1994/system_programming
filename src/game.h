/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "stdint.h"
#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"

typedef enum e_taskType {
    RickC137       = 1,
    MortyC137      = 2,
    RickD248       = 3,
    MortyD248      = 4,
    Cronenberg     = 5,
    CronenbergC137 = 6,
    CronenbergD248 = 7,
} e_taskType_t;

void game_init();

#endif  /* !__GAME_H__ */
