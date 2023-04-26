#include<stdio.h>               //Standart input output functions 
#include<stdlib.h>              //
#include<unistd.h>              //UNIX standart funtion definitions 
#include<pthread.h>             //
#include<termios.h>             //POSIX terminal control definitions 
#include<fcntl.h>               //File control definitions 
#include<errno.h>               //error number definitions 
#include<string.h>              //string function definitions 
#include <sys/ioctl.h>

struct termios serial_port_config;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int serial_port_fd;         //fd - file descriptor


void* read_thread(){             /*burda okuma ve yazma yapabilmek için bir rw_lock kullanmam gerekecek sanırım veya bir mutex kullanmalıyım*/
    printf("\ntesting reading thread...\n");
    char rx_buffer[255];
    int num_bytes;
    while(1){
        num_bytes = read(serial_port_fd, &rx_buffer, sizeof rx_buffer);
        if (num_bytes > 0){
            rx_buffer[num_bytes] = '\0';
            printf("\n%s", rx_buffer);
        }
        
    } 
    printf("\nreading thread closing...");
    pthread_exit(NULL);
    
}

void* write_thread(){               /*burda okuma ve yazma yapabilmek için bir rw_lock kullanmam gerekecek sanırım veya bir mutex kullanmalıyım*/
    char tx_buffer[255];
    printf("\ntesting writing thread...\n");
    int num_bytes;
    while (1)
    {   
        fgets(tx_buffer, sizeof(tx_buffer),stdin);          //scanf("%s", tx_buffer);
        num_bytes = write(serial_port_fd, tx_buffer, strlen(tx_buffer));
        if (num_bytes <= 0)
            printf("failed to write to serial port!\n");
        sleep(1);
        
    }
    printf("\nwriting thread closing...");
    pthread_exit(NULL);
     
}
    
    

int main(){
    
    serial_port_fd = open("/dev/ttyUSB0", O_RDWR);             //serial port is our file descriptor(fd)
    while(serial_port_fd == -1){
        if(serial_port_fd == -1){
            printf("failed to open port\n");
            printf("error %i from open: %s\n",errno,strerror(errno));
            serial_port_fd = open("/dev/ttyUSB0", O_RDWR);
            sleep(1);
        }
    }
    printf("port is open...\n");

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

    int rc_rx, rc_tx;
    pthread_t trx_id, ttx_id; 
    rc_rx = pthread_create(&trx_id,NULL,read_thread,NULL);
    rc_tx = pthread_create(&ttx_id,NULL,write_thread,NULL);
    pthread_join(trx_id, NULL);
    pthread_join(ttx_id, NULL);

    // write(serial_port_fd, const void *tx_buffer, size_t tx_size);
    // char rx_buffer[256];
    // int num_bytes = read(serial_port_fd, &rx_buffer, sizeof rx_buffer);
    // printf("%s", rx_buffer);
    // printf("\nport is open \n");
    close(serial_port_fd);             //closing the serial port
    printf("\nport is closed.");
}

