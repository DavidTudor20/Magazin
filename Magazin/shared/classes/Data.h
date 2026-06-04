#ifndef DATA_H
#define DATA_H

#include <iostream>

class Data {
private:
    int zi, luna, an;

public:
    Data(int z, int l, int a) : zi(z), luna(l), an(a) {}

    int getZi() const { return zi; }
    int getLuna() const { return luna; }
    int getAn() const { return an; }

    friend std::ostream& operator<<(std::ostream& os, const Data& d) {
        os << d.zi << "/" << d.luna << "/" << d.an;
        return os;
    }
};

#endif