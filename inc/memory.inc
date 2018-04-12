// memory.inc

expression loword(n) = (n & $ffff)

macro seek(offset) {
    origin {offset} & $3fffff
    base {offset}
}

seek($CFFFFF) // 1024K

namespace _mem_ {
    variable wRAM = $0200
    variable wRAMOpen = 0

    variable code = $C00000
    variable codeOpen = 0

    variable data = $C10000
    variable dataOpen = 0

    variable dPage = $00
    variable dPageOpen = 0

    variable bss7e = $7e2000
    variable bss7eOpen = 0
}

macro closeBlock() {
    if _mem_.wRAMOpen {
        _mem_.wRAM = pc()
        if _mem_.wRAM > 0x1fff {
            error "out of wRAM"
        }
        _mem_.wRAMOpen = 0
    } else if _mem_.codeOpen {
        _mem_.code = pc()
        if _mem_.code > $C0ffbf {
            error "out of code ROM"
        }
        _mem_.codeOpen = 0
    } else if _mem_.dataOpen {
        if pc() >= $CFFFFF {
            error "out of data area"
        }
        _mem_.data = pc()
        _mem_.dataOpen = 0
    } else if _mem_.dPageOpen {
        _mem_.dPage = pc()
        if _mem_.dPage > $ff {
            error "out of direct page"
        }
        _mem_.dPageOpen = 0
    } else if _mem_.bss7eOpen {
        _mem_.bss7e = pc()
        if _mem_.bss7e > $7e7fff {
            error "out of bss7e"
        }
        _mem_.bss7eOpen = 0
    }
}

macro wRAM() {
    closeBlock()
    origin 0; base _mem_.wRAM
    _mem_.wRAMOpen = 1
}

macro code() {
    closeBlock()
    seek(_mem_.code)
    _mem_.codeOpen = 1
}

macro data() {
    closeBlock()
    seek(_mem_.data)
    _mem_.dataOpen = 1
}

macro dPage() {
    closeBlock()
    origin 0; base _mem_.dPage
    _mem_.dPageOpen = 1
}

macro bss7e() {
    closeBlock()
    origin 0; base _mem_.bss7e
    _mem_.bss7eOpen = 1
}

macro res(n) {
    base base() + {n}
}

macro databank(n) {
    if _REGS_.aSize != 8 {
        a8()
    }
    lda #<bankbyte({n})
    pha; plb
}

// alias
macro end() {
    closeBlock()
}

inline asset(name, file) {
    data(); insert {name}, "../gfx/{file}"
}

inline romname(name) {
    seek($ffc0)
  romname:
    db {name}
    if pc() - romname > 21 {
        error "rom name too long"
    }
}

inline rominfo() {
    seek($ffd5)
}
inline vectors.native() {
    seek($ffe4)
}
inline vectors.6502() {
    seek($fff4)
}
