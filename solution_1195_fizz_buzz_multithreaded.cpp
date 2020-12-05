class FizzBuzz {
private:
    int n;
    int idx;
    int flag;
    mutex mut;
    condition_variable cond;
    void setFlag() {
        if (idx % 3 == 0 && idx % 5 == 0) flag = 3;
        else if (idx % 3 == 0) flag = 1;
        else if (idx % 5 == 0) flag = 2;
        else flag = 0;
    }
public:
    FizzBuzz(int n): flag(0), idx(1) {
        this->n = n;
    }

    // printFizz() outputs "fizz".
    void fizz(function<void()> printFizz) {
        while (idx <= n) {
            unique_lock<mutex> lk(mut);
            cond.wait(lk, [this]() { return flag == 1 || idx > n;});
            if (idx  > n) break;
            printFizz();
            idx++;
            setFlag();
            lk.unlock();
            cond.notify_all();
        }
    }

    // printBuzz() outputs "buzz".
    void buzz(function<void()> printBuzz) {
        while (idx <= n) {
            unique_lock<mutex> lk(mut);
            cond.wait(lk, [this]() { return flag == 2 || idx > n;});
            if (idx  > n) break;
            printBuzz();
            idx++;
            setFlag();
            lk.unlock();
            cond.notify_all();
        }
    }

    // printFizzBuzz() outputs "fizzbuzz".
    void fizzbuzz(function<void()> printFizzBuzz) {
        while (idx <= n) {
            unique_lock<mutex> lk(mut);
            cond.wait(lk, [this]() { return flag == 3 || idx > n;});
            if (idx  > n) break;
            printFizzBuzz();
            idx++;
            setFlag();
            lk.unlock();
            cond.notify_all();
        }
    }

    // printNumber(x) outputs "x", where x is an integer.
    void number(function<void(int)> printNumber) {
        while (idx <= n) {
            unique_lock<mutex> lk(mut);
            cond.wait(lk, [this]() { return flag == 0 || idx > n;});
            if (idx  > n) break;
            printNumber(idx++);
            setFlag();
            lk.unlock();
            cond.notify_all();
        }
    }
};
