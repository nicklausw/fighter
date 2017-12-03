code()
function init {
    sei // no interrupts
    clc; xce // no emulate
    jmp >+ // for banking and zero

  zero:; db 00

  +;cld // no decimal
    phk; plb // data bank = pc bank

  axy16()

    ldx #$1fff
    txs // stack

    lda #$2100
    tad // direct page at vRAM

  axy8()

    lda #$30; sta $30
    stz $31
    stz $32
    stz $33

    lda #$80; sta $00
    ldx #$00
  -;stz $01,x
    inx
    cpx #$0d; bne -
    ldx #$00
  -;stz $0d,x
    stz $0d,x
    inx
    cpx #$15; bne -

    lda #$80; sta $15
    stz $16
    stz $17
    stz $1a
    stz $1b

    lda #$01; sta $1b

    stz $1c; stz $1c
    stz $1d; stz $1d
    stz $1e
    sta $1e
    stz $1f; stz $1f
    stz $20; stz $20
    stz $21

    ldx #$23
  -;stz $00,x
    inx
    cpx #$30; bne -

  a16()
    lda #$4200
    tcd
  a8()

    stz $00
    lda #$ff; sta $01
    
    ldx #$02
  -;stz $00,x
    inx
    cpx #$0e; bne -

  a16()
    lda #$0000
    tcd // direct page = "zeropage"
  a8()
    
  xy16()

    // clear the vram via dma
    lda #$80
    sta >PPUCTRL // access vram in words

    ldx #$1809
    stx >DMAMODE // fixed source dma

    ldx #$0000
    stx >PPUADDR // vram port address at $0000

    ldx #>zero
    stx >DMAADDR // source address at $xx:0000

    lda #<bankbyte(zero)
    sta >DMAADDRBANK // source bank, probably $80

    ldx #$0000
    stx >DMALEN // transfer $ffff+1=65536 bytes

    lda #$01
    sta >COPYSTART // initiate vram clearing


    // clear the palette
    stz >CGADDR
    ldx #$0100
  paletteLoop:
    stz >CGDATA
    stz >CGDATA
    dex
    bne paletteLoop


    // clear the sprites
    stz >OAMADDR
    ldx #$0080
    lda #<240

  spriteLoop:
    sta >OAMDATA // x = 240
    sta >OAMDATA // y = 240
    stz >OAMDATA // tile = 0
    stz >OAMDATA // priority = 0, no flips
    dex
    bne spriteLoop

    ldx #$0020

  spriteLoop2:
    stz >OAMDATA // size bit = 0
    dex
    bne spriteLoop2


    // clear RAM
    stz >WMADDL // wram address to $000000
    stz >WMADDM
    stz >WMADDH

    ldx #$8008
    stx >DMAMODE // fixed source dma

    ldx #>zero
    stx >DMAADDR // source address

    lda #<bankbyte(zero)
    sta >DMAADDRBANK // source bank

    ldx #$0000
    stx >DMALEN // transfer $ffff+1=65536 bytes

    lda #$01
    sta >COPYSTART // start transfer

    lda #$01
    sta >COPYSTART // again to get all 128 kb


    // the last of basic set-up.
    lda #$8f // blank screen
    sta >PPUBRIGHT

    lda #$01
    sta >BGMODE  // mode 0
    stz >BGCHRADDR // bg planes at $0000
    stz >OBSEL // sprite chr at $0000

    lda #$60
    sta >NTADDR+0 // plane 0 nametable at $6000
    sta >NTADDR+1 // plane 1 nametable also at $6000

    lda #$ff
    sta >BGSCROLLY+0  // the ppu displays lines 1-224, so set scroll to
    sta >BGSCROLLY+0  // $ff so that the first displayed line is line 0

    jmp >Main
}