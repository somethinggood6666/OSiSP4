#include "spinlock.h"

void Spinlock::lock() noexcept {
    for (;;) {
        if (!lock_.exchange(true, std::memory_order_acquire)) {
            break;
        }
        while (lock_.load(std::memory_order_relaxed));
    }
};

bool Spinlock::tryLock() noexcept {
    return !lock_.load(std::memory_order_relaxed) &&
        !lock_.exchange(true, std::memory_order_acquire);
};

void Spinlock::unlock() noexcept {
    lock_.store(false, std::memory_order_release);
};

