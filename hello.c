#include <stdio.h>      // standard I/O routines
#include <stdlib.h>
#include <unistd.h>     // for sleep thread 
#include <pthread.h>    // pthread functions and data structures

// function to be executed by the new thread 

void* PrintHello(void* data)
{   
    // int my_data = (int)data;    // data recevied by thread
    pthread_t tid = (pthread_t)data;
    pthread_join(tid,NULL);
    //pthread_detach(pthread_self());     // pthread_self() gets thread id
    //pthread_detach() is make the thread detached. detached threads cannot be reaped or killed by other threads.
    printf("Hello from new thread %lu - got %lu \n",pthread_self(), data);
    pthread_exit(NULL);         // terminate the thread 
}
// like any C program, program's execution begins in main 
int main(int argc, char* argv[])
{
    int              rc;    //return value             
    pthread_t thread_id;    // thread's ID (just an integer)    // data passed to the new thread 
    pthread_t t_id;
    t_id = pthread_self();

    //create a new thread that will be execute "PrintHello"
    rc = pthread_create(&thread_id, NULL, PrintHello, (void*)t_id);
    if(rc)                  // could not create thread 
    {
        printf("\n ERROR: return code from pthread_create is %d \n", rc);
        exit(1);
    }
    sleep(1);
    printf("I am thread %lu . Created new thread (%lu)... \n", t_id, thread_id);
    pthread_exit(NULL);        // terminate the thread
}
