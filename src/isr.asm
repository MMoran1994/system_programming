; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "print.mac"

BITS 32

sched_task_offset:     dd 0xFFFFFFFF
sched_task_selector:   dw 0xFFFF

offset: dd 0
selector: dw 0
;-----------------------------------------------------------------------------;;
;;Funciones externas

global jump_toIdle
extern int137
extern whereIsMorty_posY
extern whereIsMorty_posX
extern portalCreatReset
extern iamRick
extern verificar_modo_debug
extern print_exception
extern killcurrent_task
extern activar_modo_debug
extern imprimir_registros
extern verificar_excepcion
extern activar_desactivar_excepcion
extern verificar_fin_del_juego

;; PIC
extern pic_finish1

;; Sched
extern sched_nextTask

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

global _isr32
global _isr33
global _isr137
global _isr138
global _isr139
%macro ISR 1
global _isr%1


_isr%1:

    ;push stack5 = SS3      12 23
    ;push stack4 = ESP3     11
    ;push stack3 = EFLAGS   10
    ;push stack2 = CS       9
    ;push stack1 = EIP      8
    sub esp, 5*4

    push edx
    push eax
    push es
    
    ;xchg bx, bx
    
    ;obtenemos SS3
    mov eax, [esp+4*12]
    mov es, ax
    ;obtenemos ESP3
    mov eax, [esp+4*11]

    ;no se por que esto funciona
    add eax, 8

    ;buscamos los 5 elementos del stack de nivel 3
    mov edx, [es:eax]
    mov [esp+4*7], edx
    add eax, 4
    
    mov edx, [es:eax] 
    mov [esp+4*6], edx
    add eax, 4

    mov edx, [es:eax] 
    mov [esp+4*5], edx
    add eax, 4

    mov edx, [es:eax] 
    mov [esp+4*4], edx
    add eax, 4

    mov edx, [es:eax] 
    mov [esp+4*3], edx

    pop es
    pop eax
    pop edx

	pushad
    ;colocamos los valores de la tarea en la pila
    ;xchg bx, bx
    mov eax, [esp+4*16] ;ESP3
    mov [esp+4*3], eax

	;push cs
    mov eax, [esp+4*14] ; CS3
    push eax

	push ds
	push es
	push fs
	push gs

	;push ss
    mov eax, [esp+4*22] ; SS3
    push eax

	;pushfd
    mov eax, [esp+4*21] ;EFLAGS
    push eax

    mov eax, [esp+4*20]
    push eax

    call activar_desactivar_excepcion
	call verificar_modo_debug
    cmp eax, 1
    je .modo_debug_activado
	jmp .modo_debug_desactivado

	.modo_debug_activado:
	call imprimir_registros

	.modo_debug_desactivado:
    mov eax, %1
    push eax
    call print_exception
    call killcurrent_task
    jmp $

%endmacro


;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19

;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;



_isr32:
    pushad
    
    call pic_finish1 ; Indica que la interrupcion fue antendida
    call verificar_fin_del_juego
    cmp eax, 1
    je .fin
    call portalCreatReset ; actualiza el booleano que indica si un portal fue creado
    
    call verificar_excepcion
    cmp eax, 1
    jne .seguir

    call verificar_modo_debug
    cmp eax,1
    je .fin
    call activar_desactivar_excepcion
    .seguir:
    call sched_nextTask 
 
    str cx
    
    cmp ax, cx
    
    je .fin

    mov [selector],ax
    jmp far [offset]

    .fin:
    popad
    iret
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

n_0 db     '0'
n0_len equ    $ - n_0

n_1 db     '1'
n1_len equ    $ - n_1

n_2 db     '2'
n2_len equ    $ - n_2

n_3 db     '3'
n3_len equ    $ - n_3

n_4 db     '4'
n4_len equ    $ - n_4

n_5 db     '5'
n5_len equ    $ - n_5

n_6 db     '6'
n6_len equ    $ - n_6

n_7 db     '7'
n7_len equ    $ - n_7

n_8 db     '8'
n8_len equ    $ - n_8

n_9 db     '9'
n9_len equ    $ - n_9



_isr33:
    pushad
    in al, 0x60 ; Captura una tecla

    cmp al, 0x15
    je .activar_modoDebug
    jmp .seguir

    .activar_modoDebug:
    
    call activar_modo_debug

    .seguir:
    
    push eax
    call print0a9 ; Rutina para imprimir las teclas 0-9
    add esp, 4

    call pic_finish1 ; Indica que la interrupcion fue antendida
    popad
    iret

print0a9:
    pushad
    mov eax, [esp + 36]
    
    cmp al, 0X0B
    je .print_0

    cmp al, 0X02
    je .print_1

    cmp al, 0X03
    je .print_2
    
    cmp al, 0X04
    je .print_3

    cmp al, 0X05
    je .print_4

    cmp al, 0X06
    je .print_5

    cmp al, 0X07
    je .print_6

    cmp al, 0X08
    je .print_7

    cmp al,0X09
    je .print_8

    cmp al, 0X0A
    je .print_9
    jmp .fin

    .print_0:
    print_text_pm n_0,n0_len,0x0F, 0, 79
    jmp .fin

    .print_1:
    print_text_pm n_1,n1_len,0x0F, 0, 79
    jmp .fin

    .print_2:
    print_text_pm n_2,n2_len,0x0F, 0, 79
    jmp .fin

    .print_3:
    print_text_pm n_3,n3_len,0x0F, 0, 79
    jmp .fin
    
    .print_4:
    print_text_pm n_4,n4_len,0x0F, 0, 79
    jmp .fin
    
    .print_5:
    print_text_pm n_5,n5_len,0x0F, 0, 79
    jmp .fin
    
    .print_6:
    print_text_pm n_6,n6_len,0x0F, 0, 79
    jmp .fin

    .print_7:
    print_text_pm n_7,n7_len,0x0F, 0, 79
    jmp .fin

    .print_8:
    print_text_pm n_8,n8_len,0x0F, 0, 79
    jmp .fin

    .print_9:
    print_text_pm n_9,n9_len,0x0F, 0, 79

    .fin:
    popad
    ret

;; Rutina de salto a la tarea idle
;;-----------------------------------------------------------------------------;;
jump_toIdle:
	jmp 0X0070:0

 



;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;


;usePortalGun
_isr137: ;eax= x, ebx=y ecx=cross, edx=withMorty
    
    sub esp,4
    pushad
    push edx
    push ecx
    push ebx
    push eax

    call int137
    
    mov [esp+12*4],eax
    
    pop eax
    pop ebx
    pop ecx
    pop edx
    popad
    pop eax
    iret

;IamRick
_isr138: ;ax = code
    pushad
    push eax
    call iamRick
    pop eax
    popad
    iret

;whereIsMorty
_isr139:
	sub esp,8
    pushad

    call whereIsMorty_posY
    
    mov [esp+9*4],eax

    call whereIsMorty_posX

	mov [esp+8*4],eax    
    
    popad
    pop eax
    pop ebx
    ;xchg bx,bx
    iret


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
nextClock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret
