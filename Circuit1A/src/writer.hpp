#ifndef WRITER_HPP
#define WRITER_HPP

class Printer {
public:
    virtual void print() = 0;
};

class Writer {
    unsigned int timeLapsed;
    unsigned int interval;
    Printer &_printer;

public:
    Writer(unsigned int interval, Printer&);
    ~Writer() = default;
    void update(const unsigned int);
};

#endif // WRITER_HPP
