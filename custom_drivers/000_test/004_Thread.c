#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int counter = 0U;
pthread_mutex_t lock;

void* Count_func_1(void* arg)
{
	int* thread_id = (int *)arg;
	printf("Thread id: %d\n", *thread_id);
	while(1)
	{
		pthread_mutex_lock(&lock);
		counter++;
		pthread_mutex_unlock(&lock);

		if(counter >= 100U)
		{
			printf("Count: %d\n", counter);
			pthread_exit(NULL);
		}
	}
}

void* Count_func_2(void* arg)
{
        int* thread_id = (int *)arg;
        printf("Thread id: %d\n", *thread_id);
        while(1)
	{
		pthread_mutex_lock(&lock);
       		 counter++;
       		 pthread_mutex_unlock(&lock);

		if(counter >= 200U)
        	{
                	printf("Count: %d\n", counter);
                	pthread_exit(NULL);
        	}
	}
}

void* Count_func_3(void* arg)
{
        int* thread_id = (int *)arg;
        printf("Thread id: %d\n", *thread_id);
        while(1)
	{
		pthread_mutex_lock(&lock);
        	counter++;
        	pthread_mutex_unlock(&lock);

        	if(counter >= 300U)
        	{
                	printf("Count: %d\n", counter);
                	pthread_exit(NULL);
        	}
	}

}

void* Count_func_4(void* arg)
{
        int* thread_id = (int *)arg;
        printf("Thread id: %d\n", *thread_id);
        while(1)
	{
		pthread_mutex_lock(&lock);
        	counter++;
        	pthread_mutex_unlock(&lock);

        	if(counter >= 400U)
        	{
                	printf("Count: %d\n", counter);
                	pthread_exit(NULL);
        	}
	}

}

void* Count_func_5(void* arg)
{
        int* thread_id = (int *)arg;
        printf("Thread id: %d\n", *thread_id);
        while(1)
	{
		pthread_mutex_lock(&lock);
        	counter++;
        	pthread_mutex_unlock(&lock);

        	if(counter >= 500U)
        	{
                	printf("Count: %d\n", counter);
                	pthread_exit(NULL);
        	}
	}
}

int main(void)
{
	pthread_t threads[5];
	int thread_ids[5];
	int rc;

	thread_ids[0] = 1;
	thread_ids[1] = 2;
	thread_ids[2] = 3;
	thread_ids[3] = 4;
	thread_ids[4] = 5;

	rc = pthread_create(&threads[0], NULL, Count_func_1, &thread_ids[0]);

	if(rc)
	{
		printf("Create thread 0 err: %d\n", rc);
		return -1;
	}
	
	rc = 0U;
	rc = pthread_create(&threads[1], NULL, Count_func_2, &thread_ids[1]);

        if(rc)
        {
                printf("Create thread 1 err: %d\n", rc);
                return -1;
        }
	
	rc = 0U;
	rc = pthread_create(&threads[2], NULL, Count_func_3, &thread_ids[2]);

        if(rc)
        {
                printf("Create thread 2 err: %d\n", rc);
                return -1;
        }
	
	rc = pthread_create(&threads[3], NULL, Count_func_4, &thread_ids[3]);

        if(rc)
        {
                printf("Create thread 3 err: %d\n", rc);
                return -1;
        }
	
	rc = pthread_create(&threads[4], NULL, Count_func_5, &thread_ids[4]);

        if(rc)
        {
                printf("Create thread 4 err: %d\n", rc);
                return -1;
        }

	for(int i = 0; i < 5; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	pthread_mutex_destroy(&lock);

	printf("Threads complete\n");

	return 0;
}
