seek($ffc0)
namespace header {
  romname:
    db "FIGHTER GAME" // rom name
    fill 21 - (pc() - romname)

    db $30 // lorom fastrom
    db $00 // no battery ram
    db $08 // 256K rom

    fill 12

    dw rtiPointer, rtiPointer, rtiPointer
    dw ppu.vBlank, $FFFF, rtiPointer

    fill 4  // more unused vectors

    // a bunch of 6502 vectors
    dw rtiPointer, $FFFF, rtiPointer
    dw rtiPointer, init, rtiPointer
}