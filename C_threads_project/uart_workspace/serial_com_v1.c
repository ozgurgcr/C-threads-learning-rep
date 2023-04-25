#include<stdio.h>               //Standart input output functions 
#include<stdlib.h>              //
#include<unistd.h>              //UNIX standart funtion definitions 
#include<pthread.h>             //
#include<termios.h>             //POSIX terminal control definitions 
#include<fcntl.h>               //File control definitions 
#include<errno.h>               //error number definitions 
#include<string.h>              //string function definitions 

struct termios serial_port_config;                               
int serial_port_fd;         //fd - file descriptor

void* read_thread(){
    char rx_buffer[256];
    while(1){
        int num_bytes = read(serial_port_fd, &rx_buffer, sizeof rx_buffer);
        printf("%s", rx_buffer);
    } 
    exit(0);
}

int main(){
    
    serial_port_fd = open("/dev/ttyUSB0", O_RDWR);             //serial port is our file descriptor(fd)
    while(serial_port_fd == -1){
        if(serial_port_fd == -1){
            printf("failed to open port\n");
            sleep(1);
        }else 
            printf("port is open...\n");
    }
    if (!isatty(serial_port_fd))
        printf("device is not a tty device!");
    

    if(tcgetattr(serial_port_fd, &serial_port_config) != 0){
        printf("error %i from tcgetattr: %s\n",errno,strerror(errno));
        exit(0);
    }
    cfsetispeed(&serial_port_config, B115200);
    cfsetospeed(&serial_port_config, B115200);
    cfmakeraw(&serial_port_config);
    tcsetattr(serial_port_fd,TCSANOW,&serial_port_config);

    int rc;
    pthread_t t_id; 
    rc = pthread_create(&t_id,NULL,read_thread,NULL);
    pthread_join(t_id, NULL);

    // write(serial_port_fd, const void *tx_buffer, size_t tx_size);
    // char rx_buffer[256];
    // int num_bytes = read(serial_port_fd, &rx_buffer, sizeof rx_buffer);
    // printf("%s", rx_buffer);
    // printf("\nport is open \n");
    close(serial_port_fd);             //closing the serial port
}

