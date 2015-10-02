#include "framebuffer.h"

static char *fb = (char *) 0x000B8000;

/** cursor_pos_x, cursor_pos_y
 *  Keeps current position of cursor in framebuffer
 *  WARNING: The exact position in framebuffer must
 *  be later calculated
*/
//static unsigned int cursor_pos_x = 0;
//static unsigned int cursor_pos_y = 0;

/** TO_ADDR:
 * From row,col calculate position in frambuffer
*/
#define TO_ADDR(row, col) (2*(row*FB_COLUMN_COUNT+col))

// Implementation methods

static void _fb_set_cell(unsigned int pos, char c, unsigned char fg, unsigned char bg) {
	fb[pos] = c;
	fb[pos + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

static void _fb_set_cursor(unsigned short pos) {
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00FF);
}

// Public methods

/** fb_clear:
  * Makes all framebuffer cells black
*/
void fb_clear() {
	int i,j;
	for (i=0;i<FB_ROW_COUNT;i++) {
		for (j=0;j<FB_COLUMN_COUNT;j++) {
			fb_write_cell(i, j, 0, FB_BLACK, FB_BLACK);
		}
	}
}

/** fb_write_cell:
  * Write a character with the given foreground and background
  * in the framebuffer
  *
  * @param row Row of framebuffer
  * @param col Col of framebuffer
  * @param c  The character
  * @param fg The foreground color
  * @param bg The background color
*/
void fb_write_cell(unsigned int row, unsigned int col, char c, unsigned char fg, unsigned char bg) {
    _fb_set_cell(TO_ADDR(row, col), c, fg, bg);
}

/** fb_move_cursor:
  * Moves the cursor of the framebuffer to the given position
  * @param row
  * @param col
*/
void fb_move_cursor(unsigned int row, unsigned int col) {
    _fb_set_cursor(TO_ADDR(row, col));
}









