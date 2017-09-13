namespace ppu {

dPage()
  msg:; res(2)

code()

function copy {
    php
  axy16()
    sta DMAMODE
    stx DMAADDR
    sty DMALEN
  a8()
    phb
    pla
    sta DMAADDRBANK
    lda #%00000001
    sta COPYSTART
    plp
    rts
}


function vBlank {
    phb
    phk
    plb // data bank 0
    bit.w NMISTATUS
    plb
    rti
}


function msgCopy {
  axy16()
    ldy #$0000

  loop:; lda (msg),y
    and #$00ff
    cmp #$00ff
    beq done
    sta PPUDATA
    iny
    bra loop

  done:
    rts
}

function sync {
    php
  a8()
  -;bit VBLSTATUS  // wait for leaving previous vblank
    bmi -
  -;bit VBLSTATUS  // wait for start of this vblank
    bpl -
    plp
    rts
}


macro copyPalette(palette) {
    if _REGS_.aSize != 8 {
        a8()
    }

    lda #$80
    sta CGADDR
    databank({palette})

    if _REGS_.aSize != 16 || _REGS_.iSize != 16 {
        axy16()
    }

    lda.w #DMAMODE_CGDATA
    ldx.w #{palette}
    ldy.w #{palette}.size
    jsr ppu.copy
}

macro printMessage(message, place) {
    databank({message})

    if _REGS_.aSize != 16 {
        a16()
    }

    lda.w #$6000|{place}
    sta PPUADDR
    lda.w #{message}
    sta.b ppu.msg
    jsr ppu.msgCopy
}

}