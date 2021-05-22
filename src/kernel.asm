; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"


global start
extern GDT_DESC
extern screen_drawBox
extern IDT_DESC
extern idt_init
extern pic_reset
extern pic_enable
extern mmu_initKernelDir
extern mmu_initTaskDir
extern mmu_init
extern gdt_tss_init
extern tss_init
extern gdt_tss
extern sched_init
extern imprimir_registros
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

start_pm_msgA db     'Ay Chalai'
start_pm_lenA equ    $ - start_pm_msgA

libreta_1 db    '218/16'
lib1_len equ $ - libreta_1

libreta_2 db    '650/17'
lib2_len equ $ - libreta_2

primerChar db 'A'
primerChar_len equ $ - primerChar


;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0
    

    ; Habilitar A20
    call A20_enable

    ; Cargar la GDT
    LGDT [GDT_DESC]

    ; Setear el bit PE del registro CR0
    MOV eax,cr0
	OR eax,1
	MOV cr0,eax
    
    ; Saltar a modo protegido
    jmp 0x48:modoprotegido

BITS 32
	modoprotegido:
        
    ; Establecer selectores de segmentos
    
    mov ax, 0x40
    mov es, ax
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Establecer la base de la pila
    mov esp, 0x27000
    mov ebp, 0x27000


  


    ; Inicializar pantalla
    
    
    ;mov eax, 80
    ;mov edi, 40
    ;mul edi
    ;mov esi, 0xB8000
    ;.pintar:
    ;	cmp eax,0
    ;	jz .salir
    ;	mov word [esi],0XDDAA
    ;	add esi, 2
    ;	dec eax
    ;	jmp .pintar
    ;.salir:
    ;


    ; Imprimir mensaje de bienvenida
    
    print_text_pm start_pm_msg, start_pm_len, 0XEECC, 25,20
    print_text_pm start_pm_msgA, start_pm_lenA, 0XEECC, 27,30
    
    ; Inicializar el manejador de memoria

    call mmu_init
 
    ; Inicializar el directorio de paginas
    call mmu_initKernelDir
    
    ; Cargar directorio de paginas
    mov cr3, eax

    ; Habilitar paginacion
    
    mov eax,cr0
    or eax, 0x80000000
    mov cr0,eax

    ;Imprimir libretas de integrantes

    ; print_text_pm libreta_1, lib1_len, 0X8E, 29,30
    ; print_text_pm libreta_2,lib2_len, 0X8E,30 ,30

    
    ;prueba dirTarea
    ;mov eax,0x00010000
    ;push eax
    ;mov eax, 0x08000000
    ;push eax
    ;call mmu_initTaskDir
    ;
    ;mov cr3, eax

    ;print_text_pm primerChar,primerChar_len, 0x07, 0, 0
    
    
    ; Inicializar tss
    call tss_init
    call gdt_tss_init
    call gdt_tss

    ; Inicializar tss de la tarea Idle
    call sched_init
    ; Inicializar el scheduler

    ; Inicializar la IDT
    
    call idt_init  
    
    ; Cargar IDT

    lidt [IDT_DESC]
    
    ;ROMPER TODO

    
    ; Configurar controlador de interrupciones
    call pic_reset ; remapear PIC
    call pic_enable ; habilitar PIC
    sti        ; habilitar interrupciones

    

    ; Cargar tarea inicial
    mov ax,0x0068
    ltr ax
    jmp 0x0070:0

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
