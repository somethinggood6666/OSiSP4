#include "main.h"

using namespace concurrent;

const size_t BUFFER_SIZE = 256;
const size_t THREADS_COUNT = 16;

void sort(std::string** arr, int length, int offset, int count) 
{
    if (offset + count > length)
        count = length - offset;

    int end = offset + count;
    for (int i = offset; i < end - 1; i++)
        for (int j = i + 1; j < end; j++)
            if (arr[i]->compare(*arr[j]) > 0) {
                std::string* t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
}

int main()
{
    std::string path;
    std::cout << "Enter the path: ";
    std::cin >> path;

    std::ifstream fin;
    fin.open(path);

    std::vector<std::string*> list = std::vector<std::string*>();
    char buffer[BUFFER_SIZE];
    while (!fin.eof()) {
        fin.getline(buffer, BUFFER_SIZE);
        list.push_back(new std::string(buffer));
    }

    fin.close();

    size_t length = list.size();
    std::string** arr = new std::string*[length];
    int i = 0;
    while (!list.empty()) {
        arr[i++] = list.back();
        list.pop_back();
    }

    Dispatcher* dispatcher = new Dispatcher(THREADS_COUNT);

    const int blockSize = length / THREADS_COUNT + 1;
    PTask tasks[THREADS_COUNT];
    for (int i = 0; i < THREADS_COUNT; i++)
    {
        tasks[i] = dispatcher->beginInvoke([ arr, i, blockSize, length ]() {
            int offset = blockSize * i;
            sort(arr, length, offset, blockSize);
            });
    }
    
    for (int i = 0; i < THREADS_COUNT; i++)
        tasks[i]->wait();

    delete dispatcher;

    for (int i = 0; i < THREADS_COUNT; i++)
        delete tasks[i];

    mergeSort(arr, length, blockSize);

    std::ofstream fout;
    fout.open(path);

    for (int i = 0; i < length; i++)
    {
        fout << *arr[i] << std::endl;
    }

    fout.close();

    delete [] arr;
}

//using namespace std::chrono_literals;
//
//std::atomic<int> res = 0;
//
//void Operation() {
//    std::this_thread::sleep_for(0.01s);
//    res++;
//}
//
//int main()
//{
//    
//    while (true) {
//        Dispatcher* dispatcher = new Dispatcher(100);
//
//        PTask tasks[1000];
//        for (int i = 0; i < 1000; i++)
//            tasks[i] = dispatcher->beginInvoke(Operation);
//        
//        for (int i = 0; i < 1000; i++)
//            tasks[i]->wait();
//
//        std::cout << res << std::endl;
//
//        delete dispatcher;
//
//        for (int i = 0; i < 1000; i++)
//            delete tasks[i];
//
//        std::this_thread::sleep_for(1s);
//
//    }
//}

