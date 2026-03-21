.section .data
    fmt_int:   .string "%d\n"
    fmt_char:  .string "%c\n"
    fmt_float: .string "%f\n"
    fmt_scan:  .string "%d"
    fmt_scanc: .string " %c"
    _str0: .string "sum of even numbers:\n"
    _str1: .string "grade:\n"

.section .bss
    _var_n: .space 4
    _var_i: .space 4
    _var_sum: .space 4
    _var_isEven: .space 4
    _var_grade: .space 4
    _tmp_t0: .space 4
    _tmp_t1: .space 4
    _tmp_t2: .space 4
    _tmp_t3: .space 4
    _tmp_t4: .space 4
    _tmp_t5: .space 4
    _tmp_t6: .space 4
    _tmp_t7: .space 4
    _tmp_t8: .space 4

.section .text
.globl main
main:
    pushl   %ebp
    movl    %esp, %ebp

    # declare int n

    # declare int i

    # declare int sum

    # declare int isEven

    # declare char grade

    # sum = 0
    movl    $0, %eax
    movl    %eax, _var_sum

    # i = 1
    movl    $1, %eax
    movl    %eax, _var_i

    # n = 10
    movl    $10, %eax
    movl    %eax, _var_n

L0:

    # t0 = i <= n
    movl    _var_i, %eax
    movl    _var_n, %ebx
    cmpl    %ebx, %eax
    setle   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t0

    # if_false t0 goto L1
    movl    _tmp_t0, %eax
    testl   %eax, %eax
    je      L1

    # t1 = i / 2
    movl    _var_i, %eax
    movl    $2, %ebx
    movl    %ebx, %ecx
    cdq
    idivl   %ecx
    movl    %eax, _tmp_t1

    # t2 = t1 * 2
    movl    _tmp_t1, %eax
    movl    $2, %ebx
    imull   %ebx, %eax
    movl    %eax, _tmp_t2

    # t3 = i - t2
    movl    _var_i, %eax
    movl    _tmp_t2, %ebx
    subl    %ebx, %eax
    movl    %eax, _tmp_t3

    # isEven = t3
    movl    _tmp_t3, %eax
    movl    %eax, _var_isEven

    # t4 = isEven == 0
    movl    _var_isEven, %eax
    movl    $0, %ebx
    cmpl    %ebx, %eax
    sete    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t4

    # if_false t4 goto L2
    movl    _tmp_t4, %eax
    testl   %eax, %eax
    je      L2

    # t5 = sum + i
    movl    _var_sum, %eax
    movl    _var_i, %ebx
    addl    %ebx, %eax
    movl    %eax, _tmp_t5

    # sum = t5
    movl    _tmp_t5, %eax
    movl    %eax, _var_sum

L2:

    # t6 = i + 1
    movl    _var_i, %eax
    movl    $1, %ebx
    addl    %ebx, %eax
    movl    %eax, _tmp_t6

    # i = t6
    movl    _tmp_t6, %eax
    movl    %eax, _var_i

    jmp     L0

L1:

    # print "sum of even numbers:"
    pushl   $_str0
    call    printf
    addl    $4, %esp

    # print sum
    movl    _var_sum, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t7 = sum >= 30
    movl    _var_sum, %eax
    movl    $30, %ebx
    cmpl    %ebx, %eax
    setge   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t7

    # if_false t7 goto L4
    movl    _tmp_t7, %eax
    testl   %eax, %eax
    je      L4

    # grade = 65
    movl    $65, %eax
    movl    %eax, _var_grade

    jmp     L5

L4:

    # t8 = sum >= 20
    movl    _var_sum, %eax
    movl    $20, %ebx
    cmpl    %ebx, %eax
    setge   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t8

    # if_false t8 goto L6
    movl    _tmp_t8, %eax
    testl   %eax, %eax
    je      L6

    # grade = 66
    movl    $66, %eax
    movl    %eax, _var_grade

    jmp     L7

L6:

    # grade = 67
    movl    $67, %eax
    movl    %eax, _var_grade

L7:

L5:

    # print "grade:"
    pushl   $_str1
    call    printf
    addl    $4, %esp

    # print grade (char)
    movl    _var_grade, %eax
    cmpl    $32, %eax
    jl      _print_as_int_0
    cmpl    $126, %eax
    jg      _print_as_int_0
    pushl   %eax
    pushl   $fmt_char
    call    printf
    addl    $8, %esp
    jmp     _print_done_0
_print_as_int_0:
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp
_print_done_0:

    movl    $0, %eax
    movl    %ebp, %esp
    popl    %ebp
    ret
