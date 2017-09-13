data()

// data
message:; db "This...is bass.", $FF

inline palette(name, file) {
    {name}:
    insert {file}
    {name}_size:
}

namespace palettes {
  palette(main, "gfx.pal")
}

define size(name) = ({name}_size - {name})



font:
  insert "gfx.rle"
