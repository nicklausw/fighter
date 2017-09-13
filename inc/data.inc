data()

// data
message:; db "This...is bass.", $FF

inline palette(name, file) {
    function {name} {
        insert {file}
        constant size = pc() - {name}
    }
}

namespace palettes {
  palette(main, "gfx.pal")
}


font:
  insert "gfx.rle"
