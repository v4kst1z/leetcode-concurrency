/*
class ZeroEvenOdd {
private:
    int n;
    atomic<int> flag;
    int idx;
    condition_variable cond;
public:
    ZeroEvenOdd(int n): flag(0), idx(1) {
        this->n = n;
    }

    // printNumber(x) outputs "x", where x is an integer.
    void zero(function<void(int)> printNumber) {
        for(int i = 0; i < n; i++) {
            while (flag.load() != 0 && flag.load() != 2) { std::this_thread::yield(); }
            printNumber(0);
            flag.store(flag.load() + 1);
        }
    }

    void odd(function<void(int)> printNumber) {
        for(int i = 1; i <= n; i = i+2) {
            while (flag.load() != 1) { std::this_thread::yield(); }
            printNumber(idx++);
            flag.store(flag.load() + 1);
        }
    }

    void even(function<void(int)> printNumber) {
        for(int i = 2; i <= n; i = i+2) {
            while (flag.load() != 3) { std::this_thread::yield(); }
            printNumber(idx++);
            flag.store(0);
        }
    }
};
 */


class ZeroEvenOdd {
private:
    int n;
    mutex mut;
    int flag;
    int idx;
    condition_variable cond;
public:
    ZeroEvenOdd(int n): flag(0), idx(1) {
        this->n = n;
    }

    // printNumber(x) outputs "x", where x is an integer.
    void zero(function<void(int)> printNumber) {
        for(int i = 0; i < n; i++) {
            unique_lock<mutex> lk(mut);
            cond.wait(lk, [this]() { return flag == 0 || flag == 2;});
            printNumber(0);
            flag += 1;
            lk.unlock();
            cond.notify_all();
        }
    }

    void odd(function<void(int)> printNumber) {
        for(int i = 1; i <= n; i = i+2) {
            unique_lock<mutex> lk(mut);
            cond.wait(lk, [this]() { return flag == 1;});
            printNumber(idx);
            idx++;
            flag = 2;
            lk.unlock();
            cond.notify_all();
        }
    }

    void even(function<void(int)> printNumber) {
        for(int i = 2; i <= n; i = i+2) {
            unique_lock<mutex> lk(mut);
            cond.wait(lk, [this]() { return flag == 3;});
            printNumber(idx);
            idx++;
            flag = 0;
            lk.unlock();
            cond.notify_all();
        }
    }
};