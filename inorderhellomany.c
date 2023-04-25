#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter ; 

void* PrintHello(void* data)
{   
    
    while(counter < 100){
        pthread_mutex_lock(&mutex1);
        counter=counter +1;
        printf("Hello from new thread %ld - I increase the counter to: %d. \n ---------------------- \n", pthread_self(), counter);
        pthread_mutex_unlock(&mutex1);
    }


}

int main()
{
    pthread_t  thread_id[5];     	                            /* thread's ID (just an integer)  */ 
    int        rc;         	                                    /* return value                           */
    int        t        ;                                       /* data passed to the new thread          */
    for(int i=0;i<5;i++){
        rc = pthread_create(&thread_id[i], NULL, PrintHello,(void*)i);
        if(rc)			
        {
        printf("ERROR: return code from pthread_create is %d \n", rc);
        exit(1);
        }
        printf("I am main thread. New thread (%ld) creataed in iteration %d \n", thread_id[i],i);
    } 
    for (int j = 0;j<5;j++){
    pthread_join(thread_id[j],NULL);    
    }
     pthread_exit(NULL);		/* terminate the thread */
}