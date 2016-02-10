# c2gen
Simple Silabs programmer for Arduino, load it with the Arduino ide.

Pin 11 is the clock (C2K), pin 10 is the data (C2D).

Usage:

Use c2tool to program the mcu.

CMD:

./c2tool /dev/ttyUSB0 info
DEVICEID 04, REVID 02
C8051F30x family: 512 byte pagesize, FPDAT at 0xb4
PI version 05, derivative 20

./c2tool /dev/ttyUSB0 flash test_p0_p1_p2_p3.hex
section .sec1: lma 0000000000000003, size 75 -> flash 00000003
section .sec2: lma 0000000000000000, size 3 -> flash 00000000
section .sec3: lma 000000000000004e, size 12 -> flash 0000004e

./c2tool /dev/ttyUSB0 verify test_p0_p1_p2_p3.hex
section .sec1: lma 0000000000000003, size 75 -> flash 00000003
section .sec1: verify ok
section .sec2: lma 0000000000000000, size 3 -> flash 00000000
section .sec2: verify ok
section .sec3: lma 000000000000004e, size 12 -> flash 0000004e
section .sec3: verify ok




./c2tool /dev/ttyUSB0 erase
./c2tool /dev/ttyUSB0 verify test_p0_p1_p2_p3.hex
section .sec1: lma 0000000000000003, size 75 -> flash 00000003
verify failed in 75 byte chunk at 00000003
section .sec1: verify ok
section .sec2: lma 0000000000000000, size 3 -> flash 00000000
verify failed in 3 byte chunk at 00000000
section .sec2: verify ok
section .sec3: lma 000000000000004e, size 12 -> flash 0000004e
verify failed in 12 byte chunk at 0000004e
section .sec3: verify ok
command failed: Invalid argument (-22)

