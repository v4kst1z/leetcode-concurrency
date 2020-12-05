/*
class H2O {
public:
    H2O() {

    }

    void hydrogen(function<void()> releaseHydrogen) {
        while (num_h.load() == 0) { std::this_thread::yield(); }
        releaseHydrogen();
        num_h--;
        if (!num_h && !num_o) {
            num_h = 2;
            num_o = 1;
        }
    }

    void oxygen(function<void()> releaseOxygen) {
        while (num_o.load() == 0) { std::this_thread::yield(); }
        releaseOxygen();
        num_o--;
        if (!num_h && !num_o) {
            num_h = 2;
            num_o = 1;
        }
    }

private:
    atomic<int> num_h{2};
    atomic<int> num_o{1};
};
 */

class H2O {
public:
    H2O() {

    }

    void hydrogen(function<void()> releaseHydrogen) {
        unique_lock<mutex> lk(mut);
        cond.wait(lk, [this]() { return num_h > 0; });
        releaseHydrogen();
        num_h--;
        if (!num_h && !num_o) {
            num_h = 2;
            num_o = 1;
        }
        lk.unlock();
        cond.notify_all();
    }

    void oxygen(function<void()> releaseOxygen) {
        unique_lock<mutex> lk(mut);
        cond.wait(lk, [this]() { return num_o > 0; });
        releaseOxygen();
        num_o--;
        if (!num_h && !num_o) {
            num_h = 2;
            num_o = 1;
        }
        lk.unlock();
        cond.notify_all();
    }

private:
    int num_h{2};
    int num_o{1};
    mutex mut;
    condition_variable cond;
};
