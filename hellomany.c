#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#define MAX_THREADS 15

void* PrintHello(void* data)
{
    //int iteration = (int)data;
    //pthread_join(tid,NULL);
    while(1)
        printf("Hello from new thread %lu - I was created in iteration %d...\n",pthread_self(),(int)data);
}

int main()
{
    int N;
    printf("\n how many thread do you want to create? enter a number: ");
    scanf("%d",&N);
    if( N > MAX_THREADS ) N = MAX_THREADS; 
   
    for(int i=1;i<=N;i++){
        int        rc;         	/* return value                           */
        pthread_t  thread_id;     	/* thread's ID (just an integer)          */
        int        t        ;  /* data passed to the new thread          */

        /* create a new thread that will execute 'PrintHello' */
        rc = pthread_create(&thread_id, NULL, PrintHello,(void*)i);  
        if(rc)			/* could not create thread */
        {
        printf("ERROR: return code from pthread_create is %d \n", rc);
        exit(1);
        }
        
        printf("I am main thread. Created new thread (%lu) ... in iteration %d \n", thread_id,i);
        if (i % 4 == 0) sleep(2); 
    }
     pthread_exit(NULL);		/* terminate the thread */
}