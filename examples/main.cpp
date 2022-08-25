#include <iostream>
#include "imitype.h"

class Optine {
public:
    void print();
    void print(int);
};

void Optine::print() {
    println("Optine::print()");
}

void Optine::print(int i) {
    println("Optine::print(%d)", i);
}

void printGlobal(){
    println("Global::printGlobal()");
}


int main(){
    using namespace imitype;
    
    void* handle = dlopen("libYOURNAME.so",RTLD_LAZY);

    initialize(handle);
   
    //=======================Call Class Member with symbol==========================//

    Optine* opt = new Optine;

    static IMTYPE<void, "Optine::print(int)"_> printi = {opt, "_ZN6OptZine5printEi"};

    printi(789); // -> Optine::print(789)
    
    //======================Call Global Function with address=======================//
    
    static IMTYPE<void, "GLOBAL::printG()"_> printG = {nullptr, &printGlobal};
    
    printG(); // -> Global::printGlobal()
    
    //=======================Call Class Member with address=========================//
    
    initialize(0x0b02000); <- loaded base
    
    static IMTYPE<void, "Optine::print()"_> print = {opt, 0x032dc008}; // <- set function addres
    
    print(); // -> Optine::print()
    
    //=============================================================================//

    delete opt;
    
    dlclose(handle);
}
