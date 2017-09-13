//
// S-CPU and S-PPU MMIO port definitions for Super NES
// and useful 65816 macros
//
// Copyright 2014-2015 Damian Yerrick
//
// Copying and distribution of this file, with or without
// modification, are permitted in any medium without royalty provided
// the copyright notice and this notice are preserved in all source
// code copies.  This file is offered as-is, without any warranty.
//

//
// This version of this file is written for bass ~v15 syntax.
// Everything should work perfectly; nothing commented out.
//

//
// This header summarizes some of the Super NES MMIO ports.
// For more details, see these web pages:
// http://wiki.superfamicom.org/
// http://problemkaputt.de/fullsnes.htm
//
// Names of MMIO ports in this header file may differ from purported
// official names for two reasons: to avoid the appearance of
// misappropriation, and because sometimes these make more sense.
//

if !{defined SNES_H} {
  constant SNES_H = 1
  architecture wdc65816

// S-PPU configuration ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

constant PPUBRIGHT = $2100
// 76543210
// |   ++++- brightness (F: max)
// +-------- 1: disable rendering
constant FORCEBLANK = $80

constant PPURES = $2133
// 76543210
// ||  |||+- Screen interlace
// ||  ||+-- Shrink sprites vertically during interlace
// ||  |+--- 0: show lines 1-224; 1: show lines 1-239
// ||  +---- Show subscreen in left half of each pixel
// ||        (modes 012347; forced on in modes 56)
// |+------- In mode 7, use bit 7 as priority
// +-------- External genlock, intended for SFC Titler. Use 0 on SNES.
constant INTERLACE    = $01
constant INTERLACEOBJ = $02
constant BG_TALL      = $04
constant SUB_HIRES    = $08
constant M7_EXTBG     = $40

constant PPUSTATUS1 = $213E
// 76543210  PPU address generator status
// ||  ++++- PPU1 version (always 1)
// |+------- 1: sprite overflow (>32 on a line) since the last vblank end
// +-------- 1: sliver overflow (>34 on a line) since the last vblank end
// this parallels bit 5 of $2002 on NES

constant PPUSTATUS2 = $213F
// 76543210  PPU compositor status
// || |++++- PPU2 version (1-3, not counting minor versions of 3)
// || +----- 1: PAL
// |+------- 1: GETXY has happened since last PPUSTATUS2 read
// +-------- Toggles every vblank

// S-PPU sprites ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

constant OBSEL = $2101
// 76543210
// ||||| ++- Sprite main pattern table (0=$0000, 1=$2000, 2=$4000, 3=$6000)
// |||++---- Alt pattern table offset (0=$1000, 1=$2000, 2=$3000, 3=$4000)
// +++------ 0: 8/16; 1: 8/32; 2: 8/64; 3: 16/64; 4: 32/64; 5: 64/64
//           (all sprites are square and 2D-mapped)
constant OBSIZE_8_16  = $00
constant OBSIZE_8_32  = $20
constant OBSIZE_8_64  = $40
constant OBSIZE_16_32 = $60
constant OBSIZE_16_64 = $80
constant OBSIZE_32_64 = $A0

constant OAMADDR = $2102  // 16-bit, 128 sprites followed by high-X/size table
constant OAMDATA = $2104
constant OAMDATARD = $2138
// Parallels NES $2003, except apparently word-addressed.
// OAM random access is working here, unlike on NES.
// If bit 15 is set, value at start of frame apparently also
// controls which sprites are in front

// S-PPU background configuration ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

constant BGMODE = $2105
// 76543210
// |||||+++- 0: 4 planes 2 bpp
// |||||     1: 2 planes 4 bpp, 1 plane 2 bpp
// |||||     2: 2 planes 4 bpp, OPT
// |||||     3: 1 plane 8 bpp, 1 plane 4 bpp
// |||||     4: 1 plane 8 bpp, 1 plane 2 bpp, OPT
// |||||     5: 1 plane 4 bpp, 1 plane 2 bpp, hires
// |||||     6: 1 plane 4 bpp, OPT, hires
// |||||     7: 1 plane rot/scale
// ||||+---- In mode 1, set plane 2 high-prio in front of all others
// |||+----- Plane 0 tile size (0: 8x8, 1: 16x16)
// ||+------ Plane 1 tile size (0: 8x8, 1: 16x16)
// |+------- Plane 2 tile size (0: 8x8, 1: 16x16)
// +-------- Plane 3 tile size (0: 8x8, 1: 16x16)
//           Modes 5 and 6 use 16x8 instead of 8x8
//           Mode 7 always uses 8x8

constant MOSAIC = $2106
// 76543210
// |||||||+- Apply mosaic to plane 0 (or mode 7 high-prio horizontal)
// ||||||+-- Apply mosaic to plane 1 (or mode 7 high-prio vertical)
// |||||+--- Apply mosaic to plane 2
// ||||+---- Apply mosaic to plane 3
// ++++----- Pixel size minus 1 (0=1x1, 15=16x16)

constant NTADDR = $2107  // through $210A
// 76543210
//  ||||||+- Nametable width (0: 1 screen, 1: 2 screens)
//  |||||+-- Nametable height (0: 1 screen, 1: 2 screens)
//  +++++--- Nametable base address in $400 units
// Each nametable in modes 0-6 is 32 rows, each 32 spaces long.
expression NTXY(xc,yc) = ((xc)|((yc)<<5))

constant BGCHRADDR = $210B
// FEDCBA98 76543210
//  ||| |||  ||| +++- Pattern table base address for plane 0
//  ||| |||  +++----- Same for plane 1
//  ||| +++---------- Same for plane 2
//  +++-------------- Same for plane 3

constant M7SEL = $211A
// 76543210
// ||    ||
// ||    |+- Flip screen horizontally
// ||    +-- Flip screen vertically
// ++------- 0: repeat entire mode 7 plane
//           2: transparent outside; 3: tile $00 repeating outside
constant M7_HFLIP    = $01
constant M7_VFLIP    = $02
constant M7_WRAP     = $00
constant M7_NOWRAP   = $80
constant M7_BORDER00 = $C0

// S-PPU scrolling ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

constant BGSCROLLX = $210D  // double write low then high (000-3FF m0-6, 000-7FF m7)
constant BGSCROLLY = $210E  // similar. reg 210F-2114 are same for other planes
// Hi-res scrolling in modes 5-6 moves by whole (sub+main) pixels in X
// but half scanlines in Y.
// The top visible line is the line below the value written here.
// For example, in 224-line mode, if 12 is written, lines 13 through
// 237 of the background are visible.  This differs from the NES.
//
// Mode 7 uses this value as the center of rotation.  This differs
// from the GBA, which fixes the center of rotation at the top left.

// 211B-2120 control mode 7 matrix; to be documented later

// S-PPU VRAM data port ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

constant PPUCTRL = $2115
// 76543210
// |   ||++- VRAM address increment (1, 32, 128, 128)
// |   ++--- Rotate low bits of address left by 3 (off, 8, 9, or 10)
// +-------- 0: Increment after low data port access; 1: after high
// Corresponds to bit 2 of $2000 on NES
constant VRAM_DOWN   = $01
constant VRAM_M7DOWN = $02
constant INC_DATAHI  = $80

constant PPUADDR = $2116  // Word address, not double-write anymore
constant PPUDATA = $2118
constant PPUDATAHI = $2119
constant PPUDATARD = $2139  // Same dummy read as on NES is needed
constant PPUDATARDHI = $213A

// S-PPU palette ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

constant CGADDR = $2121
constant CGDATA = $2122  // 5-bit BGR, write twice, low byte first
constant CGDATARD = $213B  // 5-bit BGR, read twice, low byte first
expression RGB(r,g,b) = ((r)|((g)<<5)|((b)<<10))

// S-PPU window ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

constant BG12WINDOW = $2123
constant BG34WINDOW = $2124
constant OBJWINDOW = $2125
// 76543210
// ||||||++- 0: disable window 1 on BG1/BG3/OBJ; 2: enable; 3: enable outside
// ||||++--- 0: disable window 2 on BG1/BG3/OBJ; 2: enable; 3: enable outside
// ||++----- 0: disable window 1 on BG2/BG4; 2: enable; 3: enable outside
// ++------- 0: disable window 2 on BG2/BG4; 2: enable; 3: enable outside

constant WINDOW1L = $2126
constant WINDOW1R = $2127
constant WINDOW2L = $2128
constant WINDOW2R = $2129

constant BGWINDOP = $212A   // Window op is how windows are combined when both
constant OBJWINDOP = $212B  // windows 1 and 2 are enabled.
// 76543210
// ||||||++- Window op for plane 0 or sprites (0: or, 1: and, 2: xor, 3: xnor)
// ||||++--- Window op for plane 1 or color window
// ||++----- Window op for plane 2
// ++------- Window op for plane 3

// S-PPU blending (or "color math") ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// The main layer enable reg, corresponding to PPUMASK on the NES,
// is BLENDMAIN.
constant BLENDMAIN  = $212C  // Layers enabled for main input of blending
constant BLENDSUB   = $212D  // Layers enabled for sub input of blending
constant WINDOWMAIN = $212E  // Windows enabled for main input of blending
constant WINDOWSUB  = $212F  // Windows enabled for sub input of blending
// 76543210
//    ||||+- plane 0
//    |||+-- plane 1
//    ||+--- plane 2
//    |+---- plane 3
//    +----- sprites
// BLENDMAIN roughly parallels NES $2001 bits 4-3,
// except that turning off both bits doesn't disable rendering.
// (Use PPUBRIGHT for that.)

// PPU1 appears to generate a stream of (main, sub) pairs, which
// PPU2 combines to form output colors.

// Blending parameters not documented yet.  Wait for a future demo.

// When BGMODE is 0-6 (or during vblank in mode 7), a fast 16x8
// signed multiply is available, finishing by the next CPU cycle.
constant M7MCAND = $211B    // write low then high
constant M7MUL = $211C      // 8-bit factor
constant M7PRODLO = $2134
constant M7PRODHI = $2135
constant M7PRODBANK = $2136

constant GETXY = $2137  // read while $4201 D7 is set: populate x and y coords
constant XCOORD = $213C  // used with light guns, read twice
constant YCOORD = $213D  // also read twice

// SPC700 communication ports ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

constant APU0 = $2140
constant APU1 = $2141
constant APU2 = $2142
constant APU3 = $2143

// S-CPU interrupt control ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

constant PPUNMI = $4200
// 76543210
// | ||   +- Automatically read controllers in first 4 lines of vblank
// | ++----- 0: No IRQ; 1: IRQs at HTIME;
// |         2: one IRQ at (0, VTIME); 3: one IRQ at (HTIME, VTIME)
// +-------- 1: Enable NMI at start of vblank
constant VBLANK_NMI = $80
constant HTIME_IRQ  = $10
constant VTIME_IRQ  = $20
constant HVTIME_IRQ = $30
constant AUTOREAD   = $01

constant HTIME   = $4207
constant HTIMEHI = $4208
constant VTIME   = $4209
constant VTIMEHI = $420A

constant NMISTATUS = $4210
// 76543210
// |   ||||
// |   ++++- DMA controller version (1, 2) where v1 has an HDMA glitch
// +-------- 1: Vblank has started since last read (like $2002.d7 on NES)

constant TIMESTATUS = $4211  // Acknowledge htime/vtime IRQ
constant VBLSTATUS = $4212
// 76543210
// ||     +- 0: Controller reading finished; 1: busy
// |+------- In hblank
// +-------- In vblank

constant ROMSPEED = $420D  // 0: slow ROM everywhere; 1: fast ROM in banks 80-FF
                  // (requires 120ns or faster PRG ROM)

// S-CPU controller I/O ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// Manual controller reading behaves almost exactly as on Famicom.
// For games using up to 2 standard controllers, these aren't needed,
// as you can enable controller autoreading along with vblank NMIs.
// But for games using (multitap, mouse, etc.), you will need to
// read the extra bits separately after the autoreader finishes.
constant JOY0 = $4016
constant JOY1 = $4017

// In addition to the common strobe, each controller port has an
// additional output bit that can be used as, say, a chip select
// for SPI peripherals.
constant JOYOUT = $4201
// 76543210
// |+------- Controller 1 pin 6 output
// +-------- Controller 2 pin 6 output

// Results of the autoreader
constant JOY1CUR = $4218    // Bit 0: used by standard controllers
constant JOY2CUR = $421A
constant JOY1B1CUR = $421C  // Bit 1: used by multitap and a few oddball
constant JOY2B1CUR = $421E  // input devices
// FEDCBA98 76543210
// BYSRUDLR AXLRTTTT
// |||||||| ||||++++- controller type (0: controller, 1: mouse)
// |||||||| ||++----- shoulder buttons
// ++-------++------- right face buttons
//   ||++++---------- Control Pad
//   ++-------------- center face buttons
constant KEY_B      = $8000
constant KEY_Y      = $4000
constant KEY_SELECT = $2000
constant KEY_START  = $1000
constant KEY_UP     = $0800
constant KEY_DOWN   = $0400
constant KEY_LEFT   = $0200
constant KEY_RIGHT  = $0100
constant KEY_A      = $0080
constant KEY_X      = $0040
constant KEY_L      = $0020
constant KEY_R      = $0010

// S-CPU multiply and divide ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// Multiply unit.  Also good for shifting pixels when drawing
// text in a proportional font.
constant CPUMCAND = $4202  // unchanged by multiplications
constant CPUMUL = $4203    // write here to fill CPUPROD 8 cycles later
constant CPUPROD = $4216
constant CPUPRODHI = $4217

// Divide unit
constant CPUNUM = $4204
constant CPUNUMHI = $4205
constant CPUDEN = $4206    // write divisor to fill CPUQUOT/CPUREM 16 cycles later
constant CPUQUOT = $4214
constant CPUQUOTHI = $4215
constant CPUREM = CPUPROD
constant CPUREMHI = CPUPRODHI

// S-CPU DMA ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

constant COPYSTART = $420B  // writes of 1 << n start a DMA copy on channel n
constant HDMASTART = $420C  // writes of 1 << n start HDMA on channel n
// Don't run a DMA copy while HDMA is enabled, or you might run into
// a defect in revision 1 of the S-CPU that causes crashing.

// There are 8 DMA channels.
// Registers for channels 1-7 start at $4310, $4320, ...
constant DMAMODE = $4300
// 76543210
// || ||+++- PPU address offset pattern
// || ||     0: 0     1: 01    2: 00    3: 0011  4: 0123  5: 0101
// || ++---- Memcpy only: 0: increment; 1: fixed; 2: decrement
// |+------- HDMA only: 1: Table contains pointers
// +-------- Direction (0: read CPU write PPU; 1: read PPU write CPU)
constant DMA_LINEAR   = $00
constant DMA_01       = $01
constant DMA_00       = $02  // For HDMA to double write ports; copies can use linear
constant DMA_0011     = $03  // For HDMA to scroll positions and mode 7 matrices
constant DMA_0123     = $04  // For HDMA to window registers
constant DMA_0101     = $05  // Not sure how this would be useful for HDMA
constant DMA_FORWARD  = $00
constant DMA_CONST    = $08
constant DMA_BACKWARD = $10
constant DMA_INDIRECT = $40
constant DMA_READPPU  = $80

constant DMAPPUREG = $4301
constant DMAADDR = $4302
constant DMAADDRHI = $4303
constant DMAADDRBANK = $4304
constant DMALEN = $4305  // number of bytes, not number of transfers; 0 means 65536
constant DMALENHI = $4306

// A future demo that includes HDMA effects would include port definitions.
//constant HDMAINDBANK = $4307
//constant HDMATABLELO = $4308
//constant HDMATABLEHI = $4309
//constant HDMALINE = $430A

expression lobyte(n) = (n & $FF)

// composite values for use with 16-bit writes to DMAMODE
constant DMAMODE_PPULOFILL = (lobyte(PPUDATA) << 8)       | DMA_LINEAR | DMA_CONST
constant DMAMODE_PPUHIFILL = (lobyte(PPUDATA + 1) << 8) | DMA_LINEAR | DMA_CONST
constant DMAMODE_PPUFILL   = (lobyte(PPUDATA) << 8)       | DMA_01     | DMA_CONST
constant DMAMODE_PPULODATA = (lobyte(PPUDATA) << 8)       | DMA_LINEAR | DMA_FORWARD
constant DMAMODE_PPUHIDATA = (lobyte(PPUDATA + 1) << 8) | DMA_LINEAR | DMA_FORWARD
constant DMAMODE_PPUDATA   = (lobyte(PPUDATA) << 8)       | DMA_01     | DMA_FORWARD
constant DMAMODE_CGDATA    = (lobyte(CGDATA) << 8)        | DMA_00     | DMA_FORWARD
constant DMAMODE_OAMDATA   = (lobyte(OAMDATA) << 8)       | DMA_00     | DMA_FORWARD

// FULLSNES NAMES ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// These aliases match the names presented in Fullsnes.

// S-PPU write
constant INIDISP = PPUBRIGHT
constant OAMADDL = OAMADDR+0
constant OAMADDH = OAMADDR+1
constant BG1SC   = NTADDR+0
constant BG2SC   = NTADDR+1
constant BG3SC   = NTADDR+2
constant BG4SC   = NTADDR+3
constant BG12NBA = BGCHRADDR+0  // Welcome to...
constant BG34NBA = BGCHRADDR+1  // BG34NBA JAM!!!
constant BG1HOFS = BGSCROLLX+0
constant BG1VOFS = BGSCROLLY+0
constant BG2HOFS = BGSCROLLX+2
constant BG2VOFS = BGSCROLLY+2
constant BG3HOFS = BGSCROLLX+4
constant BG3VOFS = BGSCROLLY+4
constant BG4HOFS = BGSCROLLX+6
constant BG4VOFS = BGSCROLLY+6
constant VMAIN   = PPUCTRL
constant VMADDL  = PPUADDR+0
constant VMADDH  = PPUADDR+1
constant VMDATAL = PPUDATA+0
constant VMDATAH = PPUDATA+1
constant M7A     = $211B
constant M7B     = $211C
constant M7C     = $211D
constant M7D     = $211E
constant M7X     = $211F
constant M7Y     = $2120
constant CGADD   = CGADDR
constant W12SEL  = BG12WINDOW
constant W34SEL  = BG34WINDOW
constant WOBJSEL = OBJWINDOW
constant WH0     = WINDOW1L
constant WH1     = WINDOW1R
constant WH2     = WINDOW2L
constant WH3     = WINDOW2R
constant WBGLOG  = BGWINDOP
constant WOBJLOG = OBJWINDOP
constant TM      = BLENDMAIN
constant TS      = BLENDSUB
constant TMW     = WINDOWMAIN
constant TSW     = WINDOWSUB
constant CGWSEL  = $2130
constant CGADSUB = $2131
constant COLDATA = $2132
constant SETINI  = PPURES

// S-PPU read
constant MPYL    = M7PRODLO
constant MPYM    = M7PRODHI
constant MPYH    = M7PRODBANK
constant SLHV    = GETXY
constant RDOAM   = OAMDATARD
constant RDVRAML = PPUDATARD
constant RDVRAMH = PPUDATARDHI
constant RDCGRAM = CGDATARD
constant OPHCT   = XCOORD
constant OPVCT   = YCOORD
constant STAT77  = PPUSTATUS1
constant STAT78  = PPUSTATUS2

// other B bus devices
constant APUIO0 = APU0  // didn't want to use capital O with digit 0
constant APUIO1 = APU1
constant APUIO2 = APU2
constant APUIO3 = APU3
constant WMDATA = $2180
constant WMADDL = $2181
constant WMADDM = $2182
constant WMADDH = $2183

// S-CPU I/O write
constant NMITIMEN = $4200
constant WRIO     = $4201
constant WRMPYA   = CPUMCAND
constant WRMPYB   = CPUMUL
constant WRDIVL   = CPUNUM
constant WRDIVH   = CPUNUMHI
constant WRDIVB   = CPUDEN
constant HTIMEL   = HTIME
constant HTIMEH   = HTIMEHI
constant VTIMEL   = VTIME
constant VTIMEH   = VTIMEHI
constant MDMAEN   = COPYSTART  // where I come from MDMA is a drug
constant HDMAEN   = HDMASTART
constant MEMSEL   = ROMSPEED

// S-CPU I/O read
constant RDNMI  = NMISTATUS
constant TIMEUP = TIMESTATUS
constant HVBJOY = VBLSTATUS
constant RDIO   = $4213
constant RDDIVL = CPUQUOT
constant RDDIVH = CPUQUOT+1
constant RDMPYL = CPUPROD
constant RDMPYH = CPUPROD+1
constant JOY1L  = JOY1CUR+0
constant JOY1H  = JOY1CUR+1
constant JOY2L  = JOY2CUR+0
constant JOY2H  = JOY2CUR+1
constant JOY3L  = JOY1B1CUR+0
constant JOY3H  = JOY1B1CUR+1
constant JOY4L  = JOY2B1CUR+0
constant JOY4H  = JOY2B1CUR+1

// MACRO PACK ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// Macros to change the accumulator and index width.
// For best results, use .smart which lets the assembler follow
// SEP/REP and generate appropriately wide immediate values.

namespace _REGS_ {
  variable aSize = 0
  variable iSize = 0
}

//;
// Sets the X and Y registers to use 8-bit values.
// The upper bits are treated as cleared.
macro xy8() {
  _REGS_.iSize = 8
  sep #$10
}

//;
// Sets the X and Y registers to use 16-bit values.
macro xy16() {
  _REGS_.iSize = 16
  rep #$10
}

//;
// Sets the accumulator (A) and memory-only data manipulation
// instructions (such as STZ and ROR) to use 8-bit data.
// The upper 8 bits of A can be swapped in and out with the
// XBA instruction.
macro a8() {
  _REGS_.aSize = 8
  sep #$20
}

//;
// Sets the accumulator and memory-only data manipulation
// instructions to use 16-bit data.
macro a16() {
  _REGS_.aSize = 16
  rep #$20
}

//;
// Equivalent to seta8 and setxy8.
macro axy8() {
  _REGS_.aSize = 8
  _REGS_.iSize = 8
  sep #$30
}

//;
// Equivalent to seta16 and setxy16.
macro axy16() {
  _REGS_.aSize = 16
  _REGS_.iSize = 16
  rep #$30
}

// Macros to push constant values on the stack to be pulled off
// by instructions such as PLB (pull data segment)

//;
// Pushes two constant bytes in the order second, first
// to be pulled in the order first, second.
macro ph2b(first, second) {
  pea ({first} & $FF) | (({second} & $FF) << 8)
}

//;
// Pushes the bank byte of two addresses such that
// PLB will pull them in the order first, second.
// One common pattern is
// ph2banks some_addr, *
// plb
// (stuff using some_addr)
// plb
expression bankbyte(n) = (n) >> 16
macro ph2banks(first, second) {
  ph2b(bankbyte({first}), bankbyte({second}))
}

}
