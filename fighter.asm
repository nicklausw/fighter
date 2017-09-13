include "inc/snes.inc"
include "inc/fighter.inc"

include "inc/data.inc"
include "inc/ppu.inc"
include "inc/oam.inc"
include "inc/rle.inc"
include "inc/init.inc"

code()
function Main {
    ppu.copyPalette(palettes.main)
    rle.copy(font)

  a16()
    lda.w #$05
    sta oam.reserved
    lda.w #$80
    sta.w oam.oamTable+0
    sta.w oam.oamTable+1
    lda.w #'A'
    sta.w oam.oamTable+2

  a8()

    lda #%00010001  // enable sprites and plane 0
    sta BLENDMAIN

    lda #$0f
    sta PPUBRIGHT

    // we want nmi
    lda.b #VBLANK_NMI|AUTOREAD
    sta PPUNMI


    cli // enable interrupts

  forever:
    wai
    
    xy16()
    ldx oam.reserved
    jsr oam.clear
    jsr oam.packHi
    jsr ppu.sync
    jsr oam.copy

    bra forever
}

function rtiPointer {
    rti
}


end()

include "inc/header.inc"