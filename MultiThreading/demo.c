#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int data = 5;

void* printHello(void* arg){
	sleep(1);
	printf("Printing value : %d\n", data++);
	return NULL;
}

void main(){
	int rc;
	pthread_t thread_ids[6];
	int t = 11;
	
	for(int i = 0; i < 6; i++){
		rc = pthread_create(&thread_ids[i], NULL, printHello, NULL);
	
		if(rc){
			printf("\nError: return code from pthread create is %d \n", rc);
			exit(1);
		}
	
		pthread_join(thread_ids[i], NULL);
	
		printf("Created new thread %lu..\n", thread_ids[i]);
	
		//pthread_exit(NULL);
	}
}
