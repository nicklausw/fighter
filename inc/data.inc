inline palette(name, file) {
    data()
    function {name} {
        insert "../gfx/{file}.pal"
        constant size = pc() - {name}
    }
}

inline tileset(name, file) {
    data()
    {name}:; insert "../gfx/{file}.rle"
}

namespace palettes {
  palette(main, sektor)
}

namespace tilesets {
  tileset(sektor, sektor)
}