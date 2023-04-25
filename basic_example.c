#include <pthread.h>
#include <stdio.h>

char * buf = "abcdefghijklmnopqrstuvwxyz";
int num_pthreads = 2;
int count = 10;
int fd = 1;

void * new_thread(void * arg)
{
    int i;

    for (i = 0; i < count; i++) {
	write(fd, arg, 1);
	sleep(1);
    }
    return(NULL);
}

main()
{
   pthread_t thread;
   int i;

   for (i = 0; i < num_pthreads; i++) {
	if (pthread_create(&thread, NULL, new_thread, (void *)(buf + i))) {
	   fprintf(stderr, "error creating a new thread \n");
	   exit(1);
	}
	pthread_detach(thread);
   }
   pthread_exit(NULL);
}