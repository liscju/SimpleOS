#include "framebuffer.h"

void kmain() {
	fb_clear();
    fb_write_char('A');
    fb_write_char('\n');
    fb_write_char('B');
    fb_write_char('C');
    fb_write_char('D');
    fb_write_char('\b');
}
