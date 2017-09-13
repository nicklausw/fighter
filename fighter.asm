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
    rle.copy(tilesets.sektor)

  a16()
    // 32x32 sprites
    lda.w #%10100000 | ($4000 >> 13)
    sta OBSEL

    // random number, don't feel like dealing with this
    lda.w #$02ff
    sta oam.reserved

    // $50 X and Y
    lda.w #$50
    sta.w oam.oamTable+0
    sta.w oam.oamTable+1

    // starting from sprite $32
    lda.w #$32
    sta.w oam.oamTable+2

    // using 8th palette
    lda.w #8<<1
    sta.w oam.oamTable+3

  a8()

    lda #%00010000  // enable sprites and plane 0
    sta BLENDMAIN

    lda #$0f
    sta PPUBRIGHT

    // we want nmi
    lda.b #VBLANK_NMI|AUTOREAD
    sta PPUNMI


    cli // enable interrupts

  forever:
  a8()

    lda JOY1CUR+1
    and.b #KEY_RIGHT >> 8
    beq +

    lda oam.oamTable+0
    clc; adc #$03
    sta oam.oamTable+0

  +;lda JOY1CUR+1
    and.b #KEY_LEFT >> 8
    beq +

    lda oam.oamTable+0
    sec; sbc #$03
    sta oam.oamTable+0

  +;
    
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