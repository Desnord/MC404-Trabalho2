#-----------------------------------------------------------------------------------------------
.globl puts
.globl set_time
.globl get_time
.globl get_gyro_angles
.globl get_current_GPS_position
.globl get_us_distance
.globl set_head_servo
.globl set_engine_torque
.globl set_torque

set_torque:
    #checa a validade dos valores
    li t0, -100
    blt a0, t0, invalid_value
    blt a1, t0, invalid_value
    li t0, 100
    bgt a0, t0, invalid_value
    bgt a1, t0, invalid_value

    mv t0, a0
    mv t1, a1

    li a0, 0
    mv a1, t0
    li a7, 18
    ecall

    li a0, 1
    mv a1, t1
    li a7, 18
    ecall

    ret

    invalid_value:
        li a0, -1
        ret
#-----------------------------------------------------------------------------------------------s
set_engine_torque:
    li a7, 18
    ecall
    ret
#-----------------------------------------------------------------------------------------------
set_head_servo:
    li a7, 17
    ecall
    ret
#-----------------------------------------------------------------------------------------------
get_us_distance:
    li a7, 16
    ecall 
    ret
#-----------------------------------------------------------------------------------------------
get_current_GPS_position:
    li a7, 19
    ecall
    ret
#-----------------------------------------------------------------------------------------------
get_gyro_angles:
    li a7, 20
    ecall
    ret
#-----------------------------------------------------------------------------------------------
get_time:
    li a7, 21
    ecall
    ret
#-----------------------------------------------------------------------------------------------
set_time:
    li a7, 22
    ecall
    ret
#-----------------------------------------------------------------------------------------------
puts:
    # puts só tem 1 parametro (a0), e esse parametro tem o endereco da string a ser escrita

    mv t1,a0 # o parametro tem a string a ser escrita

    #descobre o tamanho da string, ou seja, conta até encontrar '\0' == (0) == (null)
    li t2,0
    while_not_EOS:
        lb t3,0(t1)
        beq t3,zero,byte_null
                addi t2,t2,1 # avanca contagem de tamanho
                addi t1,t1,1 # passa para o proximo byte
            j while_not_EOS
        byte_null:

    mv t1,a0 # o parametro tem a string a ser escrita

    # prepara syscall
    li a0,1 # [1 = output] || [0 = input] //// file descriptor
    mv a1,t1 # poe endereco da string a ser escrita em a1
    mv a2,t2 # poe o tamanho encontrado da string no parametro 
    li a7,64 # syscall write
    ecall
    ret
#-----------------------------------------------------------------------------------------------

