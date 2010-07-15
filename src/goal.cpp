#include "goal.hpp"

namespace ACES{

    Goal::Goal(int id, int m, void* d){
        propID = id;
        data = d;
        this->mode = m;
    }

}
