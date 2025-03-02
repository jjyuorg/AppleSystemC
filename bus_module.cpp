// File 3: bus_module.cpp - Implements a shared bus for inter-processor communication

#include <systemc.h>
#include <iostream>

SC_MODULE(Bus) {
    sc_in<bool> clk;
    sc_signal<int> bus_owner;
    sc_signal<bool> bus_idle;

    SC_CTOR(Bus) {
        bus_owner.write(-1);
        bus_idle.write(true);
        SC_METHOD(monitor_bus);
        sensitive << clk.pos();
        dont_initialize();
        std::cout << "[Bus] Module constructed." << std::endl;
    }

    void request_bus(int cpu_id) {
        std::cout << "[Bus] CPU " << cpu_id << " requesting bus." << std::endl;
        if (bus_idle.read()) {
            bus_owner.write(cpu_id);
            bus_idle.write(false);
            std::cout << "[Bus] Bus granted to CPU " << cpu_id << std::endl;
        } else {
            std::cout << "[Bus] Bus is currently in use by CPU " << bus_owner.read() << std::endl;
        }
    }

    void release_bus(int cpu_id) {
        if (bus_owner.read() == cpu_id) {
            std::cout << "[Bus] CPU " << cpu_id << " releasing bus." << std::endl;
            bus_owner.write(-1);
            bus_idle.write(true);
        }
    }

    void monitor_bus() {
        if (!bus_idle.read()) {
            std::cout << "[Bus] Bus is currently owned by CPU " << bus_owner.read() << std::endl;
        } else {
            std::cout << "[Bus] Bus is idle." << std::endl;
        }
    }
};

int sc_main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << " Starting Bus Module Full Debug Test \n";
    std::cout << "========================================\n";

    sc_signal<bool> clk_signal;
    Bus bus("BusTest");
    bus.clk(clk_signal);

    sc_start(0, SC_NS);
    std::cout << "[Test] Initialized bus module testbench." << std::endl;

    std::cout << "[Test] Simulating CPU 1 requesting the bus..." << std::endl;
    bus.request_bus(1);
    sc_start(1, SC_NS);

    std::cout << "[Test] Simulating CPU 2 requesting the bus (should be denied)..." << std::endl;
    bus.request_bus(2);
    sc_start(1, SC_NS);

    std::cout << "[Test] Simulating CPU 1 releasing the bus..." << std::endl;
    bus.release_bus(1);
    sc_start(1, SC_NS);

    std::cout << "[Test] Simulating CPU 2 requesting the bus again (should be granted)..." << std::endl;
    bus.request_bus(2);
    sc_start(1, SC_NS);

    std::cout << "[Test] Bus Module Test Completed Successfully." << std::endl;
    return 0;
}
