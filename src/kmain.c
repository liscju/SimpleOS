#include "framebuffer.h"

void kmain() {
	fb_clear();
    fb_write_str("ABECADLO\nHEJKA co tam??\b");
    fb_write_str("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    int i;
    fb_write_str("Witam\nDupa\n");
    for (i=4;i<25;i++) {
        fb_write_str("===TEST TEXT===\n");
    }

    fb_write_str("Koncowy\n");
    fb_write_str("END");
    fb_write_str("\nEND1\nEND2\nEND3");
    // fb_write_str("Przekraczajacy");
}
