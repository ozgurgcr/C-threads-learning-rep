// this is using for testing thread functions
#include<stdio.h>               //Standart input output functions 
#include<unistd.h>              //UNIX standart funtion definitions 
#include<stdlib.h>              //
#include<pthread.h>             //
#include<signal.h>              //
#include<termios.h>             //POSIX terminal control definitions 
#include<fcntl.h>               //File control definitions 
#include<errno.h>               //error number definitions 
#include<string.h>              //string function definitions 
int main(){
    char ch;
    ch = getchar();
    putchar(ch);
    printf("\n%ld\n",sizeof(ch));
}
