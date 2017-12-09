namespace ppu {

code()

function copy {
    php
  axy16()
    sta >DMAMODE
    stx >DMAADDR
    sty >DMALEN
  a8()
    phb
    pla
    sta >DMAADDRBANK
    lda #%00000001
    sta >COPYSTART
    plp
    rts
}


function vBlank {
    phb
    phk; plb // data bank 0
    bit >NMISTATUS
    plb
    rti
}

function sync {
    php
  a8()
  -;bit >VBLSTATUS  // wait for leaving previous vblank
    bmi -
  -;bit >VBLSTATUS  // wait for start of this vblank
    bpl -
    plp
    rts
}


macro copyPalette(palette) {
    if _REGS_.aSize != 8 {
        a8()
    }

    lda #$80
    sta >CGADDR
    databank({palette})

    if _REGS_.aSize != 16 || _REGS_.iSize != 16 {
        axy16()
    }

    lda #>DMAMODE_CGDATA
    ldx #>{palette}
    ldy #>{palette}.size
    jsr >ppu.copy
    databank($00)
}

}
