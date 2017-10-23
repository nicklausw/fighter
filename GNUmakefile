graphics_png = $(wildcard gfx/*.png)
graphics_chr = $(graphics_png:.png=.chr)
graphics_rle = $(graphics_chr:.chr=.rle)

all: $(graphics_rle)
	bass-untech -benchmark -strict -sym fighter_orig.sym -o fighter.sfc fighter.asm
	tools/sym2bsnes.lua fighter_orig.sym fighter.sym; rm -f fighter_orig.sym
	tools/snes-check.py fighter.sfc
	bsnes --enable-debug-interface --break-on-brk fighter.sfc

gfx/%.rle: gfx/%.chr
	tools/rle.py $< $@

gfx/%.chr: gfx/%.png
	tools/snes-tile-tool.py -i $< -o $(subst .png,,$<)

clean:
	rm -f gfx/*.nam gfx/*.pal gfx/*.rle gfx/*.chr