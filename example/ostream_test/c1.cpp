#include "c1.hpp"
#include "c2.hpp"
#include <iostream>

c1::c1(int a, int b){
    m1 = a;
    m2 = b;
}

std::ostream &operator<<(std::ostream &s, c1 n){
    s << n.m1 << " " << n.m2 << std::endl;
    return s;
}

const c1* c1::operator<<(int a){
    std::cout << "A goddam " << a << std::endl;
    return this;
}

c2* c2::operator<<(c1 c){
    c << c.m1;
    c << c.m2;
} 

int main(){
    c1 a(5,4);
    c2 b;
    a << 6;
    b << a;
    std::cout << a;
    return 0;
}
