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

    std::string portName;
    int baudRate;
    int dataBit;
    int stopBit;
    std::string parity;
    int interval;

    bool init();
    bool openPort();
    void closePort();
    bool configurePort(int baudRate, int dataBits, int stopBits, std::string parity);

    ssize_t sendData(char *buf, int bufSize);
    ssize_t receiveData(char *buf, int bufSize);

    bool devNodeExist();

    int isOpen; // 串口是否打开 true 打开  false 关闭

private:
    int fd;
    struct termios tty;

    bool configureTermios(int baudRate, int dataBits, int stopBits, std::string parity);
};

#endif // SERIALPORT_H
