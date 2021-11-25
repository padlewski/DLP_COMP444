#ifndef WRITER_HPP
#define WRITER_HPP

class printer {
public:
    virtual void print() = 0;
};

class writer {
    unsigned int timeLapsed;
    unsigned int interval;
    printer &_printer;

public:
    writer(unsigned int interval, printer&);
    ~writer() = default;
    void update(const unsigned int);
};

#endif // WRITER_HPP
