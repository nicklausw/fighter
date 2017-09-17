// TODO: CREDIT TEPPLES PROPERLY!

namespace oam {

wRAM()
  oamTable:; res(512)
  oamHi:; res(512)

dPage()
  reserved:; res(2)

code()
function clear {
    axy16()
  -;lda.w #(225 << 8) | -128
    sta oamTable,x
    lda #$0100  // bit 8: offscreen
    sta oamHi,x
    inx; inx
    inx; inx
    cpx #512  // 128 sprites times 4 bytes per sprite
    bcc -
    rts
}

function packHi {
    xy16()
    ldx #$0000
    txy
  -;// pack four sprites' size+xhi bits from OAMHI
  a8()
    lda oamHi+13,y
    asl; asl
    ora oamHi+9,y
    asl; asl
    ora oamHi+5,y
    asl; asl
    ora oamHi+1,y
    sta oamHi,x
  a16()

    // move to the next set of 4 OAM entries
    inx
    tya
    clc; adc #16
    tay
  
    // done yet?
    cpx #32  // 128 sprites divided by 4 sprites per byte
    bcc -
    rts
}

function copy {
  axy16()
    lda.w #DMAMODE_OAMDATA
    ldx.w #oamTable
    ldy.w #544
    jmp ppu.copy
}

}