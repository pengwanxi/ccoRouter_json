#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>
#include <vector>
#include <termios.h>

class SerialPort
{
public:
    SerialPort(const std::string &portName);
    SerialPort();
    ~SerialPort();

    bool init();
    bool openPort();
    void closePort();
    bool configurePort(int baudRate, int dataBits, int stopBits, std::string parity);

    ssize_t sendData(unsigned char *buf, int bufSize);

    ssize_t receiveData(unsigned char *buf, int bufSize);

    std::string portName;
    int baudRate;
    int dataBit;
    int stopBit;
    std::string parity;
    int interval;

private:
    int fd;
    struct termios tty;

    bool configureTermios(int baudRate, int dataBits, int stopBits, std::string parity);
};

#endif // SERIALPORT_H
