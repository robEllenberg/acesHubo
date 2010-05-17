#include "packet.hpp"

void packet::printme(){
    std::cout << "id: " << std::hex << (int) this->id << ", ";
    std::cout << "len: " << std::hex << (int) this->len << ", ";
    std::cout << "error: " << std::hex << (int) this->error << ", ";
    std::cout << "check: " << std::hex << (int) this->checksum << ", ";
    std::cout << "param: " << parameters << ", ";
    std::cout << std::endl;
}

packet::packet(){
    id = 0;
    len = 0;
    error = 0;
    checksum = 0;
    parameters = new std::list<unsigned char>;
}
