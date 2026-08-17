        cseg
        xdef    _main

_main:
        movea.l #relocation_target-2,a0
        move.w  (a0),d0
        rts

        dseg
relocation_target:
        dc.b    $5a
