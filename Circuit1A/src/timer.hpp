#ifndef TIMER_HPP
#define TIMER_HPP

class timer {
    unsigned long previousUs;
    unsigned long previousMs;
    unsigned int deltaUs;
    unsigned int deltaMs;
    unsigned int size;
    unsigned int i;
    unsigned int *statsDeltaUs;
    unsigned int *statsDeltaMs;

public:
    timer();
    ~timer();

    void tick();
    const unsigned int &tickUs();
    const unsigned int &tickMs();
    const unsigned int &getDeltaUs();
    const unsigned int &getDeltaMs();
    unsigned int getRateUs();
    unsigned int getRateMs();
};

#endif //TIMER_HPP
