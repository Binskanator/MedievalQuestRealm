#ifndef GBA_H
#define GBA_H

// GBA hardware registers and type definitions

// Base addresses for GBA hardware registers
#define REG_BASE 0x04000000
#define VRAM_BASE 0x06000000
#define PALETTE_BASE 0x05000000
#define OAM_BASE 0x07000000

// Display Control
#define REG_DISPCNT *((volatile unsigned short *) (REG_BASE + 0x0000))
#define MODE_0 0x0000
#define MODE_3 0x0003
#define MODE_4 0x0004
#define BG0_ENABLE 0x0100
#define BG1_ENABLE 0x0200
#define BG2_ENABLE 0x0400
#define BG3_ENABLE 0x0800
#define OBJ_ENABLE 0x1000
#define OBJ_MAP_1D 0x0040

// Display Status
#define REG_DISPSTAT *((volatile unsigned short *) (REG_BASE + 0x0004))
#define VBLANK_FLAG 0x0001
#define HBLANK_FLAG 0x0002
#define VCOUNT_FLAG 0x0004
#define VBLANK_IRQ 0x0008
#define HBLANK_IRQ 0x0010
#define VCOUNT_IRQ 0x0020

// Background Control
#define REG_BG0CNT *((volatile unsigned short *) (REG_BASE + 0x0008))
#define REG_BG1CNT *((volatile unsigned short *) (REG_BASE + 0x000A))
#define REG_BG2CNT *((volatile unsigned short *) (REG_BASE + 0x000C))
#define REG_BG3CNT *((volatile unsigned short *) (REG_BASE + 0x000E))

// Background offsets
#define REG_BG0HOFS *((volatile unsigned short *) (REG_BASE + 0x0010))
#define REG_BG0VOFS *((volatile unsigned short *) (REG_BASE + 0x0012))
#define REG_BG1HOFS *((volatile unsigned short *) (REG_BASE + 0x0014))
#define REG_BG1VOFS *((volatile unsigned short *) (REG_BASE + 0x0016))
#define REG_BG2HOFS *((volatile unsigned short *) (REG_BASE + 0x0018))
#define REG_BG2VOFS *((volatile unsigned short *) (REG_BASE + 0x001A))
#define REG_BG3HOFS *((volatile unsigned short *) (REG_BASE + 0x001C))
#define REG_BG3VOFS *((volatile unsigned short *) (REG_BASE + 0x001E))

// For tiles or font
#define TILE_BASE_ADDR(n) (VRAM_BASE + (n) * 0x4000)
// For map (which tiles go where on the screen)
#define MAP_BASE_ADDR(n) (VRAM_BASE + (n) * 0x800)

// Input Registers
#define REG_KEYINPUT *((volatile unsigned short *) (REG_BASE + 0x0130))
#define KEY_A 0x0001
#define KEY_B 0x0002
#define KEY_SELECT 0x0004
#define KEY_START 0x0008
#define KEY_RIGHT 0x0010
#define KEY_LEFT 0x0020
#define KEY_UP 0x0040
#define KEY_DOWN 0x0080
#define KEY_R 0x0100
#define KEY_L 0x0200
#define KEY_MASK 0x03FF

// Interrupt Register
#define REG_IE *((volatile unsigned short *) (REG_BASE + 0x0200))
#define REG_IF *((volatile unsigned short *) (REG_BASE + 0x0202))
#define REG_IME *((volatile unsigned short *) (REG_BASE + 0x0208))

// DMA Registers (used for transferring data quickly)
#define REG_DMA0SAD *((volatile unsigned int *) (REG_BASE + 0x00B0))
#define REG_DMA0DAD *((volatile unsigned int *) (REG_BASE + 0x00B4))
#define REG_DMA0CNT *((volatile unsigned int *) (REG_BASE + 0x00B8))
#define REG_DMA1SAD *((volatile unsigned int *) (REG_BASE + 0x00BC))
#define REG_DMA1DAD *((volatile unsigned int *) (REG_BASE + 0x00C0))
#define REG_DMA1CNT *((volatile unsigned int *) (REG_BASE + 0x00C4))
#define REG_DMA2SAD *((volatile unsigned int *) (REG_BASE + 0x00C8))
#define REG_DMA2DAD *((volatile unsigned int *) (REG_BASE + 0x00CC))
#define REG_DMA2CNT *((volatile unsigned int *) (REG_BASE + 0x00D0))
#define REG_DMA3SAD *((volatile unsigned int *) (REG_BASE + 0x00D4))
#define REG_DMA3DAD *((volatile unsigned int *) (REG_BASE + 0x00D8))
#define REG_DMA3CNT *((volatile unsigned int *) (REG_BASE + 0x00DC))

// Screen dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

// Colors
typedef unsigned short Color;
#define RGB(r, g, b) ((r) | ((g) << 5) | ((b) << 10))
#define RGB5(r, g, b) (((b) << 10) | ((g) << 5) | (r))

// 16-bit color palette definitions
#define BLACK RGB5(0, 0, 0)
#define WHITE RGB5(31, 31, 31)
#define RED RGB5(31, 0, 0)
#define GREEN RGB5(0, 31, 0)
#define BLUE RGB5(0, 0, 31)
#define YELLOW RGB5(31, 31, 0)
#define MAGENTA RGB5(31, 0, 31)
#define CYAN RGB5(0, 31, 31)
#define GRAY RGB5(15, 15, 15)

// For OAM/sprite definitions
typedef struct {
    unsigned short attr0;
    unsigned short attr1;
    unsigned short attr2;
    unsigned short fill;
} OBJ_ATTR;

#define OAM ((OBJ_ATTR*) OAM_BASE)

// Attribute 0 definitions
#define ATTR0_REGULAR (0 << 8)
#define ATTR0_AFFINE (1 << 8)
#define ATTR0_HIDE (2 << 8)
#define ATTR0_AFFINE_DOUBLE (3 << 8)
#define ATTR0_NORMAL (0 << 10)
#define ATTR0_BLEND (1 << 10)
#define ATTR0_WINDOW (2 << 10)
#define ATTR0_MOSAIC (1 << 12)
#define ATTR0_4BPP (0 << 13)
#define ATTR0_8BPP (1 << 13)
#define ATTR0_SQUARE (0 << 14)
#define ATTR0_WIDE (1 << 14)
#define ATTR0_TALL (2 << 14)

// Attribute 1 definitions
#define ATTR1_ROTDATA(n) ((n) << 9)
#define ATTR1_FLIP_X (1 << 12)
#define ATTR1_FLIP_Y (1 << 13)
#define ATTR1_SIZE_8 (0 << 14)
#define ATTR1_SIZE_16 (1 << 14)
#define ATTR1_SIZE_32 (2 << 14)
#define ATTR1_SIZE_64 (3 << 14)

// Attribute 2 definitions
#define ATTR2_PRIORITY(n) ((n) << 10)
#define ATTR2_PALROW(n) ((n) << 12)

// Utility functions
void waitForVBlank();
unsigned short *videoBuffer;

#endif // GBA_H
