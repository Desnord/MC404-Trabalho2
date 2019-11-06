.align 4
int_handler:

salva:
    csrrw t6,mscratch,t6
    sw a1, 0(t6)  # salva a1 
    sw a2, 4(t6)  # salva a2 
    sw a3, 8(t6)  # salva a3 
    sw a4, 12(t6) # salva a4
    sw a5, 16(t6) # salva a5
    sw a6, 20(t6) # salva a6
    sw a7, 24(t6) # salva a7
    sw t1, 28(t6) # salva t1 
    sw t2, 32(t6) # salva t2 
    sw t3, 36(t6) # salva t3 
    sw t4, 40(t6) # salva t4
    sw t5, 44(t6) # salva t5
    sw s1, 48(t6) # salva s1
    sw s2, 52(t6) # salva s2
    sw s3, 56(t6) # salva s3
    sw s4, 60(t6) # salva s4
    sw s5, 64(t6) # salva s5
    sw s6, 72(t6) # salva s6
    sw s7, 76(t6) # salva s7
    sw s8, 80(t6) # salva s8
    sw s9, 84(t6) # salva s9
    sw s10, 88(t6) # salva s10 
    sw s11, 92(t6) # salva s11 

# Syscalls
read_ultrasonic_sensor:
        li t1,0xFFFF0020 # endereco do periferico
        sw zero,0(t1) # inicia leitura atribuindo zero ao endereco

        #le 0xFFFF0020 ate que seu valor seja 1
        while_rus:
            li t1,0xFFFF0020
            lw t1,0(t1)

            li t2,1
            #se 0xFFFF0020 for 1 le valor retornado pelo sensor de ultrassom (em 0xFFFF0024)
            bne t1,t2,while_rus
                li t3,0xFFFF0024
                lw a0,0(t3)

restaura:
    lw a1, 0(t6)  # carrega a1 
    lw a2, 4(t6)  # carrega a2 
    lw a3, 8(t6)  # carrega a3 
    lw a4, 12(t6) # carrega a4
    lw a5, 16(t6) # carrega a5
    lw a6, 20(t6) # carrega a6
    lw a7, 24(t6) # carrega a7
    lw t1, 28(t6) # carrega t1 
    lw t2, 32(t6) # carrega t2 
    lw t3, 36(t6) # carrega t3 
    lw t4, 40(t6) # carrega t4
    lw t5, 44(t6) # carrega t5
    lw s1, 48(t6) # carrega s1
    lw s2, 52(t6) # carrega s2
    lw s3, 56(t6) # carrega s3
    lw s4, 60(t6) # carrega s4
    lw s5, 64(t6) # carrega s5
    lw s6, 72(t6) # carrega s6
    lw s7, 76(t6) # carrega s7
    lw s8, 80(t6) # carrega s8
    lw s9, 84(t6) # carrega s9
    lw s10, 88(t6) # carrega s10 
    lw s11, 92(t6) # carrega s11 
    csrrw t6,mscratch,t6 
  
  # arruma volta e retorna ao ponto de execucao anterior
  csrr t0, mepc
  addi t0, t0, 4 
  csrw mepc, t0  
  mret           

.globl _start
_start:
  # Configura o tratador de interrupções
  la t0, int_handler
  csrw mtvec, t0      
  
  # Habilita Interrupções Global
  csrr t1, mstatus # Seta o bit 3 (MIE)
  ori t1, t1, 0x80 # do registrador mstatus
  csrw mstatus, t1

  # Habilita Interrupções Externas
  csrr t1, mie # Seta o bit 11 (MEIE)
  li t2, 0x800 # do registrador mie
  or t1, t1, t2
  csrw mie, t1

  # Ajusta o mscratch
  la t1, save # Coloca o endereço do buffer para salvar
  csrw mscratch, t1 # registradores em mscratch

  # Ajusta pilha 
  li sp, 124436465 #seta o endereço da pilha

  # Muda para o Modo de usuário
  csrr t1, mstatus # Seta os bits 11 e 12 (MPP)
  li t2, 0x1800 # do registrador mstatus
  and t1, t1, t2 # com o valor 00
  csrw mstatus, t1

  la t0, usuario # Grava o endereço do rótulo user
  csrw mepc, t0 # no registrador mepc
  mret # PC <= MEPC; MIE <= MPIE; Muda modo para MPP

usuario:

save: .skip 124
