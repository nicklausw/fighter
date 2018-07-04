graphics_png = $(wildcard gfx/*.png)
graphics_chr = $(graphics_png:.png=.chr)
graphics_rle = $(graphics_chr:.chr=.rle)

all: $(graphics_rle)
	bass-ultima -require-modifier -benchmark -strict -sym fighter.old.sym -o fighter.sfc fighter.asm
	tools/sym.py fighter.old.sym fighter.sym; rm -f fighter.old.sym
	tools/snes-check.py fighter.sfc
	bsnes --enable-debug-interface --break-on-brk fighter.sfc

gfx/%.rle: gfx/%.chr
	tools/rle.py $< $@

gfx/%.chr: gfx/%.png
	tools/snes-tile-tool.py -i $< -o $(<:.png=)

clean:
	rm -f gfx/*.nam gfx/*.pal gfx/*.rle gfx/*.chr
