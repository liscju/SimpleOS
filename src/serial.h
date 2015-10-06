#ifndef __SERIAL_H__
#define __SERIAL_H__

#define SERIAL_COM1_PORT 0x3F8 /* COM1 base port*/

void serial_initialize();
void serial_write_char(unsigned short com, unsigned char data);
void serial_write_str(unsigned short com, char* data);

#endif /* __SERIAL_H__ */
