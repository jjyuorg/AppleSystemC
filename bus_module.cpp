// File 3: bus_module.cpp
#include <systemc.h>
SC_MODULE(Bus) {
    sc_in<bool> Port_CLK;
    sc_signal<int> busOwner;
    SC_CTOR(Bus) {
        busOwner.write(-1);
        cout << "[Bus] Module constructed." << endl;
    }
};

int sc_main(int argc, char* argv[]) {
    cout << "===============================\n";
    cout << " Starting Bus Module Test \n";
    cout << "===============================\n";
    Bus bus("BusTest");
    cout << "[Test] Bus module instantiated successfully." << endl;
    cout << "[Test] Bus module test complete." << endl;
    return 0;
}