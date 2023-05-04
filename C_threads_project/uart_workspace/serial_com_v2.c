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
        memset(rx_buffer,0x0,255);
        pthread_mutex_lock(&mutex1);
        num_bytes = read(serial_port_fd, &rx_buffer[0], sizeof(rx_buffer));
        pthread_mutex_unlock(&mutex1);
        if (num_bytes > 0){
            rx_buffer[num_bytes] = '\0';
            printf("%s", rx_buffer);
        }
        
    } 
    printf("\nreading thread closing...");
    pthread_exit(NULL);
    
}

void* write_thread(){               /*burda okuma ve yazma yapabilmek için bir rw_lock kullanmam gerekecek sanırım veya bir mutex kullanmalıyım*/
    printf("\ntesting writing thread...\n");
    // char tx_buffer[255];
    int num_bytes;
    // while (1)
    // {   
    //     memset(tx_buffer,0x0,255);
    //     fgets(tx_buffer, sizeof(tx_buffer),stdin);          //scanf("%s", tx_buffer);

    //     num_bytes = write(serial_port_fd, tx_buffer, strlen(tx_buffer));
    //     if (num_bytes <= 0)
    //         printf("\nfailed to write to serial port!\n");
        
    // }
    char ch;
    while (read(STDIN_FILENO, &ch, 1) == 1) {
        // Write character to serial port
        pthread_mutex_lock(&mutex1);
        num_bytes = write(serial_port_fd, &ch, sizeof(char));
        pthread_mutex_unlock(&mutex1);
        if (num_bytes <= 0)
            printf("\nfailed to write to serial port!\n");
        
    }

    printf("\nwriting thread closing...");
    pthread_exit(NULL);
     
}
    
    

int main(){
    
    serial_port_fd = open("/dev/ttyUSB0", O_RDWR | O_APPEND | O_NONBLOCK);             //serial port is our file descriptor(fd)
    while(serial_port_fd == -1){
        if(serial_port_fd == -1){
            printf("failed to open port\n");
            printf("error %i from open: %s\n",errno,strerror(errno));
            serial_port_fd = open("/dev/ttyUSB0", O_RDWR | O_APPEND | O_NONBLOCK);
            sleep(1);
        }
    }
    printf("port is open...\n");

    if (!isatty(serial_port_fd))
        printf("Warning: Device is not a tty device!");
    

    if(tcgetattr(serial_port_fd, &serial_port_config) != 0){
        printf("Error %i from tcgetattr: %s\n",errno,strerror(errno));
        exit(0);
    }
    cfsetispeed(&serial_port_config, B115200);          // Input baud rate
    cfsetospeed(&serial_port_config, B115200);          //  Output baud rate
    serial_port_config.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                | INLCR | IGNCR | ICRNL | IXON);
    serial_port_config.c_oflag &= ~OPOST;
    serial_port_config.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN); 
    serial_port_config.c_cflag &= ~(CSIZE | PARENB);        // clear the current character size and Disable parity bit
    serial_port_config.c_cflag |= CS8;      // Set character size to 8 bits 
    serial_port_config.c_cflag &= ~CSTOPB;  // Set stop bits to 1
    serial_port_config.c_lflag &= ~ICANON;
    serial_port_config.c_lflag &= ~ECHO;
    serial_port_config.c_lflag = CLOCAL | CREAD; 

    if (tcsetattr(serial_port_fd, TCSANOW, &serial_port_config) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    int rc_rx, rc_tx;
    pthread_t trx_id, ttx_id; 
    rc_rx = pthread_create(&trx_id,NULL,read_thread,NULL);
    rc_tx = pthread_create(&ttx_id,NULL,write_thread,NULL);
    pthread_join(trx_id, NULL);
    pthread_join(ttx_id, NULL);

    close(serial_port_fd);             //closing the serial port
    printf("\nport is closed.");
}

