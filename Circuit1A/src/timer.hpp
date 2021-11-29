#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
    unsigned long previousUs;
    unsigned long previousMs;
    unsigned int deltaUs;
    unsigned int deltaMs;
    unsigned int size;
    unsigned int i;
    // Arrays used for statistical calcs
    unsigned int *statsDeltaUs;
    unsigned int *statsDeltaMs;

public:
    Timer();
    Timer(unsigned int statsResolution);
    ~Timer();

    void tick();
    const unsigned int &tickUs();
    const unsigned int &tickMs();
    const unsigned int &getDeltaUs();
    const unsigned int &getDeltaMs();
    unsigned int getRateUs();
    unsigned int getRateMs();
};

#endif //TIMER_HPP
