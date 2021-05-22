/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

uint32_t proxima_pagina_libre;
uint32_t proxima_pagina_libre_tareas;

void mmu_init() {

    proxima_pagina_libre = INICIO_DE_PAGINAS_LIBRES;
    proxima_pagina_libre_tareas = INICIO_DE_PAGINAS_LIBRES_TAREAS;

}

uint32_t mmu_nextFreeKernelPage() {
    uint32_t pagina_libre = proxima_pagina_libre;
    proxima_pagina_libre += PAGE_SIZE; 
    return pagina_libre;
}

uint32_t mmu_nextFreePage(){
    uint32_t pagina_libre_tarea = proxima_pagina_libre_tareas;
    proxima_pagina_libre_tareas += PAGE_SIZE; 
    return pagina_libre_tarea;
}


void mmu_mapPage(uint32_t cr3, uint32_t virtual, uint32_t phy, uint32_t attrs) { 
    pd_entry* pd = (pd_entry*) ((cr3>>12)<<12);
    uint32_t directoryIdx = (virtual>>22);
    uint32_t tableIdx = ((virtual<<10)>>22);
    pt_entry* pt = (pt_entry*) (pd[directoryIdx].dir_base<<12);


    if (pd[directoryIdx].present != PAG_P)
    {
        uint32_t nueva_tabla = mmu_nextFreeKernelPage();
        pt = (pt_entry*) nueva_tabla;


        for (int i = 0; i < 1024; ++i)
        {
            pt[i] = (pt_entry) {0};
        }

        pd[directoryIdx].dir_base =(((uint32_t)pt) >>12);
        pd[directoryIdx].disponible = 0;
        pd[directoryIdx].g = 0;
        pd[directoryIdx].ps = 0;
        pd[directoryIdx].ignored = 0;
        pd[directoryIdx].a = 0;
        pd[directoryIdx].pcd = 0;
        pd[directoryIdx].pwt = 0;
        pd[directoryIdx].u_s = (attrs & PAG_US) >> 2;
        pd[directoryIdx].r_w = (attrs & PAG_RW) >> 1;
        pd[directoryIdx].present = PAG_P;
        
    }

    
    pt[tableIdx].present = PAG_P;
    pt[tableIdx].pwt = 0;
    pt[tableIdx].pcd = 0;
    pt[tableIdx].a = 0;
    pt[tableIdx].d = 0;
    pt[tableIdx].pat = 0;
    pt[tableIdx].g = 0;
    pt[tableIdx].disponible = 0;
    pt[tableIdx].u_s = (attrs & PAG_US) >> 2;
    pt[tableIdx].r_w = (attrs & PAG_RW) >> 1;
    pt[tableIdx].dir_base = (phy>>12);

    tlbflush();

}

void mmu_unmapPage(uint32_t cr3, uint32_t virtual) {
    pd_entry* pd = (pd_entry*) ((cr3>>12)<<12);
    uint32_t directoryIdx = (virtual>>22);
    uint32_t tableIdx = ((virtual<<10)>>22);
    pt_entry* pt = (pt_entry*) (pd[directoryIdx].dir_base<<12);
    pt[tableIdx].present = PAG_R;

    tlbflush();
}

uint32_t mmu_initKernelDir() {
	pd_entry* pd = (pd_entry*) KERNEL_PAGE_DIR;
	
	for (int i = 1; i < 1024; ++i)
	{
		pd[i] = (pd_entry) {0};
	}
	
	/* Inicializamos la Kernel Page Directory */
	pd[0].dir_base = (KERNEL_PAGE_TABLE_0>>12);
    pd[0].disponible = 0;
    pd[0].g = 0;
    pd[0].ps = 0;
    pd[0].ignored = 0;
    pd[0].a = 0;
    pd[0].pcd = 0;
    pd[0].pwt = 0;
    pd[0].u_s = 0;
    pd[0].r_w = 1;
    pd[0].present = PAG_P;

    /* Inicializamos la Kernel Page table */
    pt_entry* pt = (pt_entry*) KERNEL_PAGE_TABLE_0;

    for (int i = 0; i < 1024; ++i)
    {
    	pt[i].dir_base = i;
    	pt[i].disponible = 0;
    	pt[i].g = 0;
    	pt[i].pat = 0;
    	pt[i].d = 0;
    	pt[i].a = 0;
    	pt[i].pcd = 0;
    	pt[i].pwt = 0;
    	pt[i].u_s = 0;
    	pt[i].r_w = 1;
    	pt[i].present = PAG_P;
    }
    tlbflush();
    return KERNEL_PAGE_DIR;
}

uint32_t mmu_initTaskDir(uint32_t virtual, uint32_t tarea ,uint32_t posInicial_x, uint32_t posInicial_y){

    /*Pedimos memoria para el nuevo directorio*/
    uint32_t cr3 = mmu_nextFreeKernelPage();
    uint32_t attrS = 0x00000007;

    /*Mapeamos los primero 4MB con identity maping */
    for (int i = 0; i < 1024; ++i)
    {
        mmu_mapPage(cr3,i*PAGE_SIZE,i*PAGE_SIZE,attrS);
    }
    
    /*Pedimos memoria para el codigo de la tarea */
    uint32_t dir_fisicaAmapear_codigo = INICIO_DE_PAGINAS_LIBRES_TAREAS + (2*PAGE_SIZE*posInicial_x) +(2*PAGE_SIZE*80*posInicial_y);
    /*Pedimos memoria para la pila de la tarea */
    uint32_t dir_fisicaAmapear_pila = INICIO_DE_PAGINAS_LIBRES_TAREAS + (2*PAGE_SIZE*posInicial_x) + (2*PAGE_SIZE*80*posInicial_y)+PAGE_SIZE;
    
    //uint32_t attrU = 0x00000007;

    /*Mapeo la primera pagina de 4kb (codigo)*/
    mmu_mapPage(cr3,virtual,dir_fisicaAmapear_codigo,attrS);
    
    /*Mapeo la segunda pagina de 4kb (pila)*/
    mmu_mapPage(cr3,virtual+PAGE_SIZE,dir_fisicaAmapear_pila,attrS);
    
    /*Cargamos el cr3 actual*/
    uint32_t cr3Actual = rcr3();
    
    /*Mapeo la primera pagina al kernel con identity mapping*/
    mmu_mapPage(cr3Actual,dir_fisicaAmapear_codigo,dir_fisicaAmapear_codigo,attrS);
    mmu_mapPage(cr3Actual,dir_fisicaAmapear_pila,dir_fisicaAmapear_pila,attrS);

    /*Copio las 2 paginas de la  tarea*/
    uint32_t* src = (uint32_t*) tarea; 
    uint32_t* dst = (uint32_t*) dir_fisicaAmapear_codigo;
    

    for (int i = 0; i < 2048; ++i)
    {
        dst[i] = src[i];
        
    }
    

    /*Desmapeo*/
    
    mmu_unmapPage(cr3Actual,dir_fisicaAmapear_pila);
    mmu_unmapPage(cr3Actual,dir_fisicaAmapear_codigo);


    tlbflush();

    return cr3;
}







