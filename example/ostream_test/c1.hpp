#include <ostream>

class c1 {
    public:
        c1(int a, int b);
        const c1* operator<<(int a);
        int m1;
        int m2;
};

std::ostream &operator<<(std::ostream &s, c1 n);
