#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

// Template cho Thread-Safe Queue
template<typename T>
class ThreadSafeQueue {
public:
    // Thêm dữ liệu vào queue
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
        condVar_.notify_one(); // Thông báo rằng có dữ liệu mới
    }

    // Lấy dữ liệu từ queue (BLOCKING nếu queue rỗng)
    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        condVar_.wait(lock, [this]() { return !queue_.empty(); }); // Chờ đến khi có dữ liệu
        T value = queue_.front();
        queue_.pop();
        return value;
    }

    // Kiểm tra queue có rỗng không
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

private:
    mutable std::mutex mutex_;               // Mutex để bảo vệ queue
    std::queue<T> queue_;                    // Queue chính
    std::condition_variable condVar_;        // Biến điều kiện để thông báo
};

#endif // THREAD_SAFE_QUEUE_H
