#pragma once

#include <iostream>

struct Spinlock {
private:
    std::atomic<bool> lock_ = { false };

public:
    void lock() noexcept;
    bool tryLock() noexcept;
    void unlock() noexcept;
};