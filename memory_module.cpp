// File 1: memory_module.cpp
#include <systemc.h>
SC_MODULE(Memory) {
    SC_CTOR(Memory) {
        cout << "[Memory] Module constructed." << endl;
    }
};

int sc_main(int argc, char* argv[]) {
    cout << "===============================\n";
    cout << " Starting Memory Module Test \n";
    cout << "===============================\n";
    Memory mem("MemoryTest");
    cout << "[Test] Memory module instantiated successfully." << endl;
    cout << "[Test] Memory module test complete." << endl;
    return 0;
}