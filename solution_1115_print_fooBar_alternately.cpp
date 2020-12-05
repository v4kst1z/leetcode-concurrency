/*
class FooBar {
private:
    int n;
    atomic<bool> flag;
public:
    FooBar(int n) : flag(false) {
        this->n = n;
    }

    void foo(function<void()> printFoo) {

        for (int i = 0; i < n; i++) {
            while (flag) { std::this_thread::yield(); }
            printFoo();
            flag.store(true);
        }
    }

    void bar(function<void()> printBar) {

        for (int i = 0; i < n; i++) {
            while (!flag) { std::this_thread::yield(); }
            printBar();
            flag.store(false);
        }
    }
};
 */

class FooBar {
private:
    int n;
    mutex mut;
    bool flag;
    condition_variable cond;
public:
    FooBar(int n) : flag(false) {
        this->n = n;
    }

    void foo(function<void()> printFoo) {

        for (int i = 0; i < n; i++) {
            unique_lock<mutex> lk(mut);
            cond.wait(lk, [this]() { return flag == false; });
            printFoo();
            flag = true;
            lk.unlock();
            cond.notify_one();
        }
    }

    void bar(function<void()> printBar) {

        for (int i = 0; i < n; i++) {
            unique_lock<mutex> lk(mut);
            cond.wait(lk, [this]() { return flag == true; });
            printBar();
            flag = false;
            lk.unlock();
            cond.notify_one();
        }
    }
};