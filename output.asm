
--- X86 ASSEMBLY OUTPUT ---

.section .data
    fmt_int:   .string "%d\n"
    fmt_float: .string "%f\n"
    fmt_scan:  .string "%d"

.section .bss
    _var_a: .space 4
    _var_b: .space 4
    _tmp_t0: .space 4

.section .text
.globl main
main:
    pushl   %ebp
    movl    %esp, %ebp

    # declare int a

    # declare int b

    # t0 = 5
    movl    $5, %eax
    movl    %eax, _tmp_t0

    # a = t0
    movl    _tmp_t0, %eax
    movl    %eax, _var_a

    # print a
    movl    _var_a, %eax
    pushl   %eax
    pushl   $fmt_int
    call    printf
    addl    $8, %esp

    movl    $0, %eax
    movl    %ebp, %esp
    popl    %ebp
    ret

---------------------------
