#include "lab2.h"
#include <cstring>
#include <semaphore.h>
#define NUMBER_OF_THREADS 10

// thread identifiers
pthread_t tid[NUMBER_OF_THREADS];
// critical section lock
pthread_mutex_t lock;

// semaphores for sequential threads
sem_t semD, semE, semF, semG, semBC;

unsigned int lab2_thread_graph_id()
{
    return 12;
}

const char* lab2_unsynchronized_threads()
{
    return "ghi";
}

const char* lab2_sequential_threads()
{
    return "defg";
}

// Функции потоков
void *thread_a(void *ptr);
void *thread_b(void *ptr);
void *thread_c(void *ptr);
void *thread_d(void *ptr);
void *thread_e(void *ptr);
void *thread_f(void *ptr);
void *thread_g(void *ptr);
void *thread_i(void *ptr);
void *thread_h(void *ptr);
void *thread_k(void *ptr);

void *thread_a(void *ptr)
{
    for (int i = 0; i < 3; ++i)
    {
        pthread_mutex_lock(&lock);
        std::cout << "a" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }

    int err = pthread_create(&tid[1], NULL, thread_b, NULL);
    if (err != 0)
        std::cerr << "Не удается создать поток. Ошибка: " << strerror(err) << std::endl;

    err = pthread_create(&tid[2], NULL, thread_c, NULL);
    if (err != 0)
        std::cerr << "Не удается создать поток. Ошибка: " << strerror(err) << std::endl;

    // Ждём завершения обоих потоков
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);

    return ptr;
}

void *thread_b(void *ptr)
{
    for (int i = 0; i < 3; ++i)
    {
        pthread_mutex_lock(&lock);
        std::cout << "b" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }

    int err = pthread_create(&tid[4], NULL, thread_e, NULL);
    if (err != 0)
        std::cerr << "Не удаётся создать поток. Ошибка: " << strerror(err) << std::endl;

    err = pthread_create(&tid[5], NULL, thread_f, NULL);
    if (err != 0)
        std::cerr << "Не удаётся создать поток. Ошибка: " << strerror(err) << std::endl;

    err = pthread_create(&tid[6], NULL, thread_g, NULL);
    if (err != 0)
        std::cerr << "Не удаётся создать поток. Ошибка: " << strerror(err) << std::endl;

    // Сигнализируем о завершении потока b
    sem_post(&semBC);

    // Ждём завершения потоков e, f и g
    pthread_join(tid[4], NULL);
    pthread_join(tid[5], NULL);
    pthread_join(tid[6], NULL);

    return ptr;
}

void *thread_c(void *ptr)
{
    for (int i = 0; i < 3; ++i)
    {
        pthread_mutex_lock(&lock);
        std::cout << "c" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }

    // Сигнализируем о завершении потока c
    sem_post(&semBC);

    // Ждём завершения потоков b и c
    sem_wait(&semBC);
    sem_wait(&semBC);

    // Создаём поток d
    int err = pthread_create(&tid[3], NULL, thread_d, NULL);
    if (err != 0)
        std::cerr << "Не удаётся создать поток. Ошибка: " << strerror(err) << std::endl;

    return ptr;
}

void *thread_d(void *ptr)
{
    for (int i = 0; i < 3; ++i)
    {
        sem_wait(&semD);
        pthread_mutex_lock(&lock);
        std::cout << "d" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&semE);
    }

    return ptr;
}

void *thread_e(void *ptr)
{
    for (int i = 0; i < 3; ++i)
    {
        sem_wait(&semE);
        pthread_mutex_lock(&lock);
        std::cout << "e" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&semF);
    }

    return ptr;
}

void *thread_f(void *ptr)
{
    for (int i = 0; i < 3; ++i)
    {
        sem_wait(&semF);
        pthread_mutex_lock(&lock);
        std::cout << "f" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&semG);
    }

    return ptr;
}

void *thread_g(void *ptr)
{
    for (int i = 0; i < 3; ++i)
    {
        sem_wait(&semG);
        pthread_mutex_lock(&lock);
        std::cout << "g" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&semD);
    }

    int err = pthread_create(&tid[7], NULL, thread_i, NULL);
    if (err != 0)
        std::cerr << "Не удаётся создать поток. Ошибка: " << strerror(err) << std::endl;

    err = pthread_create(&tid[8], NULL, thread_h, NULL);
    if (err != 0)
        std::cerr << "Не удаётся создать поток. Ошибка: " << strerror(err) << std::endl;

    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "g" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
        // Ждём завершения потоков I и H
    pthread_join(tid[7], NULL);
    pthread_join(tid[8], NULL);

    err = pthread_create(&tid[9], NULL, thread_k, NULL);
    if (err != 0)
        std::cerr << "Не удаётся создать поток. Ошибка: " << strerror(err) << std::endl;

    // Ждём завершения потока K
    pthread_join(tid[9], NULL);

    return ptr;
}

void *thread_i(void *ptr)
{
    for (int i = 0; i < 3; ++i)
    {
        pthread_mutex_lock(&lock);
        std::cout << "i" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }

    return ptr;
}

void *thread_h(void *ptr)
{
    for (int i = 0; i < 3; ++i)
    {
        pthread_mutex_lock(&lock);
        std::cout << "h" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }

    return ptr;
}

void *thread_k(void *ptr)
{
    pthread_mutex_lock(&lock);
    for (int i = 0; i < 3; ++i)
    {
        std::cout << "k" << std::flush;
        computation();
    }
    pthread_mutex_unlock(&lock);

    return ptr;
}

int lab2_init()
{
    // инициализация мьютекса
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        std::cerr << "Ошибка инициализации мьютекса" << std::endl;
        return 1;
    }

    // инициализация семафоров
    if (sem_init(&semD, 0, 1) != 0)
    {
        std::cerr << "Ошибка инициализации семафора #1" << std::endl;
        return 1;
    }
    if (sem_init(&semE, 0, 0) != 0)
    {
        std::cerr << "Ошибка инициализации семафора #2" << std::endl;
        return 1;
    }
    if (sem_init(&semF, 0, 0) != 0)
    {
        std::cerr << "Ошибка инициализации семафора #3" << std::endl;
        return 1;
    }
    if (sem_init(&semG, 0, 0) != 0)
    {
        std::cerr << "Ошибка инициализации семафора #4" << std::endl;
        return 1;
    }
    if (sem_init(&semBC, 0, 0) != 0)
    {
        std::cerr << "Ошибка инициализации семафора semBC" << std::endl;
        return 1;
    }

    //    // Запускаем основной поток
    int err = pthread_create(&tid[0], NULL, thread_a, NULL);
    if (err != 0)
    {
        std::cerr << "Не удалось создать поток. Ошибка: " << strerror(err) << std::endl;
        return 1;
    }

    // Ожидаем завершения основного потока
    pthread_join(tid[0], NULL);

    // Освобождаем ресурсы
    pthread_mutex_destroy(&lock);
    sem_destroy(&semD);
    sem_destroy(&semE);
    sem_destroy(&semF);
    sem_destroy(&semG);
    sem_destroy(&semBC);

    std::cout << std::endl;

    // Завершаем программу успешно
    return 0;
}
