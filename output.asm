.section .data
    fmt_int:   .string "%d\n"
    fmt_char:  .string "%c\n"
    fmt_float: .string "%f\n"
    fmt_scan:  .string "%d"
    fmt_scanc: .string " %c"
    _str0: .string "=== ARITHMETIC ===\n"
    _str1: .string "=== RELATIONAL ===\n"
    _str2: .string "=== LOGICAL ===\n"
    _str3: .string "=== PRECEDENCE ===\n"
    _str4: .string "=== CONSTANT FOLDING ===\n"
    _str5: .string "=== WHILE LOOP ===\n"
    _str6: .string "=== BREAK AND CONTINUE ===\n"
    _str7: .string "=== IF ELSE CHAIN ===\n"
    _str8: .string "=== CHAR TYPE ===\n"
    _str9: .string "=== SCOPING ===\n"
    _str10: .string "=== NESTED LOOPS ===\n"
    _str11: .string "=== SCAN ===\n"
    _str12: .string "=== DONE ===\n"

.section .bss
    _var_a: .space 4
    _var_b: .space 4
    _var_c: .space 4
    _var_i: .space 4
    _var_result: .space 4
    _var_grade: .space 4
    _var_ch: .space 4
    _var_a_0: .space 4
    _tmp_t0: .space 4
    _tmp_t1: .space 4
    _tmp_t2: .space 4
    _tmp_t3: .space 4
    _tmp_t4: .space 4
    _tmp_t5: .space 4
    _tmp_t6: .space 4
    _tmp_t7: .space 4
    _tmp_t8: .space 4
    _tmp_t9: .space 4
    _tmp_t10: .space 4
    _tmp_t11: .space 4
    _tmp_t12: .space 4
    _tmp_t13: .space 4
    _tmp_t14: .space 4
    _tmp_t15: .space 4
    _tmp_t16: .space 4
    _tmp_t17: .space 4
    _tmp_t27: .space 4
    _tmp_t28: .space 4
    _tmp_t29: .space 4
    _tmp_t30: .space 4
    _tmp_t31: .space 4
    _tmp_t32: .space 4
    _tmp_t33: .space 4
    _tmp_t34: .space 4
    _tmp_t35: .space 4
    _tmp_t36: .space 4
    _tmp_t37: .space 4
    _tmp_t40: .space 4
    _tmp_t41: .space 4
    _tmp_t42: .space 4
    _tmp_t43: .space 4
    _tmp_t44: .space 4

.section .text
.globl main
main:
    pushl   %ebp
    movl    %esp, %ebp

    # declare int a

    # declare int b

    # declare int c

    # declare int i

    # declare int result

    # declare char grade

    # declare char ch

    # a = 15
    movl    $15, %eax
    movl    %eax, _var_a

    # b = 4
    movl    $4, %eax
    movl    %eax, _var_b

    # c = 0
    movl    $0, %eax
    movl    %eax, _var_c

    # print "=== ARITHMETIC ==="
    pushl   $_str0
    call    printf
    addl    $4, %esp

    # t0 = a + b
    movl    _var_a, %eax
    movl    _var_b, %ebx
    addl    %ebx, %eax
    movl    %eax, _tmp_t0

    # c = t0
    movl    _tmp_t0, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t1 = a - b
    movl    _var_a, %eax
    movl    _var_b, %ebx
    subl    %ebx, %eax
    movl    %eax, _tmp_t1

    # c = t1
    movl    _tmp_t1, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t2 = a * b
    movl    _var_a, %eax
    movl    _var_b, %ebx
    imull   %ebx, %eax
    movl    %eax, _tmp_t2

    # c = t2
    movl    _tmp_t2, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t3 = a / b
    movl    _var_a, %eax
    movl    _var_b, %ebx
    movl    %ebx, %ecx
    cdq
    idivl   %ecx
    movl    %eax, _tmp_t3

    # c = t3
    movl    _tmp_t3, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # print "=== RELATIONAL ==="
    pushl   $_str1
    call    printf
    addl    $4, %esp

    # t4 = a > b
    movl    _var_a, %eax
    movl    _var_b, %ebx
    cmpl    %ebx, %eax
    setg    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t4

    # c = t4
    movl    _tmp_t4, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t5 = a < b
    movl    _var_a, %eax
    movl    _var_b, %ebx
    cmpl    %ebx, %eax
    setl    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t5

    # c = t5
    movl    _tmp_t5, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t6 = a >= 15
    movl    _var_a, %eax
    movl    $15, %ebx
    cmpl    %ebx, %eax
    setge   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t6

    # c = t6
    movl    _tmp_t6, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t7 = a <= 14
    movl    _var_a, %eax
    movl    $14, %ebx
    cmpl    %ebx, %eax
    setle   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t7

    # c = t7
    movl    _tmp_t7, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t8 = a == 15
    movl    _var_a, %eax
    movl    $15, %ebx
    cmpl    %ebx, %eax
    sete    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t8

    # c = t8
    movl    _tmp_t8, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t9 = a != 15
    movl    _var_a, %eax
    movl    $15, %ebx
    cmpl    %ebx, %eax
    setne   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t9

    # c = t9
    movl    _tmp_t9, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # print "=== LOGICAL ==="
    pushl   $_str2
    call    printf
    addl    $4, %esp

    # t10 = a > 10
    movl    _var_a, %eax
    movl    $10, %ebx
    cmpl    %ebx, %eax
    setg    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t10

    # t11 = b > 2
    movl    _var_b, %eax
    movl    $2, %ebx
    cmpl    %ebx, %eax
    setg    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t11

    # t12 = t10 && t11
    movl    _tmp_t10, %eax
    movl    _tmp_t11, %ebx
    testl   %eax, %eax
    setne   %al
    movzbl  %al, %eax
    testl   %ebx, %ebx
    setne   %bl
    movzbl  %bl, %ebx
    andl    %ebx, %eax
    movl    %eax, _tmp_t12

    # c = t12
    movl    _tmp_t12, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t13 = a < 10
    movl    _var_a, %eax
    movl    $10, %ebx
    cmpl    %ebx, %eax
    setl    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t13

    # t14 = b > 2
    movl    _var_b, %eax
    movl    $2, %ebx
    cmpl    %ebx, %eax
    setg    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t14

    # t15 = t13 || t14
    movl    _tmp_t13, %eax
    movl    _tmp_t14, %ebx
    testl   %eax, %eax
    setne   %al
    movzbl  %al, %eax
    testl   %ebx, %ebx
    setne   %bl
    movzbl  %bl, %ebx
    orl     %ebx, %eax
    movl    %eax, _tmp_t15

    # c = t15
    movl    _tmp_t15, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t16 = a == 15
    movl    _var_a, %eax
    movl    $15, %ebx
    cmpl    %ebx, %eax
    sete    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t16

    # t17 = t16 == 0
    movl    _tmp_t16, %eax
    movl    $0, %ebx
    cmpl    %ebx, %eax
    sete    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t17

    # c = t17
    movl    _tmp_t17, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # print "=== PRECEDENCE ==="
    pushl   $_str3
    call    printf
    addl    $4, %esp

    # c = 14
    movl    $14, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # c = 20
    movl    $20, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # c = 4
    movl    $4, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # print "=== CONSTANT FOLDING ==="
    pushl   $_str4
    call    printf
    addl    $4, %esp

    # c = 15
    movl    $15, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # c = 20
    movl    $20, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # c = 1
    movl    $1, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # print "=== WHILE LOOP ==="
    pushl   $_str5
    call    printf
    addl    $4, %esp

    # i = 1
    movl    $1, %eax
    movl    %eax, _var_i

    # result = 0
    movl    $0, %eax
    movl    %eax, _var_result

L0:

    # t27 = i <= 5
    movl    _var_i, %eax
    movl    $5, %ebx
    cmpl    %ebx, %eax
    setle   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t27

    # if_false t27 goto L1
    movl    _tmp_t27, %eax
    testl   %eax, %eax
    je      L1

    # t28 = result + i
    movl    _var_result, %eax
    movl    _var_i, %ebx
    addl    %ebx, %eax
    movl    %eax, _tmp_t28

    # result = t28
    movl    _tmp_t28, %eax
    movl    %eax, _var_result

    # t29 = i + 1
    movl    _var_i, %eax
    movl    $1, %ebx
    addl    %ebx, %eax
    movl    %eax, _tmp_t29

    # i = t29
    movl    _tmp_t29, %eax
    movl    %eax, _var_i

    jmp     L0

L1:

    # print result
    movl    _var_result, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # print "=== BREAK AND CONTINUE ==="
    pushl   $_str6
    call    printf
    addl    $4, %esp

    # i = 1
    movl    $1, %eax
    movl    %eax, _var_i

L2:

    # t30 = i <= 10
    movl    _var_i, %eax
    movl    $10, %ebx
    cmpl    %ebx, %eax
    setle   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t30

    # if_false t30 goto L3
    movl    _tmp_t30, %eax
    testl   %eax, %eax
    je      L3

    # t31 = i == 3
    movl    _var_i, %eax
    movl    $3, %ebx
    cmpl    %ebx, %eax
    sete    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t31

    # if_false t31 goto L4
    movl    _tmp_t31, %eax
    testl   %eax, %eax
    je      L4

    # t32 = i + 1
    movl    _var_i, %eax
    movl    $1, %ebx
    addl    %ebx, %eax
    movl    %eax, _tmp_t32

    # i = t32
    movl    _tmp_t32, %eax
    movl    %eax, _var_i

    jmp     L2

L4:

    # t33 = i == 6
    movl    _var_i, %eax
    movl    $6, %ebx
    cmpl    %ebx, %eax
    sete    %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t33

    # if_false t33 goto L6
    movl    _tmp_t33, %eax
    testl   %eax, %eax
    je      L6

    jmp     L3

L6:

    # print i
    movl    _var_i, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t34 = i + 1
    movl    _var_i, %eax
    movl    $1, %ebx
    addl    %ebx, %eax
    movl    %eax, _tmp_t34

    # i = t34
    movl    _tmp_t34, %eax
    movl    %eax, _var_i

    jmp     L2

L3:

    # print "=== IF ELSE CHAIN ==="
    pushl   $_str7
    call    printf
    addl    $4, %esp

    # result = 75
    movl    $75, %eax
    movl    %eax, _var_result

    # t35 = result >= 90
    movl    _var_result, %eax
    movl    $90, %ebx
    cmpl    %ebx, %eax
    setge   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t35

    # if_false t35 goto L8
    movl    _tmp_t35, %eax
    testl   %eax, %eax
    je      L8

    # print 1
    movl    $1, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    jmp     L9

L8:

    # t36 = result >= 75
    movl    _var_result, %eax
    movl    $75, %ebx
    cmpl    %ebx, %eax
    setge   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t36

    # if_false t36 goto L10
    movl    _tmp_t36, %eax
    testl   %eax, %eax
    je      L10

    # print 2
    movl    $2, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    jmp     L11

L10:

    # t37 = result >= 60
    movl    _var_result, %eax
    movl    $60, %ebx
    cmpl    %ebx, %eax
    setge   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t37

    # if_false t37 goto L12
    movl    _tmp_t37, %eax
    testl   %eax, %eax
    je      L12

    # print 3
    movl    $3, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    jmp     L13

L12:

    # print 4
    movl    $4, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

L13:

L11:

L9:

    # print "=== CHAR TYPE ==="
    pushl   $_str8
    call    printf
    addl    $4, %esp

    # ch = 65
    movl    $65, %eax
    movl    %eax, _var_ch

    # print ch (char)
    movl    _var_ch, %eax
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

    # ch = 122
    movl    $122, %eax
    movl    %eax, _var_ch

    # print ch (char)
    movl    _var_ch, %eax
    cmpl    $32, %eax
    jl      _print_as_int_1
    cmpl    $126, %eax
    jg      _print_as_int_1
    pushl   %eax
    pushl   $fmt_char
    call    printf
    addl    $8, %esp
    jmp     _print_done_1
_print_as_int_1:
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp
_print_done_1:

    # ch = 66
    movl    $66, %eax
    movl    %eax, _var_ch

    # print ch (char)
    movl    _var_ch, %eax
    cmpl    $32, %eax
    jl      _print_as_int_2
    cmpl    $126, %eax
    jg      _print_as_int_2
    pushl   %eax
    pushl   $fmt_char
    call    printf
    addl    $8, %esp
    jmp     _print_done_2
_print_as_int_2:
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp
_print_done_2:

    # c = 25
    movl    $25, %eax
    movl    %eax, _var_c

    # print c
    movl    _var_c, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # print "=== SCOPING ==="
    pushl   $_str9
    call    printf
    addl    $4, %esp

    # a = 100
    movl    $100, %eax
    movl    %eax, _var_a

    # declare int a_0

    # a_0 = 999
    movl    $999, %eax
    movl    %eax, _var_a_0

    # print a_0
    movl    _var_a_0, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # print a
    movl    _var_a, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # print "=== NESTED LOOPS ==="
    pushl   $_str10
    call    printf
    addl    $4, %esp

    # i = 1
    movl    $1, %eax
    movl    %eax, _var_i

L14:

    # t40 = i <= 3
    movl    _var_i, %eax
    movl    $3, %ebx
    cmpl    %ebx, %eax
    setle   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t40

    # if_false t40 goto L15
    movl    _tmp_t40, %eax
    testl   %eax, %eax
    je      L15

    # b = 1
    movl    $1, %eax
    movl    %eax, _var_b

L16:

    # t41 = b <= 3
    movl    _var_b, %eax
    movl    $3, %ebx
    cmpl    %ebx, %eax
    setle   %al
    movzbl  %al, %eax
    movl    %eax, _tmp_t41

    # if_false t41 goto L17
    movl    _tmp_t41, %eax
    testl   %eax, %eax
    je      L17

    # t42 = i * b
    movl    _var_i, %eax
    movl    _var_b, %ebx
    imull   %ebx, %eax
    movl    %eax, _tmp_t42

    # result = t42
    movl    _tmp_t42, %eax
    movl    %eax, _var_result

    # print result
    movl    _var_result, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # t43 = b + 1
    movl    _var_b, %eax
    movl    $1, %ebx
    addl    %ebx, %eax
    movl    %eax, _tmp_t43

    # b = t43
    movl    _tmp_t43, %eax
    movl    %eax, _var_b

    jmp     L16

L17:

    # t44 = i + 1
    movl    _var_i, %eax
    movl    $1, %ebx
    addl    %ebx, %eax
    movl    %eax, _tmp_t44

    # i = t44
    movl    _tmp_t44, %eax
    movl    %eax, _var_i

    jmp     L14

L15:

    # print "=== SCAN ==="
    pushl   $_str11
    call    printf
    addl    $4, %esp

    # scan a
    pushl   $_var_a
    pushl   $fmt_scan
    call    scanf
    addl    $8, %esp

    # print a
    movl    _var_a, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    # print "=== DONE ==="
    pushl   $_str12
    call    printf
    addl    $4, %esp

    movl    $0, %eax
    movl    %ebp, %esp
    popl    %ebp
    ret
