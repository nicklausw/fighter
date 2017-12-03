Fighter!
========
It's currently a useless demo, but soon I
hope for this to become a playable SNES
fighting game.

[Uses my fork of bass-untech.](https://github.com/nicklausw/bass-untech)

Credit goes to UnDisbeliever for some of
the framework going on inside the code
(bass magic and all), and to tepples for
pretty much all of the code in some
sense or another.

If you're wondering what the deal is with
fighter.sym, that's designed to work with
[BenjaminSchulte's fork of bsnes-plus.](https://github.com/BenjaminSchulte/bsnes-plus)
The SNES.BLABLABLA definitions are added by
the emulator itself, and it also orders the
labels numerically. It does all this after
sym2bsnes.lua plays with bass-untech's output.
It's like those labels went through an entire factory.