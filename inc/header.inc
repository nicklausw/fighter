namespace header {

romname("FIGHTER GAME")

rominfo(); {
    db $30 // lorom fastrom
    db $00 // no battery ram
    db $08 // 256K rom
}

vectors.native(); {
    dw rtiPointer, rtiPointer, rtiPointer
    dw ppu.vBlank, $ffff, rtiPointer
}

vectors.6502(); {
    dw rtiPointer, $ffff, rtiPointer
    dw rtiPointer, init, rtiPointer
}

}