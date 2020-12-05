class DiningPhilosophers {
private:
    mutex mut[5];
public:
    DiningPhilosophers() {

    }

    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork) {
        std::lock(mut[philosopher], mut[(philosopher + 1) % 5]);
        pickLeftFork();
        pickRightFork();
        eat();
        putLeftFork();
        putRightFork();
        mut[philosopher].unlock();
        mut[(philosopher + 1) % 5].unlock();
    }
};