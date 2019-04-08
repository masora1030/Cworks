/*   Semaphore_by_Mutex_ans.c
     17B08965
     Sora Takashima   */
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

struct t_Semaphore{
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	volatile int counter;
};
typedef struct t_Semaphore Semaphore;

void SemaphoreInit(Semaphore* sem, int c){
	pthread_mutex_init(&sem->mutex, NULL);
	pthread_cond_init(&sem->cond, NULL);
	sem->counter = c;
}

void SemaphoreTake(Semaphore* sem){
	pthread_mutex_lock(&sem->mutex);
	if (sem->counter == 0){
		pthread_cond_wait(&sem->cond, &sem->mutex);
	}
	sem->counter --;
	pthread_mutex_unlock(&sem->mutex);
}
void SemaphoreGive(Semaphore* sem){
	pthread_mutex_lock(&sem->mutex);
	sem->counter ++;
	if (sem->counter == 1){
		pthread_cond_signal(&sem->cond);
	}
	pthread_mutex_unlock(&sem->mutex);
}


pthread_t tid[2];
Semaphore avail;
Semaphore vacancy;

#define N 10
int data[N];
void* producer(void *arg){
    int i = 0;
	int write = 0;
	printf("producer start.\n");
	for(i=0; i<100;i++){
		SemaphoreTake(&vacancy);
		data[write] = i;
		printf("p%d ", data[write]);
		fflush(stdout);
		write = (write+1)%N;
		SemaphoreGive(&avail);
	}
    return NULL;
}
void* consumer(void *arg){
    int i = 0;
	int read = 0;
	printf("consumer start.\n");
	for(int i=0; i<100; ++i){
		SemaphoreTake(&avail);
		printf("c%d ", data[read]);
		fflush(stdout);
		read = (read+1)%N;
		SemaphoreGive(&vacancy);
		usleep(1000);
	}
    return NULL;
}


int main(void)
{
    int i = 0;
    int err;
    SemaphoreInit(&avail, 0);
    SemaphoreInit(&vacancy, N);
	pthread_create(&(tid[0]), NULL, &producer, NULL);
	pthread_create(&(tid[1]), NULL, &consumer, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    return 0;
}
