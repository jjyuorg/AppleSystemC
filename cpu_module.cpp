// File 4: cpu_module.cpp
#include <systemc.h>
SC_MODULE(CPU_Module) {
    sc_in<bool> Port_CLK;
    SC_CTOR(CPU_Module) {
        cout << "[CPU] Module constructed." << endl;
    }
};

int sc_main(int argc, char* argv[]) {
    cout << "===============================\n";
    cout << " Starting CPU Module Test \n";
    cout << "===============================\n";
    CPU_Module cpu("CPUTest");
    cout << "[Test] CPU module instantiated successfully." << endl;
    cout << "[Test] CPU module test complete." << endl;
    return 0;
}