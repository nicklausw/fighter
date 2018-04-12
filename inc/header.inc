namespace header {

romname("FIGHTER GAME")

rominfo(); {
    db $21 // lorom fastrom
    db $00 // no battery ram
    db $0a // 1024K rom
}

vectors.native(); {
    dw rtiPointer, rtiPointer, rtiPointer
    dw vBlank, $ffff, rtiPointer
}

vectors.6502(); {
    dw rtiPointer, $ffff, rtiPointer
    dw rtiPointer, init, rtiPointer
}

}
