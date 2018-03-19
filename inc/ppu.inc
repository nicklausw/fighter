namespace ppu {

code()

// the neatest way for this function to
// exist in hirom does involve usage of
// long addresses, sadly.
function copy {
    php
  axy16()
    sta ^DMAMODE
    txa; sta ^DMAADDR
    tya; sta ^DMALEN
  a8()
    phb
    pla
    sta ^DMAADDRBANK
    lda #%00000001
    sta ^COPYSTART
    plp
    rtl
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
    rtl
}


macro copyPalette(palette) {
    if _REGS_.aSize != 8 {
        a8()
    }

    lda #$80
    sta ^CGADDR
    databank({palette})

    if _REGS_.aSize != 16 || _REGS_.iSize != 16 {
        axy16()
    }

    lda #>DMAMODE_CGDATA
    ldx #>{palette}
    ldy #>{palette}.size
    jsl ^ppu.copy
    databank($00)
}

}
