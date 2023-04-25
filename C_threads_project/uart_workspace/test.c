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
    char array[10][100] = { };        //"1","2","3","4","5","6","7","8","9","10"
    char str[100];
    
    while(1){
            scanf("%s",str);
            for(int i = 9;i>=1;i--){
                strcpy(array[i], array[i-1]);
                //printf("\narray[%d]:%s",i,array[i]);
            }
            //str[strcspn(str, "\n")] = '\0'; // remove newline character from input
            strcpy(array[0],str);
            printf("\n--array[0]:%s",array[0]); 
            printf("\nson------------------------------------------------\n");
            for(int i = 0;i<10;i++)
                printf("\narray[%d]: %s", i, array[i]); 
        }
}
