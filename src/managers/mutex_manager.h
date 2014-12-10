#ifndef MUTEX_MANAGER_H
#define MUTEX_MANAGER_H


class MutexManager {
    private:
        static std::map< const char*, std::mutex*, cmp_str> mutex_map;

    public:
        static std::mutex* get(const char*);
};

#endif
