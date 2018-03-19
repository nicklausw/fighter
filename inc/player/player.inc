namespace player {

constant x = oam.table+0
constant y = oam.table+1

inline new(name) {
  include "new.inc"
}

inline _modify(n, var, dir) {
    lda >{var}
    if {dir} == 0 {
        sec; sbc #<{n}
    } else {
        clc; adc #<{n}
    }
    sta >{var}
}

// we have to specify namespace with locals like "player.x"
// because inlines aren't always called from the player namespace.
inline left(n) {
  player._modify({n}, player.x, 0)
}
inline right(n) {
  player._modify({n}, player.x, 1)
}
inline up(n) {
  player._modify({n}, player.y, 0)
}
inline down(n) {
  player._modify({n}, player.y, 1)
}

}
