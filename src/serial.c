#include "serial.h"
#include "io.h"

/* The I/O ports */

/** All the I/O ports are calculated relative to the
 *  data port. This is because all serial ports
 *  (COM1, COM2, COM3, COM4) have their ports in the
 *  same order, but they start at different values.
*/

#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

#define SERIAL_BAUD_RATE_DIVISOR 0x03 /* will give a baud rate of 115200 / 3 = 38400 */

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits
 * on the data port, then the lowest 8 bits will follow
*/
#define SERIAL_LINE_ENABLE_DLAB 0x80

/** Private functions */

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed
 *  of a serial port is 115200 bits/s. The argument is a
 *  divisor of that number, hence the resulting speed
 *  becomes (115200 / divisor) bits/s.
 *
 *  @param com     The COM port to configure
 *  @param divisor The divisor
*/
static void serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
    outb(SERIAL_LINE_COMMAND_PORT(com),
         SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com),
         (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com),
         divisor & 0x00FF);
}


/** serial_configure_line:
 *  Configures the line of the given serial port. The port is set
 *  to have a data length of 8 bits, no parity bits, one stop bit
 *  and break control disabled
 *
 *  @param com The serial port to configure
*/
static void serial_configure_line(unsigned short com) {
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s | dl  |
     * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
     *
     * where
     *  d - enabled(d=1) or disables DLAB
     *  b - if break control is enabled(b=1) or disabled
     *  prty - number of parity bits to use
     *  s - the number of stop bits to use (s=0 => 1, s=1 => 1.5 or 2)
     *  dl - describes the length of the data
    */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_configure_modem
 *  Configures the modem of the given serial port. We are setting
 *  RTS(Ready to Transmit) and DTR(Data Terminal Ready) to 1 which
 *  means that we are ready to send data
*/
static void serial_configure_modem(unsigned short com) {
    /** Bit:     | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
     *  Content: | r  | r  | af | lb | ao2| ao1| rts| dtr|
     *  Value:   | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 1  | = 0x03
     *  where
     *      r - reserved
     *      af - autoflow control enabled
     *      lb - loopback mode(used for debugging serial ports)
     *      ao2 - auxiliary output 2, used for receiving interrupts
     *      ao1 - auxiliary output 1
     *      rts - ready to transmit
     *      dtr - data terminal ready
    */
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

/** serial_initialize_port:
 *  Initializes given port
 *
 *  @param com port to initialize
*/
static void serial_initialize_port(unsigned short com) {
    serial_configure_baud_rate(com, SERIAL_BAUD_RATE_DIVISOR);
    serial_configure_line(com);
    serial_configure_modem(com);
}

/* Public Functions*/

/** serial_initialize:
 *  Initializes com ports, must be invoked
 *  before reading/writing from COM ports
*/
void serial_initialize() {
    serial_initialize_port(SERIAL_COM1_PORT);
}

/** serial_write:
 *  Writes data to the given port
 *
 *  @param com port to write data
 *  @param data
*/
void serial_write(unsigned short com, unsigned char data) {
    outb(SERIAL_DATA_PORT(com), data);
}
