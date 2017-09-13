// stuff for fighter

expression loword(n) = (n & $ffff)

macro seek(offset) {
    origin (({offset} & 0x3f0000) >> 1) | ({offset} & 0x7fff)
    base {offset}
}

seek($48000) // 256 kb

namespace space {
    variable wRAM = $0200
    variable wRAMOpen = 0

    variable code = $8000
    variable codeOpen = 0

    variable data = $18000
    variable dataOpen = 0

    variable dPage = $00
    variable dPageOpen = 0

    variable bss7e = $7e2000
    variable bss7eOpen = 0
}

macro closeBlock() {
    if space.wRAMOpen {
        space.wRAM = pc()
        if space.wRAM > 0x1fff {
            error "out of wRAM"
        }
        space.wRAMOpen = 0
    } else if space.codeOpen {
        space.code = pc()
        if space.code > $ffbf {
            error "out of code ROM"
        }
        space.codeOpen = 0
    } else if space.dataOpen {
        if bankbyte(space.data) != bankbyte(pc()) {
            error "data overruns bank"
        }
        if pc() > $48000 {
            error "out of data area"
        }
        space.data = pc()
        space.dataOpen = 0
    } else if space.dPageOpen {
        space.dPage = pc()
        if space.dPage > $ff {
            error "out of direct page"
        }
        space.dPageOpen = 0
    } else if space.bss7eOpen {
        space.bss7e = pc()
        if space.bss7e > $7e7fff {
            error "out of bss7e"
        }
        space.bss7eOpen = 0
    }
}

macro wRAM() {
    closeBlock()
    origin 0; base space.wRAM
    space.wRAMOpen = 1
}

macro code() {
    closeBlock()
    seek(space.code)
    space.codeOpen = 1
}

macro data() {
    closeBlock()
    seek(space.data)
    space.dataOpen = 1
}

macro dPage() {
    closeBlock()
    origin 0; base space.dPage
    space.dPageOpen = 1
}

macro bss7e() {
    closeBlock()
    origin 0; base space.bss7e
    space.bss7eOpen = 1
}

macro res(n) {
    base base() + {n}
}

macro databank(n) {
    seta8()
    lda.b #bankbyte({n})
    pha; plb
}

macro data.upBank() {
    pc() = (pc & $ffff) + $10000 + $8000
}

// alias
macro end() {
    closeBlock()
}