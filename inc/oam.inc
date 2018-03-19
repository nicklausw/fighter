// TODO: CREDIT TEPPLES PROPERLY!

namespace oam {

// the SNES has so much wRAM that I don't
// feel bad at all about below.

wRAM()
  table:; res(512)
  hiTable:; res(512)
  avail:; res(512)

dPage()
  reserved:; res(2)

code()
function clear {
    axy16()
  -;lda #>(225 << 8) | -128
    sta >table,x
    lda #$0100  // bit 8: offscreen
    sta >hiTable,x
    inx; inx; inx; inx
    cpx #>512  // 128 sprites times 4 bytes per sprite
    bcc -
    rtl
}

function packHi {
    xy16()
    ldx #$0000
    txy
  -;// pack four sprites' size+xhi bits from OAMHI
  a8()
    lda >hiTable+13,y
    asl; asl
    ora >hiTable+9,y
    asl; asl
    ora >hiTable+5,y
    asl; asl
    ora >hiTable+1,y
    sta >hiTable,x
  a16()

    // move to the next set of 4 OAM entries
    inx
    tya
    clc; adc #>16
    tay

    // done yet?
    cpx #>32  // 128 sprites divided by 4 sprites per byte
    bcc -
    rtl
}

function copy {
    databank($00)
  axy16()
    lda #>DMAMODE_OAMDATA
    ldx #>table
    ldy #> 512 + 32
    jml ^ppu.copy
}

// findSlot
// returns accumulator with an open sprite slot.
// eats a, x.
function findSlot {
  a8(); xy16()
    ldx #$0000
  -;lda >avail,x
    inx
    cpx #>512; beq + // return anyway if no slots for now.
    bit #$01; bne -
  +;
  a16()
    dex; txa; rtl
}

}
