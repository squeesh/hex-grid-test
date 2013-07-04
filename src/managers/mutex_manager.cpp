#include "../includes.h"


std::map< const char*, std::mutex*, cmp_str> MutexManager::mutex_map;


std::mutex* MutexManager::get(const char* key) {
    std::mutex* output = MutexManager::mutex_map[key];

    if(!output) {
        MutexManager::mutex_map[key] = new std::mutex();
        output = MutexManager::mutex_map[key];
    }

    return output;
}
