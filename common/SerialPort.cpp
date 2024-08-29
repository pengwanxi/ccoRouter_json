
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <sys/select.h>

#include "SerialPort.h"
#include <sys/types.h>

SerialPort::SerialPort(const std::string &portName)
    : portName(portName), fd(-1) {}

SerialPort::SerialPort()
{
}

SerialPort::~SerialPort()
{
    closePort();
}

bool SerialPort::init()
{
    return openPort();
}

bool SerialPort::openPort()
{
    fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        std::cerr << "Error opening serial port " << portName << ": " << strerror(errno) << std::endl;
        return false;
    }

    return configurePort(baudRate, dataBit, stopBit, parity);
}

void SerialPort::closePort()
{
    if (fd >= 0)
    {
        close(fd);
        fd = -1;
    }
}

bool SerialPort::configurePort(int baudRate, int dataBits, int stopBits, std::string parity)
{
    if (tcgetattr(fd, &tty) != 0)
    {
        fprintf(stderr, "tcgetattr error: %s\n", strerror(errno));
        close(fd);
        return false;
    }

    return configureTermios(baudRate, dataBits, stopBits, parity);
}

bool SerialPort::configureTermios(int baudRate, int dataBits, int stopBits, std::string parity)
{

    struct termios new_cfg = {0}; // 将 new_cfg 对象清零
    speed_t speed;

    /* 设置为原始模式 */
    cfmakeraw(&new_cfg);

    /* 使能接收 */
    new_cfg.c_cflag |= CREAD;

    /* 设置波特率 */
    switch (baudRate)
    {
    case 1200:
        speed = B1200;
        break;
    case 1800:
        speed = B1800;
        break;
    case 2400:
        speed = B2400;
        break;
    case 4800:
        speed = B4800;
        break;
    case 9600:
        speed = B9600;
        break;
    case 19200:
        speed = B19200;
        break;
    case 38400:
        speed = B38400;
        break;
    case 57600:
        speed = B57600;
        break;
    case 115200:
        speed = B115200;
        break;
    case 230400:
        speed = B230400;
        break;
    case 460800:
        speed = B460800;
        break;
    case 500000:
        speed = B500000;
        break;
    default: // 默认配置为 115200
        speed = B115200;
        printf("default baud rate: 115200\n");
        break;
    }

    if (0 > cfsetspeed(&new_cfg, speed))
    {
        fprintf(stderr, "cfsetspeed error: %s\n", strerror(errno));
        return -1;
    }

    /* 设置数据位大小 */
    new_cfg.c_cflag &= ~CSIZE; // 将数据位相关的比特位清零
    switch (dataBits)
    {
    case 5:
        new_cfg.c_cflag |= CS5;
        break;
    case 6:
        new_cfg.c_cflag |= CS6;
        break;
    case 7:
        new_cfg.c_cflag |= CS7;
        break;
    case 8:
        new_cfg.c_cflag |= CS8;
        break;
    default: // 默认数据位大小为 8
        new_cfg.c_cflag |= CS8;
        printf("default data bit size: 8\n");
        break;
    }

    /* 设置奇偶校验 */
    if (parity == "even")
    {
        new_cfg.c_cflag |= PARENB;
        new_cfg.c_cflag &= ~PARODD; /* 清除 PARODD 标志，配置为偶校验 */
        new_cfg.c_iflag |= INPCK;
    }
    else if (parity == "odd")
    {
        new_cfg.c_cflag |= (PARODD | PARENB);
        new_cfg.c_iflag |= INPCK;
    }
    else if (parity == "none")
    {
        new_cfg.c_cflag &= ~PARENB;
        new_cfg.c_iflag &= ~INPCK;
    }
    else
    {
        new_cfg.c_cflag &= ~PARENB;
        new_cfg.c_iflag &= ~INPCK;
        printf("default parity: none\n");
    }

    /* 设置停止位 */
    switch (stopBits)
    {
    case 1: // 1 个停止位
        new_cfg.c_cflag &= ~CSTOPB;
        break;
    case 2: // 2 个停止位
        new_cfg.c_cflag |= CSTOPB;
        break;
    default: // 默认配置为 1 个停止位
        new_cfg.c_cflag &= ~CSTOPB;
        printf("default stop bit size: 1\n");
        break;
    }

    /* 将 MIN 和 TIME 设置为 0 */
    new_cfg.c_cc[VTIME] = 0;
    new_cfg.c_cc[VMIN] = 0;

    /* 清空缓冲区 */
    if (0 > tcflush(fd, TCIOFLUSH))
    {
        fprintf(stderr, "tcflush error: %s\n", strerror(errno));
        return -1;
    }

    /* 写入配置、使配置生效 */
    if (0 > tcsetattr(fd, TCSANOW, &new_cfg))
    {
        fprintf(stderr, "tcsetattr error: %s\n", strerror(errno));
        return -1;
    }
    /* 配置 OK 退出 */
    return true;
}

ssize_t SerialPort::sendData(unsigned char *buf, int bufSize)
{

    int n;
    int sum = 0;
    while (sum < bufSize)
    {
        bufSize -= sum;
        n = write(fd, buf + sum, bufSize);
        sum += n;
    }
    return sum;
}

ssize_t SerialPort::receiveData(unsigned char *buf, int bufSize)
{

    int ret = 0;
    struct timeval timev_frame, timev_byte;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    timev_frame.tv_sec = 10;
    timev_frame.tv_usec = 0;
    int ret_val = select(fd + 1, &rfds, NULL, NULL, &timev_frame);
    if (-1 == ret_val)
    {
        perror("select1");
    }
    else if (0 == ret_val)
    {
        return ret;
    }
    else
    {
        if (FD_ISSET(fd, &rfds))
        {
            for (ret = 0; ret < bufSize;)
            {
                int tmp_len = read(fd, buf + ret, bufSize - ret);
                if (tmp_len > 0)
                {
                    ret += tmp_len;
                    if (ret >= bufSize)
                    {
                        return ret;
                    }
                }

                fd_set byte_fd;
                // 处理接收帧超时句柄
                FD_ZERO(&byte_fd);
                FD_SET(fd, &byte_fd);
                int once_max_bytes = 30;
                unsigned long byte_wait_time =
                    (1000 * (once_max_bytes + 3) * 11 * 1000) / baudRate;
                timev_byte.tv_sec = byte_wait_time / 1000000;
                timev_byte.tv_usec = byte_wait_time % 1000000;
                int byte_ret =
                    select((fd) + 1, &byte_fd, NULL, NULL, &timev_byte);
                if (-1 == byte_ret)
                {
                    perror("byte select");
                }
                else if (0 == byte_ret)
                {

                    return ret;
                }
                else
                {
                    if (ret >= bufSize)
                        return ret;
                }
            }
        }
    }
    /* ret = read(*fdp, buf, len); */
    return ret;
}
