.section .data
    fmt_int:   .string "%d\n"
    fmt_float: .string "%f\n"
    fmt_scan:  .string "%d"
    _str0: .string "Enter first number\n"
    _str1: .string "Enter second number\n"
    _str2: .string "Sum is\n"
    _str3: .string "Product is\n"

.section .bss
    _var_a: .space 4
    _var_b: .space 4
    _var_result: .space 4
    _tmp_t0: .space 4
    _tmp_t1: .space 4
    _tmp_t2: .space 4
    _tmp_t3: .space 4

.section .text
.globl main

add:
    pushl   %ebp
    movl    %esp, %ebp


    # declare int a

    # declare int b

    # t0 = a + b
    movl    _var_a, %eax
    movl    _var_b, %ebx
    addl    %ebx, %eax
    movl    %eax, _tmp_t0

    # return t0
    movl    _tmp_t0, %eax
    movl    %ebp, %esp
    popl    %ebp
    ret

    # end of function add
    movl    $0, %eax
    movl    %ebp, %esp
    popl    %ebp
    ret



multiply:
    pushl   %ebp
    movl    %esp, %ebp


    # declare int a

    # declare int b

    # t1 = a * b
    movl    _var_a, %eax
    movl    _var_b, %ebx
    imull   %ebx, %eax
    movl    %eax, _tmp_t1

    # return t1
    movl    _tmp_t1, %eax
    movl    %ebp, %esp
    popl    %ebp
    ret

    # end of function multiply
    movl    $0, %eax
    movl    %ebp, %esp
    popl    %ebp
    ret

main:
    pushl   %ebp
    movl    %esp, %ebp


    # declare int result

    # declare int a

    # declare int b

    # print "Enter first number"
    pushl   $_str0
    call    printf
    addl    $4, %esp

    # scan a
    pushl   $_var_a
    pushl   $fmt_scan
    call    scanf
    addl    $8, %esp

    # print "Enter second number"
    pushl   $_str1
    call    printf
    addl    $4, %esp

    # scan b
    pushl   $_var_b
    pushl   $fmt_scan
    call    scanf
    addl    $8, %esp

    # param a
    movl    _var_a, %eax
    pushl   %eax

    # param b
    movl    _var_b, %eax
    pushl   %eax

    # t2 = call add 2
    call    add
    addl    $8, %esp
    movl    %eax, _tmp_t2

    # result = t2
    movl    _tmp_t2, %eax
    movl    %eax, _var_result

    # print "Sum is"
    pushl   $_str2
    call    printf
    addl    $4, %esp

    # print result
    movl    _var_result, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # param a
    movl    _var_a, %eax
    pushl   %eax

    # param b
    movl    _var_b, %eax
    pushl   %eax

    # t3 = call multiply 2
    call    multiply
    addl    $8, %esp
    movl    %eax, _tmp_t3

    # result = t3
    movl    _tmp_t3, %eax
    movl    %eax, _var_result

    # print "Product is"
    pushl   $_str3
    call    printf
    addl    $4, %esp

    # print result
    movl    _var_result, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    movl    $0, %eax
    movl    %ebp, %esp
    popl    %ebp
    ret
