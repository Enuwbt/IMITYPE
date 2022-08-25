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

int main(){
    using namespace imitype;
    
    void* handle = dlopen("libYOURNAME.so",RTLD_LAZY);

    initialize(handle);

    Optine* opt = new Optine;

    static IMTYPE<void, "Optine::print(int)"_> print = {opt, "_ZN6OptZine5printEi"};

    print(789); // -> Optine::print(789)

    delete opt;
    
    dlclose(handle);
}
