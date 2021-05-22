/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "syscall.h" 
#include "i386.h"
void f();

void task() {
	uint8_t i = 1;
	uint8_t v;
	while(1){
		do { v = syscall_usePortalGun(i, 0, 0, 0); } while(v!=1);
		i++;
		i = i%80;
	};
}

    // while(1){

    //     syscall_usePortalGun(1,1,1,1);
    //     syscall_usePortalGun(2,1,0,0);
    
    // }
    //int x1, x2, x3, x4, x5;
/*  
    int x, y, z1, z2, z3, z4, v;
    
    
    do { v = syscall_usePortalGun(1, 0, 0, 0); } while(v!=1);
    do { v = syscall_usePortalGun(2, 0, 0, 0); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 0, 0); } while(v!=1);
    do { v = syscall_usePortalGun(0, 2, 0, 0); } while(v!=1);
    do { v = syscall_usePortalGun(-1, 0, 0, 0); } while(v!=1);
    do { v = syscall_usePortalGun(-2, 0, 0, 0); } while(v!=1);
    do { v = syscall_usePortalGun(0, -1, 0, 0); } while(v!=1);
    do { v = syscall_usePortalGun(0, -2, 0, 0); } while(v!=1);

    while(1) {
        for(int j=1; j< 40; j++) {
            for(int i=1; i< 80; i++) {
                do {
                    syscall_whereIsMorty(&x,&y);
                    syscall_whereIsMorty(&z1,&z2);
                    syscall_whereIsMorty(&z3,&z4);
                    if(!(x==i && y==j)) {
                        v = syscall_usePortalGun(i, j, 0, 0);
                        if(i == 40){
    						y = 4;
                        	x = 0;
    						z1 = 0xBABA;
    						z2 = 0xDEAD;
    						z3 = 0xCACA;
    						z4 = 0xCAFE;
    						x = y/x;
                        }
                    } else {
                        i++;
                    }
                } while(v!=1);
                 uint8_t *place = (uint8_t*)0x08002000;
                 uint8_t *pf = (uint8_t*)(&f);
                 for(int k=0; k< 512; k++) { //2048
                    //place[k] = 0x90;
                    place[k] = pf[k];
                 }
                 pf = (uint8_t*)(&f);
                 for(int k=0; k< 512; k++) {
                    place[k+2048] = pf[k];
                 }                 
            }
        }
    }
    
}
*/

/*
while(1) {
        for(int j=1; j< 40; j++) {
            for(int i=1; i< 80; i++) {
                do {
                    syscall_whereIsMorty(&x,&y);
                    if(!(x==i && y==j)) {
                        v = syscall_usePortalGun(i, j, 0, 0); 
                    } else {
                        i++;
                    }
                } while(v!=1);
                 uint8_t *place = (uint8_t*)0x08002000;
                 for(int k=0; k< 2048; k++) {
                    place[k] = 0x90;
                 }
                 uint8_t *pf = (uint8_t*)(&f);
                 for(int k=0; k< 512; k++) {
                    place[k+2048] = pf[k];
                 }                 
            }
        }
    }
*/

/*
void f() {
    while(1) {
    	uint8_t x = 3;
    	uint8_t y = 0;
    	x = x/y;
        //syscall_IamRick(0xC137);
    }
}
*/