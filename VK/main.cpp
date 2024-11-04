#include <iostream>
#include <vector>
#include <algorithm>
#include "TaskScheduler.h"

using namespace std;
// Пример использования TaskScheduler для сортировки массива
int main() {
    TaskScheduler scheduler;

    // Пример массива для сортировки
    vector<int> arrayToSort = {5, 3, 8, 1, 2};

    // Установка времени выполнения задачи на 5 секунд позже
    time_t executeTime = time(nullptr) + 5;

    // Добавление задачи на сортировку массива
    scheduler.Add([arrayToSort]() mutable {
        sort(arrayToSort.begin(), arrayToSort.end());
        cout << "Sorted array: ";
        for (int num : arrayToSort) {
            cout << num << " ";
        }
        cout << endl;
    }, executeTime);

    // Ожидание, чтобы задача могла выполниться
    this_thread::sleep_for(chrono::seconds(10));

    return 0;
}
