// File 1: memory_module.cpp
#include <systemc.h>
SC_MODULE(Memory) {
    SC_CTOR(Memory) {
        cout << "[Memory] Module constructed." << endl;
    }
};
