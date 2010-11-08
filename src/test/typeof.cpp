#include <iostream>

using namespace std;

class Base {
    public:
        virtual bool me();
};

class Der : public Base {
    public:
        virtual bool me();
};

bool Base::me(){
    return true;
}

bool Der::me(){
    return false;
}

int main(){
    Base b;
    Der d;

    Der* pd = (Der*)&d;
    Base* pb = (Base*)&d;
    typeof(*pd) pd2 = pd;
    cout << pd2->me() << endl;
    return 0;
}
