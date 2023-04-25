// UART seri arayüz programı
// senkron yazma ve okuma yapabilecek 
// yazma için bir thread 
// hafıza için bir thread 
// sinyal komutları için bir thread
 
#include<stdio.h>               //Standart input output functions 
#include<stdlib.h>              //
#include<unistd.h>              //UNIX standart funtion definitions 
#include<pthread.h>             //
#include<termios.h>             //POSIX terminal control definitions 
#include<fcntl.h>               //File control definitions 
#include<errno.h>               //error number definitions 
#include<string.h>              //string function definitions 
                                //fd - file descriptor


void * thread_function1(){

    while(1){
        
    }
        
}

void * thread_function2(){
    
}

void * thread_function3(void* data){
    char * memory = (char*) data; 

}

void * thread_function4(){
    
}

int main(){
    int serial_port;
    serial_port = open("/dev/ttyUSB0", O_RDWR);             //serial port is our file descriptor(fd)
    if(serial_port == -1){
    printf("failed to open port\n");
    exit(0);
    }else 
        printf("port is open...\n");
    if (!isatty(serial_port)){
        printf("device is not a tty device!");
        close(serial_port);  
        exit(0);
    }
    struct termios serial_port_config;

    if(tcgetattr(serial_port, &serial_port_config) != 0){
        printf("error %i from tcgetattr: %s\n",errno,strerror(errno));
        close(serial_port);  
        exit(0);
    }
    cfsetispeed(&serial_port_config, B115200);
    cfsetospeed(&serial_port_config, B115200);
    cfmakeraw(&serial_port_config);
    tcsetattr(serial_port,TCSANOW,&serial_port_config);

    
    pthread_t thread_id1;
    pthread_t thread_id2;
    pthread_t thread_id3;
    pthread_t thread_id4;
    
    int rc1, rc2, rc3, rc4; 

    rc1 = pthread_create(&thread_id1, NULL, *thread_function1, NULL);
    rc2 = pthread_create(&thread_id2, NULL, *thread_function2, NULL);
    rc3 = pthread_create(&thread_id3, NULL, *thread_function3, NULL);
    rc4 = pthread_create(&thread_id4, NULL, *thread_function4, NULL);

    close(serial_port);             //closing the serial port

    
}
