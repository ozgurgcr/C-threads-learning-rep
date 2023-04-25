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


void* thread_function1(){

    char* array[10];
    char str[150];
    
    while(1){
            printf("\nthread basladi...\n");                      // test code 
            scanf("%s",str);
            printf("string alindi, array öteleniyor...");     //test code
            array[0] =str;
            for(int i = 9;i>0;i--){
                array[i]=array[i-1];
                printf("\narray[%d]:%s",i,array[i]);
            }
            printf("\n--array[0]:%s",array[0]); 
            //                                                // printf("\narray ötelendi");         //test code
             //test code
            // int print_control = strcmp(array[0], "print");
            // if(print_control == 0){
            //     printf("\nprint komutu algılandı...");
            //     for(int i = 0;i<10;i++)
            //         printf("\narray[%d]:%s",i,array[i]);
            // }
            printf("\nson------------------------------------------------\n"); 
        }
}

int main(int argc, char* argv[]){
    printf("\ntest...");
    pthread_t thread_id1;
    int rc1;
    rc1 = pthread_create(&thread_id1, NULL, thread_function1, NULL);
    if(rc1)
        printf("\nERROR");
    printf("\nthread oluşturuldu...");
    // sleep(2);
    pthread_join(thread_id1, NULL);
    printf("...................................");
    pthread_exit(NULL);
}

