#ifndef WRITER_HPP
#define WRITER_HPP

class Printer {
public:
    virtual ~Printer() = default;
    virtual void print() { return; }
};

class Writer {
    unsigned int timeLapsed;
    unsigned int interval;
    Printer** printers;
    int size;

public:
    Writer(unsigned int interval, Printer** printers, int size);
    ~Writer() = default;
    void update(const unsigned int);
};

#endif // WRITER_HPP
