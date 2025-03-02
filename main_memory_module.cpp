// File 5: main_memory_module.cpp
#include <systemc.h>
SC_MODULE(MainMemory_Module) {
    sc_in<bool> Port_CLK;
    SC_CTOR(MainMemory_Module) {
        cout << "[MainMemory] Module constructed." << endl;
    }
};
