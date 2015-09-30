#include "framebuffer.h"

void kmain() {
	fb_clear();
	fb_write_cell(0, 'A', FB_GREEN, FB_DARK_GREY);
}
