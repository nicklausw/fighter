graphics_bmp = $(wildcard gfx/*.bmp)
graphics_chr = $(graphics_bmp:.bmp=.chr)
graphics_rle = $(graphics_chr:.chr=.rle)

all: $(graphics_rle)
	bass-untech -strict -sym fighter.sym -o fighter.sfc fighter.asm
	tools/snes-check.py fighter.sfc
	bsnes fighter.sfc

gfx/%.rle: gfx/%.chr
	tools/rle.py $< $@

gfx/%.chr: gfx/%.bmp
	tools/snes-tile-tool.py -i $< -o $(subst .bmp,,$<)