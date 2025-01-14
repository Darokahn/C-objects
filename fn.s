.globl fn

.section .text

fn:
    movabs $0, %r10
    movabs $0, %r11
    call *%r10
    ret
