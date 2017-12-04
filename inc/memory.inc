// memory.inc

expression loword(n) = (n & $ffff)

macro seek(offset) {
    origin (({offset} & 0x3f0000) >> 1) | ({offset} & 0x7fff)
    base {offset}
}

seek($48000) // 128 kb

namespace _mem_ {
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
    if _mem_.wRAMOpen {
        _mem_.wRAM = pc()
        if _mem_.wRAM > 0x1fff {
            error "out of wRAM"
        }
        _mem_.wRAMOpen = 0
    } else if _mem_.codeOpen {
        _mem_.code = pc()
        if _mem_.code > $ffbf {
            error "out of code ROM"
        }
        _mem_.codeOpen = 0
    } else if _mem_.dataOpen {
        if pc() > $48000 {
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

// gets temporary info about an inserted file.
inline _tempInsert(file) {
    evaluate _pc = pc()
    insert {file}
    evaluate _size = pc() - {_pc}
}

// inserts a file, checks if it overruns a bank,
// and inserts it again at the right place if so.
// can cause junk data at end of bank. kinda wasteful.
inline dataInsert(name, file) {
    _tempInsert({file})
    if {_size} > $8000 {
        error "file is larger than a bank in size"
    }
    if pc() & $ffff < $8000 {
        seek((pc() & $ff0000) + $8000)
        _tempInsert({file})
    }
    enqueue pc
    seek({_pc})
    {name}:; namespace {name} {
        constant size = {_size}
    }
    dequeue pc
}

inline asset(name, file) {
    data(); dataInsert({name}, "../gfx/{file}")
}