#include "lab3.h"
#include <iostream>
#include <string>
#include <windows.h>

unsigned int lab3_thread_graph_id() { return 8; }

const char* lab3_unsynchronized_threads() { return "bce"; }

const char* lab3_sequential_threads() { return "deg"; }

CRITICAL_SECTION g_coutLock;

//Семафоры
HANDLE semD, semE, semG;
HANDLE sem_g_done, sem_c_done, sem_m_done, sem_k_done;

//Потоки
HANDLE a_thread;
HANDLE b_thread;
HANDLE c_thread;
HANDLE d_thread;
HANDLE e_thread;
HANDLE f_thread;
HANDLE g_thread;
HANDLE h_thread;
HANDLE i_thread;

// Объявление функций (прототипы)
DWORD WINAPI thread_a(LPVOID);
DWORD WINAPI thread_b(LPVOID);
DWORD WINAPI thread_c(LPVOID);
DWORD WINAPI thread_e(LPVOID);
DWORD WINAPI thread_f(LPVOID);
DWORD WINAPI thread_g(LPVOID);
DWORD WINAPI thread_i(LPVOID);
DWORD WINAPI thread_h(LPVOID);

//Безопасное выведение
void print_safe(const char* str) {
    EnterCriticalSection(&g_coutLock);
    std::cout << str << std::flush;
    LeaveCriticalSection(&g_coutLock);
}

DWORD WINAPI thread_a(LPVOID) {
    for (int i = 0; i < 3; ++i) {
        print_safe("a");
        computation();
    }

    b_thread = CreateThread(NULL, 0, thread_b, NULL, 0, NULL);
    if (b_thread == NULL) {
        std::cerr << "CreateThread_b error: " << GetLastError() << std::endl;
        return 1;
    }

    c_thread = CreateThread(NULL, 0, thread_c, NULL, 0, NULL);
    if (c_thread == NULL) {
        std::cerr << "CreateThread_c error: " << GetLastError() << std::endl;
        return 1;
    }

    e_thread = CreateThread(NULL, 0, thread_e, NULL, 0, NULL);
    if (e_thread == NULL) {
        std::cerr << "CreateThread_e error: " << GetLastError() << std::endl;
        return 1;
    }
    //Ожидание потоков
    WaitForSingleObject(b_thread, INFINITE);
    WaitForSingleObject(c_thread, INFINITE);
    WaitForSingleObject(e_thread, INFINITE);
    return 0;
}

DWORD WINAPI thread_b(LPVOID) {
    for (int i = 0; i < 3; ++i) {
        print_safe("b");
        computation();
    }
    return 0;
}

DWORD WINAPI thread_c(LPVOID) {
    for (int i = 0; i < 3; ++i) {
        print_safe("c");
        computation();
    }
    return 0;
}

DWORD WINAPI thread_d(LPVOID) {
    for (int i = 0; i < 3; ++i) {
        WaitForSingleObject(semD, INFINITE);// Ждем сигнала от потока D
        print_safe("d");
        computation();
        ReleaseSemaphore(semE, 1, NULL); // Сигнализируем потоку E
    }
    return 0;
}

DWORD WINAPI thread_e(LPVOID) {
    for (int i = 0; i < 3; ++i) {
        print_safe("e");
        computation();
    }

    WaitForSingleObject(b_thread, INFINITE);
    WaitForSingleObject(c_thread, INFINITE);

    // создаем дочерние потоки
    d_thread = CreateThread(NULL, 0, thread_d, NULL, 0, NULL);
    if (d_thread == NULL) {
        std::cerr << "CreateThread_d error: " << GetLastError() << std::endl;
        return 1;
    }
    // создаем дочерние потоки
    g_thread = CreateThread(NULL, 0, thread_g, NULL, 0, NULL);
    if (g_thread == NULL) {
        std::cerr << "CreateThread_g error: " << GetLastError() << std::endl;
        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        WaitForSingleObject(semE, INFINITE);// Ждем сигнала от потока E
        print_safe("e");
        computation();
        ReleaseSemaphore(semG, 1, NULL); // Сигнализируем потоку G
    }

    //Ожидание потоков
    WaitForSingleObject(d_thread, INFINITE);
    WaitForSingleObject(g_thread, INFINITE);

    return 0;
}

DWORD WINAPI thread_f(LPVOID) {
    for (int i = 0; i < 3; ++i) {
        print_safe("f");
        computation();
    }
    return 0;
}

DWORD WINAPI thread_g(LPVOID) {
    for (int i = 0; i < 3; ++i) {
        WaitForSingleObject(semG, INFINITE);// Ждем сигнала от потока G
        print_safe("g");
        computation();
        ReleaseSemaphore(semD, 1, NULL); // Сигнализируем потоку D
    }

    WaitForSingleObject(d_thread, INFINITE);

    // создаем дочерние потоки
    f_thread = CreateThread(NULL, 0, thread_f, NULL, 0, NULL);
    if (f_thread == NULL) {
        std::cerr << "CreateThread_d error: " << GetLastError() << std::endl;
        return 1;
    }
    // создаем дочерние потоки
    h_thread = CreateThread(NULL, 0, thread_h, NULL, 0, NULL);
    if (h_thread == NULL) {
        std::cerr << "CreateThread_g error: " << GetLastError() << std::endl;
        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        print_safe("g");
        computation();
    }

    WaitForSingleObject(f_thread, INFINITE);
    
    // создаем дочерние потоки
    i_thread = CreateThread(NULL, 0, thread_i, NULL, 0, NULL);
    if (i_thread == NULL) {
        std::cerr << "CreateThread_g error: " << GetLastError() << std::endl;
        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        print_safe("g");
        computation();
    }

    WaitForSingleObject(i_thread, INFINITE);
    WaitForSingleObject(h_thread, INFINITE);

    return 0;
}

DWORD WINAPI thread_i(LPVOID) {
    for (int i = 0; i < 3; ++i) {
        print_safe("i");
        computation();
    }
    return 0;
}

DWORD WINAPI thread_h(LPVOID) {
    for (int i = 0; i < 3; ++i) {
        print_safe("h");
        computation();
    }

    WaitForSingleObject(f_thread, INFINITE);

    for (int i = 0; i < 3; ++i) {
        print_safe("h");
        computation();
    }

    return 0;
}

void clean_up() {
    if (semD != NULL) {
        CloseHandle(semD);
    }
    if (semE != NULL) {
        CloseHandle(semE);
    }
    if (semG != NULL) {
        CloseHandle(semG);
    }

    DeleteCriticalSection(&g_coutLock);

    if (a_thread != NULL) {
        WaitForSingleObject(a_thread, INFINITE);
        CloseHandle(a_thread);
    }
    if (b_thread != NULL) {
        WaitForSingleObject(b_thread, INFINITE);
        CloseHandle(b_thread);
    }
    if (c_thread != NULL) {
        WaitForSingleObject(c_thread, INFINITE);
        CloseHandle(c_thread);
    }
    if (d_thread != NULL) {
        WaitForSingleObject(d_thread, INFINITE);
        CloseHandle(d_thread);
    }
    if (e_thread != NULL) {
        WaitForSingleObject(e_thread, INFINITE);
        CloseHandle(e_thread);
    }
    if (f_thread != NULL) {
        WaitForSingleObject(f_thread, INFINITE);
        CloseHandle(f_thread);
    }
    if (g_thread != NULL) {
        WaitForSingleObject(g_thread, INFINITE);
        CloseHandle(g_thread);
    }
    if (h_thread != NULL) {
        WaitForSingleObject(h_thread, INFINITE);
        CloseHandle(h_thread);
    }
    if (i_thread != NULL) {
        WaitForSingleObject(i_thread, INFINITE);
        CloseHandle(i_thread);
    }  
}

int lab3_init() {
    InitializeCriticalSection(&g_coutLock);

    semD = CreateSemaphore(NULL, 1, 1, NULL);
    if (semD == NULL) {
        std::cerr << "CreateSemaphore_D error: " << GetLastError() << std::endl;
        clean_up(); 
        return 1;
    }
    semE = CreateSemaphore(NULL, 0, 1, NULL);
    if (semE == NULL) {
        std::cerr << "CreateSemaphore_E error: " << GetLastError() << std::endl;
        clean_up();
        return 1;
    }
    semG = CreateSemaphore(NULL, 0, 1, NULL);
    if (semG == NULL) {
        std::cerr << "CreateSemaphore_G error: " << GetLastError() << std::endl;
        clean_up();
        return 1;
    }

    // Создаем поток а
    a_thread = CreateThread(NULL, 0, thread_a, NULL, 0, NULL);
    if (a_thread == NULL) {
        std::cerr << "CreateThread_a error: " << GetLastError() << std::endl;
        clean_up();
        return 1;
    }

    // ждем завершения потока
    WaitForSingleObject(a_thread, INFINITE);

    clean_up();
    std::cout << std::endl;
    return 0;
}