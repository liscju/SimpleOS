#include "framebuffer.h"

char *BOOT_MSG =
    "Welcome to the SimpleOS\n"
    "OS was developed only for learning sth about building\n"
    "operating system and is not ready for production\n"
    "\tUSE WITH CAUTION!!!\n"
    "author: liscju <piotr.listkiewicz@gmail.com>\n";

void kmain() {
	fb_clear();
    fb_write_str(BOOT_MSG);
}
