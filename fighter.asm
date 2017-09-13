include "inc/snes.inc"
include "inc/fighter.inc"

include "inc/data.inc"
include "inc/ppu.inc"
include "inc/rle.inc"
include "inc/init.inc"

code()
function Main {
    ppu.copyPalette(palettes.main)
    rle.copy(font)
    ppu.printMessage(message, NTXY(1,1))

  a8()

    lda #%00000001  // enable sprites and plane 0
    sta BLENDMAIN

    lda #$00
    sta PPUBRIGHT

    // we want nmi
    lda.b #VBLANK_NMI|AUTOREAD
    sta PPUNMI


    cli // enable interrupts

    // now to fade in
    lda #$01
  fade_in:
    wai // wait a frame
    wai // another!
    sta PPUBRIGHT
    inc
    cmp #$10
    bne fade_in

  forever:
    wai
    bra forever
}

function rtiPointer {
    rti
}


end()

include "inc/header.inc"