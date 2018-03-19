include "inc/snes.inc"
include "inc/memory.inc"

include "inc/data.inc"
include "inc/ppu.inc"
include "inc/oam.inc"
include "inc/rle.inc"
include "inc/init.inc"
include "inc/player/player.inc"

wRAM()
  player.new(sektor)

code()
function Main {
    ppu.copyPalette(palettes.main)
    rle.copy(tilesets.sektor)

    databank($00)

    lda #$01; sta >oam.avail
    jsl ^oam.findSlot

  a16()
    lda #>OBSIZE_32_64; sta >OBSEL // 32x32 sprites

    // random number, don't feel like dealing with this
    //lda #>$02ff
    //sta oam.reserved

    // $50 X and Y
    lda #>$50
    sta >oam.table+0
    sta >oam.table+1

    // starting from sprite $30
    lda #>$30
    sta >oam.table+2
    stz >oam.table+3

  a8()

    lda #$10; sta >BLENDMAIN // enable sprites and plane 0

    lda #$0f; sta >PPUBRIGHT

    // we want nmi
    lda #<VBLANK_NMI|AUTOREAD
    sta >PPUNMI


    cli // enable interrupts

  forever:
  a8()

    lda >JOY1CUR+1
    and #<KEY_RIGHT >> 8
    beq +

    player.right(3)

  +;lda >JOY1CUR+1
    and #<KEY_LEFT >> 8
    beq +

    player.left(3)

  +;lda >JOY1CUR+1
    and #<KEY_UP >> 8
    beq +

    player.up(3)

  +;lda >JOY1CUR+1
    and #<KEY_DOWN >> 8
    beq +

    player.down(3)

  +;

  xy16()
    ldx <oam.reserved
    //jsr >oam.clear
    jsl ^oam.packHi
    jsl ^ppu.sync
    jsl ^oam.copy

    bra forever
}

function rtiPointer {
    rti
}


end()

include "inc/header.inc"
