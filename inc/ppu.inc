namespace ppu {

dPage()
  msg:; res(2)

code()

function copy {
    php
    setaxy16()
    sta DMAMODE
    stx DMAADDR
    sty DMALEN
    seta8()
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
    setaxy16()
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

}