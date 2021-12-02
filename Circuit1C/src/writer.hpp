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
    Writer(unsigned int interval, Printer** printers, int size): 
    timeLapsed(0), interval(interval), printers(printers), size(size) {}
    ~Writer() = default;
    void update(const unsigned int time){
        timeLapsed += time;
        if(timeLapsed < interval) return;
        timeLapsed = 0;
        for(int i = 0 ; i < size ; ++i) printers[i]->print();
    }
};

#endif // WRITER_HPP
