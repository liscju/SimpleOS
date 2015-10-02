#include "framebuffer.h"

void kmain() {
	fb_clear();
    fb_write_str("ABECADLO\nHEJKA co tam??\b");
    fb_write_str("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
                 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    fb_write_str("Witam\nDupa");
}
