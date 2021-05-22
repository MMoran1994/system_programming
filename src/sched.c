/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

/*Alterado*/
//                            0    1   2   3   4   5   6   7   8   9
//const uint8_t valoresX[24] = {2,  60, 18, 63, 30, 65, 70, 75, 73, 32, 70, 28, 73, 64, 28, 12,  3, 41, 27, 24, 54, 20, 33, 74};
//const uint8_t valoresY[24] = {31, 6, 39,  5, 37,  35,  5,  5,  0,  4,  2, 16, 15, 24, 24, 18, 23, 38, 12, 25, 14, 22,  0, 28};

/*Original*/
const uint8_t valoresX[24] = {2,  63, 18, 36, 30, 79, 21, 10, 73, 32, 70, 28, 73, 64, 28, 12,  3, 41, 27, 24, 54, 20, 33, 74};
const uint8_t valoresY[24] = {31, 32, 39,  5, 37, 29, 12, 24,  0,  4,  2, 16, 15, 24, 24, 18, 23, 38, 12, 25, 14, 22,  0, 28};

void getRandomPosicion(uint8_t index, uint8_t *x, uint8_t *y)
{
    *x = valoresX[index%24];
    *y = valoresY[index%24];    
}




#define IDLE 0
#define RICKC137 1
#define RICKD248 2
#define MORTYC137 3
#define MORTYD248 4
#define CRONENBERG1 5
#define CRONENBERG2 6
#define CRONENBERG3 7

#define CANT_COLUMNAS 80
#define CANT_FILAS    40
#define PORTAL_CREATED 1
#define PORTAL_NOT_CREATED 0

sched sched_task[8];
uint32_t current_task;
uint8_t times_GunUsed_rickC137;
uint8_t times_GunUsed_rickD248;
uint8_t minds_conquered_rickC137;
uint8_t minds_conquered_rickD248;
uint32_t canbeuse_byMortyC137;
uint32_t canbeuse_byMortyD248;
uint32_t portalNotYetCreated_byC137;
uint32_t portalNotYetCreated_byD248;
uint32_t cronenbergs_dead;
uint32_t excepcion;
uint32_t termino_juego;

_portal portals_Created_rickC137;
_portal portals_Created_rickD248;
_portal portals_Created_mortyC137;
_portal portals_Created_mortyD248;
void sched_init() {
	//Inicializamos las tss de cada tarea en la posicion x,y
	data_tarea datos_temp[8];

	datos_temp[RICKC137].codigo_fuente = TASK_DIR_RICKC137;
	datos_temp[RICKC137].p_tss = (uint32_t*) &tss_RickC137;
	datos_temp[RICKC137].tag = RICKC137;

	datos_temp[RICKD248].codigo_fuente = TASK_DIR_RICKD248;
	datos_temp[RICKD248].p_tss = (uint32_t*) &tss_RickD248;
	datos_temp[RICKD248].tag = RICKD248;

	datos_temp[MORTYC137].codigo_fuente = TASK_DIR_MORTYC137;
	datos_temp[MORTYC137].p_tss = (uint32_t*) &tss_MortyC137;
	datos_temp[MORTYC137].tag = MORTYC137;

	datos_temp[MORTYD248].codigo_fuente = TASK_DIR_MORTYD248;
	datos_temp[MORTYD248].p_tss = (uint32_t*) &tss_MortyD248;
	datos_temp[MORTYD248].tag = MORTYD248;

	datos_temp[CRONENBERG1].codigo_fuente = TASK_DIR_CRONENBERG;
	datos_temp[CRONENBERG1].p_tss = (uint32_t*) &tss_Cronenberg1;
	datos_temp[CRONENBERG1].tag = CRONENBERG1;

	datos_temp[CRONENBERG2].codigo_fuente = TASK_DIR_CRONENBERG;
	datos_temp[CRONENBERG2].p_tss = (uint32_t*) &tss_Cronenberg2;
	datos_temp[CRONENBERG2].tag = CRONENBERG2;

	datos_temp[CRONENBERG3].codigo_fuente = TASK_DIR_CRONENBERG;
	datos_temp[CRONENBERG3].p_tss = (uint32_t*) &tss_Cronenberg3;
	datos_temp[CRONENBERG3].tag = CRONENBERG3;


	modo_debug = 0;
	current_task = 0;
	
	minds_conquered_rickC137 = 0;
	minds_conquered_rickD248 = 0;
	
	canbeuse_byMortyD248 = FALSE;
	canbeuse_byMortyC137 = FALSE;
	
	portalNotYetCreated_byC137 = TRUE;
	portalNotYetCreated_byD248 = TRUE;
	
	times_GunUsed_rickD248 = 0;
	times_GunUsed_rickC137 = 0;
	cronenbergs_dead = 0;
	
	portals_Created_rickC137.pos_x = -1;
	portals_Created_rickC137.pos_y = -1;

	portals_Created_rickD248.pos_x = -1;
	portals_Created_rickD248.pos_y = -1;

	portals_Created_mortyC137.pos_x = -1; 
	portals_Created_mortyC137.pos_y = -1;
	
	portals_Created_mortyD248.pos_x = -1;
	portals_Created_mortyD248.pos_y = -1;
	excepcion  = FALSE;
	termino_juego = FALSE;

	uint8_t x, y;
	uint32_t codigo_fuente_tarea;
	uint32_t *tss_tarea;

	//Inicializamos el scheduler
	for (uint8_t i = 0; i < 8 ; ++i)
	{
		if(i != IDLE){
			getRandomPosicion(i, &x, &y);
			codigo_fuente_tarea = datos_temp[i].codigo_fuente;
			tss_tarea = datos_temp[i].p_tss;
			iniciar_tss(codigo_fuente_tarea, (tss*) tss_tarea, x, y);
			sched_task[i].pos_x  = (int) x;
			sched_task[i].pos_y  = (int) y;
		}
		sched_task[i].is_alive = 1;
		sched_task[i].tss_selector = ((i+14)<<3); //a partir de la 15 se encuentras los tss de cada tarea
		
	}
	
	iniciar_pantalla();
	

}


void iniciar_pantalla()
{

	screen_drawBox(0,0,CANT_FILAS,CANT_COLUMNAS,1,0x55); //Imicio pantalla CANT_COLUMNASxCANT_FILAS
	screen_drawBox(41,0,10,CANT_COLUMNAS,1,0x33);//Inicio panel CANT_COLUMNASx09

	screen_drawBox(42,26,7,26,1,0x66);  //panel de cronenbergs
	for (int i = 0; i < 10; ++i)
	{
		if (i<3)
		{
			print("O",30+2*i,44,0x0F);
		}
		else
		{
			print("-",30+2*i,44,0x0F);
			print("-",30+2*i,46,0x0F);
		}

	}

	screen_drawBox(44,4,3,9,1,0x44); //Panel rick C-137
	print_dec(0,7,5,45,0x0F); //Puntaje Inicial

	screen_drawBox(44,67,3,9,1,0x11);//Panel rick D-248
	print_dec(0,7,68,45,0x0F);//Puntaje Inicial
	
	print("R",sched_task[RICKC137].pos_x,sched_task[RICKC137].pos_y,0x04);
	print("R",sched_task[RICKD248].pos_x,sched_task[RICKD248].pos_y,0x01);
	print("M",sched_task[MORTYC137].pos_x,sched_task[MORTYC137].pos_y,0x04);
	print("M",sched_task[MORTYD248].pos_x,sched_task[MORTYD248].pos_y,0x01);
	print("C",sched_task[CRONENBERG1].pos_x,sched_task[CRONENBERG1].pos_y,0x06);
	print("C",sched_task[CRONENBERG2].pos_x,sched_task[CRONENBERG2].pos_y,0x06);
	print("C",sched_task[CRONENBERG3].pos_x,sched_task[CRONENBERG3].pos_y,0x06);
		
}


int16_t sched_nextTask() {

			
	current_task = (current_task + 1) % 8;
	while(sched_task[current_task].is_alive == 0){

		current_task = (current_task + 1) % 8;
	}
    return sched_task[current_task].tss_selector ;
}

void killcurrent_task()
{
	
	sched_task[current_task].is_alive = 0;
	if (current_task == CRONENBERG1 || current_task == CRONENBERG2 || current_task == CRONENBERG3)
	{
		if (excepcion)
		{
			if (cronenbergs_dead == 10)
			{
				print("X",30+2*(cronenbergs_dead%10),46,0x0F); // conquistado por rickC-137
				cronenbergs_dead++;
			
			}
			else
			{
	 			print("X",30+2*cronenbergs_dead,44,0x0F); // Marco en el tablero como muerto
				cronenbergs_dead++;
			}
		}
	}
	if ( current_task == RICKC137 || current_task == MORTYC137)
	{
		termino_juego = TRUE;

		reset_screen();
		
		print("GAME OVER",35,15,0x0F);
		print("Universe-D248 Wins",30,17,0x09);
		jump_toIdle();
		
	}
	else
	{
		if (current_task == RICKD248 || current_task == MORTYD248 )
		{
			termino_juego = TRUE;
			reset_screen();			
			print("GAME OVER",35,15,0x0F);
			print("Universe-C137 Wins",30,17,0x0C);
			jump_toIdle();
		}
	}
	if (cronenbergs_dead == 3)
	{
		termino_juego = TRUE;
		print("GAME OVER",35,15,0x0F);
		if (minds_conquered_rickC137 > minds_conquered_rickD248)
		{
			print("Universe-C137 Wins",30,17,0x0C);
		}
		else
		{
			if (minds_conquered_rickD248 == minds_conquered_rickC137 )
			{
				print("EMPATE",35,17,0x0F);
			}
			else
			{
				print("Universe-D248 Wins",30,17,0x09);
			}
		}
	}	
	
	jump_toIdle();
	
}


void reset_screen()
{
	screen_drawBox(0,0,CANT_FILAS,CANT_COLUMNAS,1,0x55); //Imicio pantalla CANT_COLUMNASxCANT_FILAS

	for (int i = 1; i < 8; ++i)
	{
		if (sched_task[i].is_alive == 1 )
		{
			if (i == RICKC137 )
			{
				print("R",sched_task[i].pos_x,sched_task[i].pos_y,0x04);	
			}
			else
			{
			if (i == MORTYC137)
			{	
				print("M",sched_task[i].pos_x,sched_task[i].pos_y,0x04);
			}
			else
			{
			if (i == RICKD248)
			{	
				print("R",sched_task[i].pos_x,sched_task[i].pos_y,0x01);
			}
			else
			{
			if (i == MORTYD248)
			{
				print("M",sched_task[i].pos_x,sched_task[i].pos_y,0x01);
			}
			else
			{
				print("C",sched_task[i].pos_x,sched_task[i].pos_y,0x06);
			}
			}
			}
			}
		}
	}

	//Portales

	if (portals_Created_rickC137.pos_x != -1)
	{
		print("O",portals_Created_rickC137.pos_x,portals_Created_rickC137.pos_y,0x24);

		
	}
	if (portals_Created_rickD248.pos_x != -1)
	{
		print("O",portals_Created_rickD248.pos_x,portals_Created_rickD248.pos_y,0x21);
		
	}
	if (portals_Created_mortyC137.pos_x != -1)
	{
		print("O",portals_Created_mortyC137.pos_x,portals_Created_mortyC137.pos_y,0x24);
	}
	if (portals_Created_mortyD248.pos_x  != -1)
	{
		print("O",portals_Created_mortyD248.pos_x,portals_Created_mortyD248.pos_y,0x21);
	}
}


void portalCreatReset()
{
		portalNotYetCreated_byC137 = TRUE;
		portalNotYetCreated_byD248 = TRUE;
}

void iamRick(uint16_t code)
{
	if (current_task == CRONENBERG1 || current_task == CRONENBERG2 || current_task == CRONENBERG3)
	{
		if (code == 0xC137)
		{

			minds_conquered_rickC137++;
			print_dec(minds_conquered_rickC137,7,5,45,0x0F); //SUmar Puntaje 
			if (cronenbergs_dead == 10)
			{
				print("O",30+2*(cronenbergs_dead%10),46,0x04); // conquistado por rickC-137
				cronenbergs_dead++;
			
			}
			else
			{
				print("O",30+2*cronenbergs_dead,44,0x04); // conquistado por rickC-137
				cronenbergs_dead++;

			}
			killcurrent_task();
		}
		else
		{
			minds_conquered_rickD248++;
			print_dec(minds_conquered_rickD248,7,68,45,0x0F);// Sumar Puntaje
			
			if (cronenbergs_dead == 10)
			{
				print("O",30+2*(cronenbergs_dead%10),46,0x01); // conquistado por rickC-137
				cronenbergs_dead++;
			}
			else
			{				
				print("O",30+2*cronenbergs_dead,44,0x01); // conquistado por rickD-248
				cronenbergs_dead++;
			}


			killcurrent_task();
			

		}
	}
}


 int whereIsMorty_posX()
 {	
 	int desp_x = 0;
 	
 	if (current_task == RICKD248)
 	{ 
 		desp_x = (sched_task[MORTYD248].pos_x - sched_task[current_task].pos_x) % CANT_COLUMNAS;
 		return desp_x; 
 	}
 	else
 	{
 		if (current_task == RICKC137)
 		{
 			desp_x = (sched_task[MORTYC137].pos_x - sched_task[current_task].pos_x) % CANT_COLUMNAS;
 			return desp_x;
 			
 		}
 		else
 		{
 			if (current_task == CRONENBERG1 || 
 				current_task == CRONENBERG2 || 
 				current_task == CRONENBERG3 )
 			{
	 			if (cronenbergs_dead == 10)
				{
					print("X",30+2*(cronenbergs_dead%10),46,0x0F); // conquistado por rickC-137
					cronenbergs_dead++;
				
				}
				else
				{
	 				print("X",30+2*cronenbergs_dead,44,0x0F); // Marco en el tablero como muerto
					cronenbergs_dead++;
				}
	 		}
 			
 			killcurrent_task();
 			return 0;
 				
 		}
 	}

 }


 int whereIsMorty_posY()
 {
 	int desp_y = 0;
 	
 	if (current_task == RICKD248)
 	{
 
 		desp_y = (sched_task[MORTYD248].pos_y - sched_task[current_task].pos_y) % CANT_FILAS;

 		return desp_y; 
 	}
 	else
 	{
 		if (current_task == RICKC137)
 		{
 			desp_y = (sched_task[MORTYC137].pos_y - sched_task[current_task].pos_y) % CANT_FILAS;

 			return desp_y;
 			
 		}
 		else
 		{
 			if (current_task == CRONENBERG1 || 
 				current_task == CRONENBERG2 || 
 				current_task == CRONENBERG3 )
 			{
 				if (cronenbergs_dead == 10)
				{
					print("X",30+2*(cronenbergs_dead%10),46,0x0F); // conquistado por rickC-137
					cronenbergs_dead++;
				
				}
				else
				{
	 				print("X",30+2*cronenbergs_dead,44,0x0F); // Marco en el tablero como muerto
					cronenbergs_dead++;
				}
 			}
 				
 			killcurrent_task();
 			return 0;
 				 				
 		}
 	} 	

 }



uint32_t int137(int desp_x,int desp_y,uint32_t cross,uint32_t withMorty)
{
	//si me llama una Cronenberg mato la tarea
	if(current_task == CRONENBERG1 ||
	   current_task == CRONENBERG2 ||
	   current_task == CRONENBERG3)
	{	

		if (cronenbergs_dead == 10)
		{
			print("X",30+2*(cronenbergs_dead%10),46,0x0F); // conquistado por rickC-137
			cronenbergs_dead++;
		
		}
		else
		{
	 		print("X",30+2*cronenbergs_dead,44,0x0F); // Marco en el tablero como muerto
			cronenbergs_dead++;
		}
		killcurrent_task();
		
	}
	

	if( ((current_task == RICKC137) && (portalNotYetCreated_byC137)) ||
	   ((current_task == RICKD248) && (portalNotYetCreated_byD248)))
	{

		
		//caso cruzar de una tarea Rick
		if(cross)
		{
			if(withMorty)
			{
				
				//si cruza con Morty, transladar codigo a direccion indicada por
				//el desplazamiento a partir de la direccion de Morty
				uint8_t morty;
				uint32_t* src_code_morty;
				uint32_t* src_code_rick;
				
				if(current_task == RICKC137){
					morty = MORTYC137;
					src_code_morty = (uint32_t*)TASK_DIR_MORTYC137;
					src_code_rick = (uint32_t*)TASK_DIR_RICKC137;
					
				} else {

					morty = MORTYD248;
					src_code_morty = (uint32_t*)TASK_DIR_MORTYD248;
					src_code_rick = (uint32_t*)TASK_DIR_RICKD248;					
				}

				//calculamos las posiciones de destino
				int x_morty = hacer_resto_positivo((sched_task[morty].pos_x + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS);
				int y_morty = hacer_resto_positivo((sched_task[morty].pos_y + desp_y) % CANT_FILAS 	 , CANT_FILAS);
				
				int x_rick = hacer_resto_positivo((sched_task[current_task].pos_x + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS);
				int y_rick = hacer_resto_positivo((sched_task[current_task].pos_y + desp_y) % CANT_FILAS   , CANT_FILAS);

				int pos_rickx = x_rick;
				int pos_ricky = y_rick;
					
				x_morty = x_morty * 2*PAGE_SIZE;
				y_morty = y_morty * 2*CANT_COLUMNAS*PAGE_SIZE;

				x_rick = x_rick * 2*PAGE_SIZE;
				y_rick = y_rick * 2*CANT_COLUMNAS*PAGE_SIZE;

				uint32_t desp_final_morty = (uint32_t) (x_morty + y_morty);
				uint32_t desp_final_rick = (uint32_t) (x_rick + y_rick);


				//obtenemos los cr3 de Rick y de Morty
				uint16_t tss_base_0a15 = gdt[(sched_task[morty].tss_selector>>3)].base_0_15;
				uint8_t tss_base_23a16 = gdt[(sched_task[morty].tss_selector>>3)].base_23_16;
				uint8_t tss_base_31a24 = gdt[(sched_task[morty].tss_selector>>3)].base_31_24;
				uint32_t tss_base1 = (uint32_t) tss_base_0a15;
				uint32_t tss_base2 = (uint32_t) tss_base_23a16;
				uint32_t tss_base3 = (uint32_t) tss_base_31a24;
				tss* tss_morty = (tss*) (tss_base1 + (tss_base2<<16) + (tss_base3<<24));
				
				uint32_t cr3Rick = rcr3();
				uint32_t cr3Morty = tss_morty->cr3;
				uint32_t attrs = 0x00000007;
				uint32_t dir_dest_morty;
				uint32_t dir_dest_rick;

				
				//mapeamos la direccion destino usando Identity Mapping
				dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty;
				dir_dest_rick = INICIO_MUNDO_CRONENBERG + desp_final_rick;
				mmu_mapPage(cr3Rick,dir_dest_rick, dir_dest_rick, attrs);
				mmu_mapPage(cr3Rick,dir_dest_morty, dir_dest_morty, attrs);
				
				dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty + PAGE_SIZE;
				dir_dest_rick = INICIO_MUNDO_CRONENBERG + desp_final_rick + PAGE_SIZE;
				mmu_mapPage(cr3Rick,dir_dest_rick, dir_dest_rick, attrs);
				mmu_mapPage(cr3Rick,dir_dest_morty, dir_dest_morty, attrs); 

				uint32_t* dst_morty = (uint32_t*) (INICIO_MUNDO_CRONENBERG + desp_final_morty);
				uint32_t* dst_rick  = (uint32_t*) (INICIO_MUNDO_CRONENBERG + desp_final_rick);

				//copiamos el codigo fuente a las dos paginas de destino
				for (int i = 0; i < 2048; ++i)
    			{
					
    			    dst_morty[i] = src_code_morty[i];
    			    dst_rick[i] = src_code_rick[i];
    			    
    			}
			    /*Desmapeo*/
			    dir_dest_morty = INICIO_MUNDO_CRONENBERG+desp_final_morty;
				dir_dest_rick = INICIO_MUNDO_CRONENBERG+desp_final_rick;			    
			    mmu_unmapPage(cr3Rick,dir_dest_rick);
			    mmu_unmapPage(cr3Rick,dir_dest_morty);
			    
			    dir_dest_morty = INICIO_MUNDO_CRONENBERG+desp_final_morty+PAGE_SIZE;
				dir_dest_rick = INICIO_MUNDO_CRONENBERG+desp_final_rick+PAGE_SIZE; 
			    mmu_unmapPage(cr3Rick,dir_dest_rick);
			    mmu_unmapPage(cr3Rick,dir_dest_morty);

			    //Desmapeamos las direcciones actuales de las tareas
			    mmu_unmapPage(cr3Rick,TASK_CODE);
			    mmu_unmapPage(cr3Morty,TASK_CODE);
			    mmu_unmapPage(cr3Rick,TASK_CODE+PAGE_SIZE);
			    mmu_unmapPage(cr3Morty,TASK_CODE+PAGE_SIZE);

			    //Mapeamos a la nueva direccion destino
			    dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty;
				dir_dest_rick = INICIO_MUNDO_CRONENBERG + desp_final_rick;
			    
			    mmu_mapPage(cr3Rick,TASK_CODE, dir_dest_rick, attrs);
				mmu_mapPage(cr3Morty,TASK_CODE, dir_dest_morty, attrs);
				
				dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty+PAGE_SIZE;
				dir_dest_rick = INICIO_MUNDO_CRONENBERG + desp_final_rick+PAGE_SIZE;
				
				mmu_mapPage(cr3Rick,TASK_CODE + PAGE_SIZE, dir_dest_rick, attrs);
				mmu_mapPage(cr3Morty,TASK_CODE + PAGE_SIZE, dir_dest_morty, attrs);


				//Calculo las nuevas posiciones de la tarea en patanlla
				

				x_morty = sched_task[morty].pos_x;
				y_morty = sched_task[morty].pos_y;
				x_morty = hacer_resto_positivo((x_morty + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS);
				y_morty = hacer_resto_positivo((y_morty + desp_y) % CANT_FILAS, CANT_FILAS);

				sched_task[current_task].pos_y = pos_ricky;
				sched_task[current_task].pos_x = pos_rickx;

				sched_task[morty].pos_y = y_morty;
				sched_task[morty].pos_x = x_morty;

				
				

				//reseteamos la pantala con las nuevas posiciones
				reset_screen();

				if(current_task == RICKC137)
				{
					portalNotYetCreated_byC137 = FALSE;
					times_GunUsed_rickC137++;
				
					if (times_GunUsed_rickC137 == 10 )
					{
						canbeuse_byMortyC137 = TRUE;
						times_GunUsed_rickC137 = 0;
					}

				} else {

					portalNotYetCreated_byD248 = FALSE;
					times_GunUsed_rickD248++;
					
					if (times_GunUsed_rickD248 == 10)
					{
						canbeuse_byMortyD248 = TRUE;
						times_GunUsed_rickD248 = 0;
					}
				}

    			//breakpoint();
				return PORTAL_CREATED;

			}
			else
			{	//La tarea sigue siendo una rick 
				//Cross without Morty
				uint32_t* src_code_rick;

				if(current_task == RICKC137){
					src_code_rick = (uint32_t*)TASK_DIR_RICKC137;
				} else {
					src_code_rick = (uint32_t*)TASK_DIR_RICKD248;
				}

				//obtenemos posiciones de partida
				int x_rick = sched_task[current_task].pos_x;
				int y_rick = sched_task[current_task].pos_y;

				//calculamos las posiciones de destino
				x_rick	= hacer_resto_positivo( (x_rick + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS) * 2*PAGE_SIZE;
				y_rick	= hacer_resto_positivo( (y_rick + desp_y) % CANT_FILAS   , CANT_FILAS) * 2*CANT_COLUMNAS*PAGE_SIZE;
				uint32_t desp_final_rick = (uint32_t) (x_rick + y_rick);



				//obtenemos el cr3 de Rick

				uint32_t cr3Rick = rcr3();
				uint32_t attrs = 0x00000007;
				uint32_t dir_dest_rick;

				
				//mapeamos la direccion destino usando Identity Mapping
				dir_dest_rick = INICIO_MUNDO_CRONENBERG + desp_final_rick;
				mmu_mapPage(cr3Rick,dir_dest_rick, dir_dest_rick, attrs);
				dir_dest_rick = INICIO_MUNDO_CRONENBERG + desp_final_rick + PAGE_SIZE;
				mmu_mapPage(cr3Rick,dir_dest_rick, dir_dest_rick, attrs);


				//copiamos el codigo fuente a las dos paginas de destino
				uint32_t* dst_rick  = (uint32_t*) (INICIO_MUNDO_CRONENBERG + desp_final_rick);
				for (int i = 0; i < 2048; ++i)
    			{
    			    dst_rick[i] = src_code_rick[i];   			    
    			}

			    /*Desmapeo*/
				dir_dest_rick = INICIO_MUNDO_CRONENBERG+desp_final_rick;			    
			    mmu_unmapPage(cr3Rick,dir_dest_rick);
				dir_dest_rick = INICIO_MUNDO_CRONENBERG+desp_final_rick+PAGE_SIZE;
			    mmu_unmapPage(cr3Rick,dir_dest_rick);
			    
			    //Mapeamos a la nueva direccion destino
				dir_dest_rick = INICIO_MUNDO_CRONENBERG + desp_final_rick;			    
			    mmu_mapPage(cr3Rick,TASK_CODE, dir_dest_rick, attrs);
				dir_dest_rick = INICIO_MUNDO_CRONENBERG + desp_final_rick+PAGE_SIZE;	
				mmu_mapPage(cr3Rick,TASK_CODE + PAGE_SIZE, dir_dest_rick, attrs);

				x_rick = sched_task[current_task].pos_x;
				y_rick = sched_task[current_task].pos_y;
				x_rick = hacer_resto_positivo( (x_rick + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS);
				y_rick = hacer_resto_positivo( (y_rick + desp_y) % CANT_FILAS   , CANT_FILAS);

				sched_task[current_task].pos_y = y_rick;
				sched_task[current_task].pos_x = x_rick;

				//reseteamos la pantala con las nuevas posiciones
				reset_screen();

				if(current_task == RICKC137){
					portalNotYetCreated_byC137 = FALSE;
					times_GunUsed_rickC137++;
					
					if (times_GunUsed_rickC137 == 10)
					{
						canbeuse_byMortyC137 = TRUE;
						times_GunUsed_rickC137 = 0;
					}
				} else {		
					portalNotYetCreated_byD248 = FALSE;
					times_GunUsed_rickD248++;
					if (times_GunUsed_rickD248 == 10)
					{
						canbeuse_byMortyD248 = TRUE;
						times_GunUsed_rickD248 = 0;
					}
				}

				return PORTAL_CREATED;


			}
		
		}
		else
		{
			//Case No cross la tarea sigue siendo una rick
			if (withMorty)
			{
				//Mapeamos el portal de Rick desde la posicion de su Morty
				uint8_t morty;
				
				if(current_task == RICKC137){
					morty = MORTYC137;
				} else {
					morty = MORTYD248;
				}
				//obtenemos posiciones de partida
				int x_morty = sched_task[morty].pos_x;
				int y_morty = sched_task[morty].pos_y;
				
				//calculamos las posiciones de destino
				x_morty = hacer_resto_positivo( (x_morty + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS) 	*2*PAGE_SIZE;
				y_morty = hacer_resto_positivo( (y_morty + desp_y) % CANT_FILAS, CANT_FILAS) 		*2*CANT_COLUMNAS*PAGE_SIZE;
				uint32_t desp_final_morty = x_morty + y_morty;



				//obtenemos el cr3 de Rick y el de morty
				uint32_t cr3Rick = rcr3();
				uint32_t attrs = 0x00000007;
				uint32_t dir_dest_morty;
				
				//mapeamos la direccion destino usando la direccion Virtual 
				//mapeamos codigo
				dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty;
				mmu_mapPage(cr3Rick,RE_MAP_TASK_CODE, dir_dest_morty, attrs);
				
				//mapeamos pila
				dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty + PAGE_SIZE;
				mmu_mapPage(cr3Rick,RE_MAP_TASK_CODE + PAGE_SIZE, dir_dest_morty, attrs);

				//calculamos las posiciones para setear el portal
				x_morty = sched_task[morty].pos_x;
				y_morty = sched_task[morty].pos_y;
				
				x_morty = hacer_resto_positivo( (x_morty + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS);
				y_morty = hacer_resto_positivo( (y_morty + desp_y) % CANT_FILAS, CANT_COLUMNAS);

				if(current_task == RICKC137){
					portalNotYetCreated_byC137 = FALSE;
					times_GunUsed_rickC137++;
					portals_Created_rickC137.pos_x = x_morty;
					portals_Created_rickC137.pos_y = y_morty;
					if (times_GunUsed_rickC137 == 10)
					{
						canbeuse_byMortyC137 = TRUE;
						times_GunUsed_rickC137 = 0;
					}

				} else {

					portalNotYetCreated_byD248 = FALSE;
					times_GunUsed_rickD248++;
					portals_Created_rickD248.pos_x = x_morty;
					portals_Created_rickD248.pos_y = y_morty;
					if (times_GunUsed_rickD248 == 10)
					{
						canbeuse_byMortyD248 = TRUE;
						times_GunUsed_rickD248 = 0;
					}
				}
				//reseteo Pantalla
				reset_screen();

				
				return PORTAL_CREATED;

				
			}
			else
			{	
				//Tarea Rick
				//Case No cross without morty
				//mapeamos desde la posicion de Rick
				
				int x_rick = sched_task[current_task].pos_x;
				int y_rick = sched_task[current_task].pos_y;

				x_rick	= hacer_resto_positivo((x_rick + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS) * 2*PAGE_SIZE;
				y_rick	= hacer_resto_positivo((y_rick + desp_y) % CANT_FILAS, CANT_FILAS)    	 * 2*CANT_COLUMNAS*PAGE_SIZE;
				uint32_t desp_final_rick = (uint32_t) (x_rick + y_rick);



				//obtenemos el cr3 de Rick 
				uint32_t cr3Rick = rcr3();
				uint32_t attrs = 0x00000007;
				uint32_t dir_dest_rick;

				dir_dest_rick = INICIO_MUNDO_CRONENBERG + desp_final_rick;				
				mmu_mapPage(cr3Rick,RE_MAP_TASK_CODE, dir_dest_rick, attrs);
				dir_dest_rick = INICIO_MUNDO_CRONENBERG + desp_final_rick + PAGE_SIZE;		
				mmu_mapPage(cr3Rick,RE_MAP_TASK_CODE + PAGE_SIZE, dir_dest_rick, attrs);

				//calculamos las posiciones de destino
				x_rick = sched_task[current_task].pos_x;
				y_rick = sched_task[current_task].pos_y;


				x_rick	= hacer_resto_positivo((x_rick + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS);
				y_rick	= hacer_resto_positivo((y_rick + desp_y) % CANT_FILAS, CANT_FILAS);


				if(current_task == RICKC137)
				{
					portalNotYetCreated_byC137 = FALSE;
					times_GunUsed_rickC137++;
					portals_Created_rickC137.pos_x = x_rick;
					portals_Created_rickC137.pos_y = y_rick;
					if (times_GunUsed_rickC137 == 10)
					{
						canbeuse_byMortyC137 = TRUE;
						times_GunUsed_rickC137 = 0;
					}

				} else {

					portalNotYetCreated_byD248 = FALSE;
					times_GunUsed_rickD248++;
					portals_Created_rickD248.pos_x = x_rick;
					portals_Created_rickD248.pos_y = y_rick;
					if (times_GunUsed_rickD248 == 10)
					{
						canbeuse_byMortyD248 = TRUE;
						times_GunUsed_rickD248 = 0;
					}
				}

				//Reseteo Pantalla
				reset_screen();

				return PORTAL_CREATED;

			}


		}

	}
	else
	{
		//chequeamos que sea una Morty y que pueda usar el arma.
		//si no, retornamos 0
		if (((current_task == MORTYC137) && canbeuse_byMortyC137) || 
			((current_task == MORTYD248) && canbeuse_byMortyD248))
		{

			if (cross)
			{
				
				//Cruza solo Morty, transladar codigo a direccion indicada por
				//el desplazamiento a partir de la direccion de Morty
				uint8_t morty;
				uint32_t* src_code_morty;
				
				if(current_task == MORTYC137){
					morty = MORTYC137;
					src_code_morty = (uint32_t*)TASK_DIR_MORTYC137;
					
				} else {

					src_code_morty = (uint32_t*)TASK_DIR_MORTYD248;
					morty = MORTYD248;
				}

				//obtenemos posiciones de partida
				int x_morty = sched_task[morty].pos_x;
				int y_morty = sched_task[morty].pos_y;
				
				//calculamos las posiciones de destino
				x_morty = hacer_resto_positivo((x_morty + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS) * 2*PAGE_SIZE;
				y_morty = hacer_resto_positivo((y_morty + desp_y) % CANT_FILAS, CANT_FILAS) * 2*CANT_COLUMNAS*PAGE_SIZE;
				uint32_t desp_final_morty = x_morty + y_morty;



				//obtenemos el cr3 de Morty
				uint32_t cr3Morty = rcr3();
				uint32_t attrs = 0x00000007;
				uint32_t dir_dest_morty;
				
				
				//mapeamos la direccion destino usando Identity Mapping
				dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty;
				mmu_mapPage(cr3Morty,dir_dest_morty, dir_dest_morty, attrs);
				dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty + PAGE_SIZE;
				mmu_mapPage(cr3Morty,dir_dest_morty, dir_dest_morty, attrs); 

				//copiamos el codigo fuente a las dos paginas de destino
				uint32_t* dst_morty = (uint32_t*) (INICIO_MUNDO_CRONENBERG + desp_final_morty);
				for (int i = 0; i < 2048; ++i)
    			{
    			    dst_morty[i] = src_code_morty[i];
    			    
    			}

			    /*Desmapeo*/
			    dir_dest_morty = INICIO_MUNDO_CRONENBERG+desp_final_morty;
			    mmu_unmapPage(cr3Morty,dir_dest_morty);
			    dir_dest_morty = INICIO_MUNDO_CRONENBERG+desp_final_morty+PAGE_SIZE;
			    mmu_unmapPage(cr3Morty,dir_dest_morty);

			    //Mapeamos a la nueva direccion destino
			    dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty;
				mmu_mapPage(cr3Morty,TASK_CODE, dir_dest_morty, attrs);
				dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty+PAGE_SIZE;
				mmu_mapPage(cr3Morty,TASK_CODE + PAGE_SIZE, dir_dest_morty, attrs);

				x_morty = sched_task[morty].pos_x;
				y_morty = sched_task[morty].pos_y;
				
				//calculamos las posiciones de destino
				x_morty = hacer_resto_positivo((x_morty + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS);
				y_morty = hacer_resto_positivo((y_morty + desp_y) % CANT_FILAS,    CANT_FILAS);

				sched_task[current_task].pos_y = y_morty;
				sched_task[current_task].pos_x = x_morty;

				
				if (morty == MORTYC137)
				{
					canbeuse_byMortyC137 = FALSE;
				}
				else
				{
					canbeuse_byMortyD248 = FALSE;
				}

				//reseteamos la pantala con las nuevas posiciones
				reset_screen();
				return PORTAL_CREATED;
				
			}
			else
			{
				//La tarea que llamo es una morty y no cross
				//Solo mapeamos desde el mismo Morty

				//obtenemos posiciones de partida
				int x_morty = sched_task[current_task].pos_x;
				int y_morty = sched_task[current_task].pos_y;
				
				//calculamos las posiciones de destino
				x_morty = hacer_resto_positivo((x_morty + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS) * 2*PAGE_SIZE;
				y_morty = hacer_resto_positivo((y_morty + desp_y) % CANT_FILAS, CANT_FILAS) * 2*CANT_COLUMNAS*PAGE_SIZE;
				uint32_t desp_final_morty = x_morty + y_morty;

				//obtenemos el cr3 de Morty
				uint32_t cr3Morty = rcr3();
				uint32_t attrs = 0x00000007;
				uint32_t dir_dest_morty;

				//mapeamos la direccion destino usando la direccion Virtual 
				//mapeo del codigo
				dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty;
				mmu_mapPage(cr3Morty, RE_MAP_TASK_CODE, dir_dest_morty, attrs);

				//mapeo de la pila
				dir_dest_morty = INICIO_MUNDO_CRONENBERG + desp_final_morty + PAGE_SIZE;				
				mmu_mapPage(cr3Morty,RE_MAP_TASK_CODE + PAGE_SIZE, dir_dest_morty, attrs);

				//calculamos las posiciones de destino
				x_morty = sched_task[current_task].pos_x;
				y_morty = sched_task[current_task].pos_y;
				
				x_morty = hacer_resto_positivo((x_morty + desp_x) % CANT_COLUMNAS, CANT_COLUMNAS);
				y_morty = hacer_resto_positivo((y_morty + desp_y) % CANT_FILAS, CANT_FILAS);

				if(current_task == MORTYC137)
				{
					portals_Created_mortyC137.pos_x = x_morty;
					portals_Created_mortyC137.pos_y = y_morty;
					canbeuse_byMortyC137 = FALSE;
				} else {
					portals_Created_mortyD248.pos_x = x_morty;
					portals_Created_mortyD248.pos_y = y_morty;
					canbeuse_byMortyD248 = FALSE;			
				}

				//seteamos la pantalla
				reset_screen();
				return PORTAL_CREATED;


			}
			

		}
		//Sino puedo usar el arma morty retorno 0
		return PORTAL_NOT_CREATED;

	}
}




void imprimir_registros(uint32_t eip, uint32_t eflags,uint16_t ss,uint16_t gs,uint16_t fs,uint16_t es,uint16_t ds,
	uint16_t cs, uint32_t edi,uint32_t esi,uint32_t ebp,uint32_t esp,uint32_t ebx,uint32_t edx,uint32_t ecx, 
	uint32_t eax,uint32_t stack1,uint32_t stack2,uint32_t stack3,uint32_t stack4,uint32_t stack5 ){
	print("eax",26,7,0x0F);
	print_hex(eax,8,30,7,0x0F);
	print("ecx",26,9,0x0F);
	print_hex(ecx,8,30,9,0x0F);
	print("edx",26,11,0x0F);
	print_hex(edx,8,30,11,0x0F);
	print("ebx",26,13,0x0F);
	print_hex(ebx,8,30,13,0x0F);
	print("esp",26,15,0x0F);
	print_hex(esp,8,30,15,0x0F);
	print("ebp",26,17,0x0F);
	print_hex(ebp,8,30,17,0x0F);
	print("esi",26,19,0x0F);
	print_hex(esi,8,30,19,0x0F);
	print("edi",26,21,0x0F);
	print_hex(edi,8,30,21,0x0F);
	/*
	uint16_t tss_base_0a15 = gdt[sched_task[current_task].tss_selector].base_0_15;
	uint8_t tss_base_23a16 = gdt[sched_task[current_task].tss_selector].base_23_16;
	uint8_t tss_base_31a24 = gdt[sched_task[current_task].tss_selector].base_31_24;
	uint32_t tss_base1 = (uint32_t) tss_base_0a15;
	uint32_t tss_base2 = (uint32_t) tss_base_23a16;
	uint32_t tss_base3 = (uint32_t) tss_base_31a24;
	uint32_t tss_entera= tss_base1 + (tss_base2<<16) + (tss_base3<<24);
	tss* tss_tareaActual = (tss*) tss_entera;
	*/
	print("eip",26,23,0x0F);
	print_hex(eip,8,30,23,0x0F);
	print("cs",27,25,0x0F);
	print_hex(cs,4,30,25,0x0F);
	print("ds",27,27,0x0F);
	print_hex(ds,4,30,27,0x0F);
	print("es",27,29,0x0F);
	print_hex(es,4,30,29,0x0F);
	print("fs",27,31,0x0F);
	print_hex(fs,4,30,31,0x0F);
	print("gs",27,33,0x0F);
	print_hex(gs,4,30,33,0x0F);
	print("ss",27,35,0x0F);
	print_hex(ss,4,30,35,0x0F);
	print("eflags",26,37,0x0F);
	print_hex(eflags,8,33,37,0x0F);		
	uint32_t cr0 = rcr0();
	print("cr0",39,8,0x0F);
	print_hex(cr0,8,44,8,0x0F);
	uint32_t cr2 = rcr2();
	print("cr2",39,10,0x0F);
	print_hex(cr2,8,44,10,0x0F);
	uint32_t cr3 = rcr3();
	print("cr3",39,12,0x0F);
	print_hex(cr3,8,44,12,0x0F);
	uint32_t cr4 = rcr4();
	print("cr4",39,14,0x0F);
	print_hex(cr4,8,44,14,0x0F);
	print("Stack",40,27,0x0F);
	print_hex(stack1,8,40,28,0x0F);
	print_hex(stack2,8,40,29,0x0F);
	print_hex(stack3,8,40,30,0x0F);
	print_hex(stack4,8,40,31,0x0F);
	print_hex(stack5,8,40,32,0x0F);
	screen_drawBox(1,24,40,1,1,0x00); //pinto linea izquierda
	screen_drawBox(1,53,40,1,1,0x00); //pinto linea derecha
	screen_drawBox(1,24,1,29,1,0x00); //pinto linea horzontal superior
	screen_drawBox(3,24,1,29,1,0x00); //pinto linea horzontal inferior
	screen_drawBox(2,25,1,28,1,0x44); //pinto linea roja intermedia


}



void print_exception(uint32_t num_excepcion)
{
	print("Exception ",28,2,0x4F);
	print_dec(num_excepcion,2,38,2,0x4F);
}


void activar_modo_debug()
{
	if(modo_debug == 0)
	{
		modo_debug = 1;
	}
	else
	{
		modo_debug = 0;
	}
}

int32_t verificar_modo_debug()
{
	return modo_debug;
}

void activar_desactivar_excepcion()
{
	if (excepcion == FALSE)
	{
		excepcion = TRUE;
	}
	else
	{
		excepcion = FALSE;
	}
}

int32_t verificar_excepcion()
{
	return excepcion;
}

int32_t verificar_fin_del_juego()
{
	return termino_juego;
}

int hacer_resto_positivo(int x, int divisor){
	if(x<0){
		return x+divisor;
	} else {
		return x;
	}
}