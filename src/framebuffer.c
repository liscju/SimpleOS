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
#define TO_ADDR(row, col) (2*((row)*FB_COLUMN_COUNT+(col)))

// Implementation methods

static void _fb_set_cell(unsigned int pos, char c, unsigned char fg, unsigned char bg) {
	fb[pos] = c;
    fb[pos + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

static void _fb_shift_buffer_up() {
    int row,col;
    for (row=1;row<FB_ROW_COUNT;row++) {
        for (col=0;col<FB_COLUMN_COUNT;col++) {
            fb[TO_ADDR(row-1,col)] = fb[TO_ADDR(row,col)];
            fb[TO_ADDR(row-1,col)+1] = fb[TO_ADDR(row,col)+1];
        }
    }
}

static void _fb_clear_row(unsigned int row) {
    int i;
    for (i=0;i<FB_COLUMN_COUNT;i++) {
        _fb_set_cell(TO_ADDR(row, i), 0, FB_WHITE, FB_BLACK);
    }
}

/** Move inner cursor function deals with moving
 *  inner pointer to next place in framebuffer
 *  to write into, they have nothing to do
 *  with framebuffer cursor which is displayed on
 *  the screen
*/

static void _fb_move_inner_cursor_to_next_line() {
    if (cursor_pos_row < FB_ROW_COUNT-1) {
        cursor_pos_row++;
        cursor_pos_col = 0;
    } else {
        _fb_shift_buffer_up();
        _fb_clear_row(FB_ROW_COUNT - 1);

        cursor_pos_row = FB_ROW_COUNT - 1;
        cursor_pos_col = 0;
    }
}

static void _fb_move_inner_cursor_right() {
    if (cursor_pos_col < FB_COLUMN_COUNT-1) {
        cursor_pos_col++;
    } else {
        _fb_move_inner_cursor_to_next_line();
    }
}

static void _fb_move_inner_cursor_left() {
    if (cursor_pos_col > 0) {
        cursor_pos_col--;
    }
}

static void _fb_set_cursor(unsigned short pos) {
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00FF);
}

// Public methods

/** fb_clear:
  * Makes all framebuffer cells black with whiteforeground,
  * moves cursor to the beggining of the buffer
*/
void fb_clear() {
	int i,j;
    cursor_pos_row = cursor_pos_col = 0;
	for (i=0;i<FB_ROW_COUNT;i++) {
		for (j=0;j<FB_COLUMN_COUNT;j++) {
			_fb_set_cell(TO_ADDR(i, j), 0, FB_WHITE, FB_BLACK);
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
  * ,changes inner cursor position
  * @param row
  * @param col
*/
void fb_move_cursor(unsigned int row, unsigned int col) {
    cursor_pos_row = row;
    cursor_pos_col = col;
    _fb_set_cursor(row * FB_COLUMN_COUNT + col);
}

/** fb_write_char
 *  Writes char to the output in current cursor place and move it
 *  to next place
 *  @param c char to write
*/
void fb_write_char(char c) {
    if (c == '\n') {
        _fb_move_inner_cursor_to_next_line();
    } else if (c == '\b') {
        fb_write_cell(cursor_pos_row, cursor_pos_col, 0, FB_WHITE, FB_BLACK);
        _fb_move_inner_cursor_left();
        fb_write_cell(cursor_pos_row, cursor_pos_col, 0, FB_WHITE, FB_BLACK);
    } else if (c == '\t') {
        int i;
        for (i=0;i<4;i++) {
            fb_write_char(' ');
        }
    } else {
        fb_write_cell(cursor_pos_row, cursor_pos_col, c, FB_WHITE, FB_BLACK);
        _fb_move_inner_cursor_right();
    }
    fb_move_cursor(cursor_pos_row, cursor_pos_col);
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



