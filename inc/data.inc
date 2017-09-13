data()

// data
message:; db "This...is bass.", $FF

inline palette(name, file) {
    function {name} {
        insert "../gfx/{file}.pal"
        constant size = pc() - {name}
    }
}

namespace palettes {
  palette(main, sektor)
}


font:
  insert "../gfx/sektor.rle"
