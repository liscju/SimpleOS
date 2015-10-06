#ifndef __SERIAL_H__
#define __SERIAL_H__

#define SERIAL_COM1_PORT 0x3F8 /* COM1 base port*/

void serial_initialize();
void serial_write(unsigned short com, unsigned char data);

#endif /* __SERIAL_H__ */
