namespace rle {

dPage()
  src:; res(2)
  index:; res(2)
  count:; res(2)
  remain:; res(2)

bss7e()
  dat:; res(8192) // 8 kb

code()

// Decompresses data to rle_cp_dat using a simple RLE scheme.
// @param DBR:rle_cp_src pointer to compressed data
// @return rle_cp_index = 4
function copyRAM {
    php

  xy16()
  a8()
    ldy #$0000
    sty.b rle.index
    tya  // clear low and high bytes of accumulator

  loop:
    lda (rle.src),y
    cmp #$ff  // RLE data is terminated by a run length of $FF
    beq done  // But what does a run length of 0 do?
    cmp #$fe
    beq copy_fe
    tax
    iny
    lda (rle.src),y
    iny
    phy

    // At this point, Y (source index) is saved on the stack,
    // A is the byte to write, and X is the length of the run.
    txy

    // no higher byte!
    pha
  a16()
    tya
    and #$00ff
    tay
  a8()
    pla

    ldx.b rle.index
    // And here, Y is the length of the run, A is the byte to write,
    // and X is the index into the decompression buffer.
  rle_inter:
    sta rle.dat,x
    inx
    dey
    bne rle_inter

    stx.b rle.index
    ply  // Restore source index
    bra loop

  copy_fe:

    // this code gets a little crazy, so bear with me.

    iny // get number of bytes to copy
    lda (rle.src),y
    iny // don't copy number of bytes to copy!
    sty.b rle.count

    ldx #$0000 // 0 higher bit
    tax // x has bytes remaining
    stx.b rle.remain

    ldx.b rle.index

  fe_loop:
    lda (rle.src),y
    iny // for next byte

    sta rle.dat,x

    // this is a weird way of saying "one less remaining"
    phx
    ldx.b rle.remain
    dex
    stx.b rle.remain
    plx

    // increase the index
    inx

    // now check how much remaining
    lda.b rle.remain
    cmp #$00
    bne fe_loop

    // put the index back
    stx.b rle.index

    bra loop

  done:
    plp
    rts
}


function dma {
    php

  axy16()
    lda #$4000
    sta PPUADDR

    lda #DMAMODE_PPUDATA
    ldx.w #rle.dat
    ldy #8192

  axy16()

    sta DMAMODE
    stx DMAADDR
    sty DMALEN

  a8()
    lda.b #bankbyte(rle.dat)
    sta DMAADDRBANK
    lda #%00000001
    sta COPYSTART

    plp
    rts
}


macro copy(data) {
    databank({data})

    if _REGS_.iSize != 16 {
        xy16()
    }

    ldx.w #{data}
    stx.b rle.src
    jsr rle.copyRAM
    jsr rle.dma
}

}