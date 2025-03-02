// File 4: cpu_module.cpp
#include <systemc.h>
SC_MODULE(CPU_Module) {
    sc_in<bool> Port_CLK;
    SC_CTOR(CPU_Module) {
        cout << "[CPU] Module constructed." << endl;
    }
};

int sc_main(int argc, char* argv[]) {
    cout << "Testing CPU Module" << endl;
    return 0;
}
