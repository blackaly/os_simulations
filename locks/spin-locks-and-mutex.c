#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>


typedef struct {
	atomic_flag locked;

} spinlock_t;

void spinlock_init(spinlock_t* lock)
{
	atomic_flag_clear(&lock->locked);
}

void spinlock_lock(spinlock_t* lock)
{
	while(atomic_flag_test_and_set(&lock->locked))
	{}
}

void spinlock_unlock(spinlock_t* lock)
{
	atomic_flag_clear(&lock->locked);
}

typedef struct
{
	volatile int lock;
} basic_spinlock_t;


void basic_spinlock_init(basic_spinlock_t* lock)
{
	lock->lock = 0;
}

void basic_spinlock_lock(basic_spinlock_t* lock)
{
	while(__sync_lock_test_and_set(&lock->lock, 1)) {}
}

void basic_spinlock_unlock(basic_spinlock_t* lock)
{
	__sync_lock_release(&lock->lock);
}


pthread_mutex_t mutex;
spinlock_t spinlock;
int shared_counter = 0;

void* mutex_thread_function(void* arg)
{
	int thread_id = *(int*)arg;

	for(int i = 0; i < 100000; ++i)
	{
		pthread_mutex_lock(&mutex);
		shared_counter++;
		pthread_mutex_unlock(&mutex);
	}

	printf("Mutex Thread %d completed\n", thread_id);

	return NULL;
}

void* spinlock_thread_function(void* arg)
{
	int thread_id = *(int*)arg;

	for(int i = 0; i < 100000; ++i)
	{
		spinlock_lock(&spinlock);
		shared_counter++;
		spinlock_unlock(&spinlock);
	}

	printf("Spinlock Thread %d completed\n", thread_id);
	return NULL;

}

int main()
{

	pthread_t threads[4];
	int thread_ids[4] = {1, 2, 3, 4};

	printf("\n=== Mutex Example ===\n");
	pthread_mutex_init(&mutex, NULL);

	shared_counter = 0;

	for(int i = 0; i < 4; ++i)
	{
		pthread_create(&threads[i], NULL, mutex_thread_function, &thread_ids[i]);
	}

	for(int i = 0; i < 4; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	printf("Mutex final counter value: %d\n", shared_counter);
	pthread_mutex_destroy(&mutex);

	printf("\n=== Spinlock Example ===\n");

	spinlock_init(&spinlock);

	shared_counter = 0;

	for(int i = 0; i < 4; ++i)
	{
		pthread_create(&threads[i], NULL, spinlock_thread_function, &thread_ids[i]);
	}

	for(int i = 0; i < 4; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	printf("Spinlock final counter value: %d\n", shared_counter);

	return 0;
}
