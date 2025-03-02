// File 5: main_memory_module.cpp
#include <systemc.h>
SC_MODULE(MainMemory_Module) {
    sc_in<bool> Port_CLK;
    SC_CTOR(MainMemory_Module) {
        cout << "[MainMemory] Module constructed." << endl;
    }
};

int sc_main(int argc, char* argv[]) {
    cout << "===============================\n";
    cout << " Starting Main Memory Module Test \n";
    cout << "===============================\n";
    MainMemory_Module mem("MainMemoryTest");
    cout << "[Test] Main Memory module instantiated successfully." << endl;
    cout << "[Test] Main Memory module test complete." << endl;
    return 0;
}