#include "framebuffer.h"

static char *fb = (char *) 0x000B8000;

/** cursor_pos_x, cursor_pos_y
 *  Keeps current position of cursor in framebuffer
 *  WARNING: The exact position in framebuffer must
 *  be later calculated
*/
static unsigned int cursor_pos_row = 0;
static unsigned int cursor_pos_col = 0;

/** TO_ADDR:
 * From row,col calculate position in frambuffer
*/
#define TO_ADDR(row, col) (2*(row*FB_COLUMN_COUNT+col))

// Implementation methods

static void _fb_move_cursor_to_next_line() {
    cursor_pos_row++;
    cursor_pos_col = 0;
}

static void _fb_move_cursor_right() {
    cursor_pos_col++;
}

static void _fb_move_cursor_left() {
    if (cursor_pos_col > 0) {
        cursor_pos_col--;
    }
}

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
    cursor_pos_row = cursor_pos_col = 0;
	for (i=0;i<FB_ROW_COUNT;i++) {
		for (j=0;j<FB_COLUMN_COUNT;j++) {
			_fb_set_cell(TO_ADDR(i, j), 0, FB_BLACK, FB_BLACK);
		}
	}
}

/** fb_write_cell:
  * Write a character with the given foreground and background
  * in the framebuffer
  * WARNING: It is not moving cursor
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
    cursor_pos_row = row;
    cursor_pos_col = col;
    _fb_set_cursor(TO_ADDR(row, col));
}

/** fb_write_char
 *  Writes char to the output in current cursor place and move it
 *  to next place
 *  @param c char to write
*/
void fb_write_char(char c) {
    if (c == '\n') {
        _fb_move_cursor_to_next_line();
    } else if (c == '\b') {
        _fb_move_cursor_left();
        fb_write_cell(cursor_pos_row, cursor_pos_col, 0, FB_BLACK, FB_BLACK);
        _fb_move_cursor_left();
    } else {
        fb_write_cell(cursor_pos_row, cursor_pos_col, c, FB_WHITE, FB_BLACK);
        _fb_move_cursor_right();
    }
}

/** fb_write_str
 *  Write string to the output moving cursor to new place
 *  @param str string to write
*/
void fb_write_str(char* str) {
    char str_tmp;
    while ((str_tmp=*str++)) {
        fb_write_char(str_tmp);
    }
}



