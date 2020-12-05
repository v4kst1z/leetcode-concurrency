/*
class Foo {
public:
    Foo() {

    }

    void first(function<void()> printFirst) {
        printFirst();
        flag.store(1);
    }

    void second(function<void()> printSecond) {
        while(flag.load() != 1) { std::this_thread::yield(); }
        printSecond();
        flag.store(2);
    }

    void third(function<void()> printThird) {
        while(flag.load() != 2) { std::this_thread::yield(); }
        printThird();
    }

private:
    std::atomic<int> flag{0};
};
 */

class Foo {
public:
    Foo(): tv(0) {
    }

    void first(function<void()> printFirst) {
        unique_lock<mutex> lk(mut);
        cond.wait(lk, [this]() { return tv == 0;});
        printFirst();
        tv++;
        lk.unlock();
        cond.notify_all();
    }

    void second(function<void()> printSecond) {
        unique_lock<mutex> lk(mut);
        cond.wait(lk, [this]() { return tv == 1;});
        printSecond();
        tv++;
        lk.unlock();
        cond.notify_all();
    }

    void third(function<void()> printThird) {
        unique_lock<mutex> lk(mut);
        cond.wait(lk, [this]() { return tv == 2;});
        printThird();
    }

private:
    std::mutex mut;
    std::condition_variable cond;
    int tv;
};
