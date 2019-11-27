#-----------------------------------------------------------------------------------------------
.equ GPT_GEN, 0xFFFF0100 
.equ GPT_FLAG, 0xFFFF0104
#-----------------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------
int_handler:
    salva:
        csrrw t6,mscratch,t6
        addi t6, t6, -104
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
        sw a0, 96(t6)   #salva a0
        sw t0, 100(t6)   #salva a0

    #ve se é interrupcao do gpt
    csrr t0, mcause
    blt t0, zero, gpt_treatment

    li t0, 16
    beq t0, a7, read_ultrasonic_sensor

    li t0, 17
    beq t0, a7, set_servo_angles

    li t0, 18
    beq t0, a7, set_engine_torque

    li t0, 19
    beq t0, a7, read_gps

    li t0, 20
    beq t0, a7, read_gyroscope

    li t0, 21
    beq t0, a7, get_time

    li t0, 22
    beq t0, a7, set_time
    
    li t0, 64
    beq t0, a7, write
#-----------------------------------------------------------------------------------------------
ret_syscall: 
    restaura:

        # arruma mepc para retornar ao ponto de execucao anterior
        csrr t0, mepc
        addi t0, t0, 4 
        csrw mepc, t0  

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
        lw t0, 100(t6)   #carrega t0
        addi t6, t6, 104
        csrrw t6,mscratch,t6 

        mret     
#-----------------------------------------------------------------------------------------------
ret_gpt: 
    restaura2:

        # arruma mepc para retornar ao ponto de execucao anterior
        csrr t0, mepc
        csrw mepc, t0  

        lw a1, 0(t6)  # carrega a1 
        lw a0, 96(t6)   #carrega a0
        lw t0, 100(t6)   #carrega t0
        addi t6, t6, 104

        csrrw t6,mscratch,t6 

        mret     
#-----------------------------------------------------------------------------------------------
gpt_treatment: 
    #checa se ha interrupcoes nao tratadas
    la a0, GPT_FLAG
    lb a0, 0(a0)
    beq zero, a0, ret_gpt 

    #adiciona 1 no tempo
    la a0, rot_tempo
    lw a1, 0(a0)
    addi a1, a1, 1
    sw a1, 0(a0)

    #set do valor para 0 (interrupção tratada)
    la a0, GPT_FLAG
    sb zero, 0(a0)

    #manda esperar 100ms
    la a0, GPT_GEN
    li t0, 100
    sw t0, 0(a0)

    j ret_gpt

#-----------------------------------------------------------------------------------------------
set_servo_angles:
#parâmetros: a0(id do servo), a1(ângulo para o servo)
#retorno: a0(-1 se o ângulo é inválido, -2 se o id é inválido , 0 caso contrário)

    li t0, 0
    beq a0, t0, servo_base
    li t0, 1
    beq a0, t0, servo_mid
    li t0, 2
    beq a0, t0, servo_top

    #se não pulou, id é inválido
    li a0, -2

    j ret_syscall

    #checa se o angulo é valido e seta o angulo
    servo_base:
        li t0, 16
        blt a1, t0, erro_angulo_servo
        li t0, 116
        bgt a1, t0, erro_angulo_servo
        li a0, 0xFFFF001E
        sb a1, 0(a0)
        j ret_syscall
    servo_mid:
        li t0, 52
        blt a1, t0, erro_angulo_servo
        li t0, 90
        bgt a1, t0, erro_angulo_servo
        li a0, 0xFFFF001D
        sb a1, 0(a0)
        j ret_syscall
    servo_top:
        blt a1, zero, erro_angulo_servo
        li t0, 156
        bgt a1, t0, erro_angulo_servo
        li a0, 0xFFFF001C
        sb a1, 0(a0)
        j ret_syscall

    erro_angulo_servo:
        li a0, -1
        j ret_syscall
#-----------------------------------------------------------------------------------------------
set_engine_torque:
    #parâmetros: a0(id do motor), a1(torque do motor)
    #retorno: a0(-1 se o id for invalido, 0 caso contrario)

    li t0, 0
    beq t0, a0, motor_1
    li t0, 1
    beq t0, a0, motor_2
    
    #id inválido
    li a0, -1
    j ret_syscall

    motor_1:
        li a0, 0xFFFF001A
        sh a1, 0(a0)
        li a0, 0
        j ret_syscall
    motor_2:
        li a0, 0xFFFF0018
        sh a1, 0(a0)
        li a0, 0
        j ret_syscall
#-----------------------------------------------------------------------------------------------
read_gps:
    #parâmetros: a0(endereço do registro)
    #retorno: nada

    li a1, 0xFFFF0004
    sw zero, 0(a1)  
    li t0, 1
    checa_resultado:
        lw a2, 0(a1)
        beq a2, t0, ret_syscall_checagem
        j checa_resultado
    
    ret_syscall_checagem:
        li a1, 0xFFFF0008
        lw a1, 0(a1)
        sw a1, 0(a0)

        li a1, 0xFFFF000C	
        lw a1, 0(a1)
        sw a1, 4(a0)
    
        li a1, 0xFFFF0010	
        lw a1, 0(a1)
        sw a1, 8(a0)

    j ret_syscall
#-----------------------------------------------------------------------------------------------
read_gyroscope:
    #parâmetros: a0(endereço do registro)
    #retorno: nada

    li a1, 0xFFFF0004
    sw zero, 0(a1)  
    li t0, 1
    checa_resultado2:       #checa se o valor é 1
        lw a2, 0(a1)
        beq a2, t0, ret_syscall_checagem2
        j checa_resultado2

    ret_syscall_checagem2:
        li a1, 0xFFFF0014
        lw a1, 0(a1)
        mv t0, a1
        mv t1, a1
        
        #pega x
        srli a1, a1, 20

        #pega y
        slli t0, t0, 12
        srli t0, t0, 22

        #pega z
        slli t1, t1, 22
        srli t1, t1, 22

        sw a1, 0(a0)
        sw t0, 4(a0)
        sw t1, 8(a0)

        j ret_syscall
#-----------------------------------------------------------------------------------------------
get_time:
    la a0, rot_tempo
    lw a0, 0(a0)
    j ret_syscall
#-----------------------------------------------------------------------------------------------
set_time:
    la a1, rot_tempo
    sw a0, 0(a1)
    j ret_syscall
#-----------------------------------------------------------------------------------------------
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
  j ret_syscall
#-----------------------------------------------------------------------------------------------
write:
  li t1,1
  bne a0,t1,not_stdout
    # escreve [valor em a2] bytes
    li t3,0
    while_write:
      bge t3,a2,not_stdout
        lb t4,0(a1) # le byte atual

        # poe o byte no endereco do periferico
        li t1,0xFFFF0109
        sb t4,0(t1)

        # atribui valor 1 ao periferico para iniciar transmissao
        li t2,0xFFFF0108
        li t1,1
        sb t1,0(t2)

        # espera o valor do periferico ser 0 para prosseguir com a escrita
        while_wait_write:
            lb t1,0(t2)
            beq t1,zero,panama
              j while_wait_write
            panama:

        addi t3,t3,1 # prossegue ao proximo byte
        addi a1,a1,1 # prossegue ao proximo endereco 
        j while_write

    mv a0,t3
  not_stdout:
  j ret_syscall
#-----------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------
.globl _start
_start:
    # Configura o tratador de interrupções
    la t0, int_handler # Grava o endereço do rótulo int_handler
    csrw mtvec, t0 # no registrador mtvec

    # Habilita Interrupções Global
    csrr t1, mstatus # Seta o bit 7 (MPIE)
    ori t1, t1, 0x80 # do registrador mstatus
    csrw mstatus, t1

    # Habilita Interrupções Externas
    csrr t1, mie # Seta o bit 11 (MEIE)
    li t2, 0x800 # do registrador mie
    or t1, t1, t2
    csrw mie, t1

    # Ajusta o mscratch
    li t1, 131072000 # Coloca o endereço do buffer para salvar (pilha do sistema)
    csrw mscratch, t1 # registradores em mscratch

    # Pilha do programa (user)
    li t1, 104857600
    mv sp, t1

    gpt_setup:
        la t0, rot_tempo    #set do tempo em 0
        li t1, 0
        sw t1, 0(t0)
        la t1, GPT_GEN  #gera a primeira interrupção
        li t0, 100
        sw t0, 0(t1)
    
    # seta torques em 0

    # seta torque motor 1 = 0
    li a0,0
    li a1,0
    li a7,18
    ecall

    # seta torque motor 2 = 0
    li a0,1
    li a1,0
    li a7,18
    ecall

    # seta posicao do corpo do ouli
    
     # seta BASE
    li a0,0
    li a1,31
    li a7,17
    ecall

    # seta MID
    li a0,1
    li a1,80
    li a7,17
    ecall

    # seta TOP
    li a0,2
    li a1,78
    li a7,17
    ecall

    # Muda para o Modo de usuário
    csrr t1, mstatus # Seta os bits 11 e 12 (MPP)
    li t2, ~0x1800 # do registrador mstatus
    and t1, t1, t2 # com o valor 00
    csrw mstatus, t1

    la t0, main # Grava o endereço do rótulo user
    csrw mepc, t0 # no registrador mepc

    mret # PC <= MEPC; MIE <= MPIE; Muda modo para MPP

    loop:
        j loop
#-----------------------------------------------------------------------------------------------

rot_tempo: .skip 4
reg_buffer: .skip 200
