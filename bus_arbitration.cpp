// File 11: bus_arbitration.cpp - Implements a bus arbitration mechanism for multiple CPUs

#include <systemc.h>
#include <iostream>
#include <vector>

const int NUM_CPUS = 4;

SC_MODULE(BusArbitration) {
    sc_in<bool> clk;
    sc_signal<int> bus_owner;
    sc_signal<bool> bus_available;
    std::vector<sc_signal<bool>> cpu_requests;

    SC_CTOR(BusArbitration) {
        bus_owner.write(-1);
        bus_available.write(true);
        cpu_requests.resize(NUM_CPUS);
        SC_METHOD(handle_bus_requests);
        sensitive << clk.pos();
        dont_initialize();
        std::cout << "[BusArbitration] Module constructed." << std::endl;
    }

    void handle_bus_requests() {
        std::cout << "[BusArbitration] Clock tick. Checking CPU requests..." << std::endl;
        if (bus_available.read()) {
            for (int i = 0; i < NUM_CPUS; i++) {
                if (cpu_requests[i].read()) {
                    bus_owner.write(i);
                    bus_available.write(false);
                    std::cout << "[BusArbitration] Bus granted to CPU " << i << std::endl;
                    return;
                }
            }
        } else {
            std::cout << "[BusArbitration] Bus is currently owned by CPU " << bus_owner.read() << std::endl;
        }
    }

    void release_bus(int cpu_id) {
        if (bus_owner.read() == cpu_id) {
            std::cout << "[BusArbitration] CPU " << cpu_id << " releasing the bus." << std::endl;
            bus_owner.write(-1);
            bus_available.write(true);
        }
    }
};

int sc_main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << " Starting Bus Arbitration Full Debug Test \n";
    std::cout << "========================================\n";

    sc_signal<bool> clk_signal;
    std::vector<sc_signal<bool>> cpu_requests(NUM_CPUS);

    BusArbitration bus_arbitration("BusArbitrationTest");
    bus_arbitration.clk(clk_signal);
    for (int i = 0; i < NUM_CPUS; i++) {
        bus_arbitration.cpu_requests[i].write(false);
    }

    sc_start(0, SC_NS);
    std::cout << "[Test] Initialized bus arbitration testbench." << std::endl;

    for (int i = 0; i < NUM_CPUS; i++) {
        std::cout << "[Test] CPU " << i << " requesting the bus." << std::endl;
        cpu_requests[i].write(true);
        bus_arbitration.cpu_requests[i] = cpu_requests[i];
        sc_start(1, SC_NS);
        cpu_requests[i].write(false);
        bus_arbitration.release_bus(i);
        sc_start(1, SC_NS);
    }

    std::cout << "[Test] Bus Arbitration Test Completed Successfully." << std::endl;
    return 0;
}
