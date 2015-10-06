#include "framebuffer.h"
#include "serial.h"

char *BOOT_MSG =
    "Welcome to the SimpleOS\n"
    "OS was developed only for learning sth about building\n"
    "operating system and is not ready for production\n"
    "\tUSE WITH CAUTION!!!\n"
    "author: liscju <piotr.listkiewicz@gmail.com>\n";

void kmain() {
	fb_clear();
    fb_write_str(BOOT_MSG);
    serial_initialize();
    serial_write_str(SERIAL_COM1_PORT, "Hello Serial World\n");
}
