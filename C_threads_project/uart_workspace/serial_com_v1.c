#include<stdio.h>               //Standart input output functions 
#include<stdlib.h>              //
#include<unistd.h>              //UNIX standart funtion definitions 
#include<pthread.h>             //
#include<termios.h>             //POSIX terminal control definitions 
#include<fcntl.h>               //File control definitions 
#include<errno.h>               //error number definitions 
#include<string.h>              //string function definitions 
                                //fd - file descriptor

int main(){
    int serial_port;
    serial_port = open("/dev/ttyUSB0", O_RDWR);             //serial port is our file descriptor(fd)
    if(serial_port == -1){
        printf("failed to open port\n");
        exit(0);
    }else 
        printf("port is open...\n");
    if (!isatty(serial_port))
        printf("device is not a tty device!");

    struct termios serial_port_config;

    if(tcgetattr(serial_port, &serial_port_config) != 0){
        printf("error %i from tcgetattr: %s\n",errno,strerror(errno));
        exit(0);
    }
    cfsetispeed(&serial_port_config, B115200);
    cfsetospeed(&serial_port_config, B115200);
    cfmakeraw(&serial_port_config);
    tcsetattr(serial_port,TCSANOW,&serial_port_config);

    // write(serial_port, const void *tx_buffer, size_t tx_size);
    // read(serial_port, void *rx_buffer, size_t rx_size);

    close(serial_port);             //closing the serial port
}