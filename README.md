## DGPS

* Read GPGGA and other messages and calculate errors for GPS output ```LOG.txt``` is output from serial port for GPS receivers.

* We find **GPGGA** from ```LOG.txt``` and convert them to meter from longitude and latitude using ```art/UTM.h```.
