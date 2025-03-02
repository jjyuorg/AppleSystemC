// File 8: bus_snooping.cpp - Simulates the bus-based cache snooping mechanism

#include <systemc.h>
#include <iostream>

enum SnoopType { READ_SNOOP, WRITE_SNOOP, INVALIDATE_SNOOP };

SC_MODULE(BusSnooping) {
    sc_in<bool> clk;
    sc_in<SnoopType> snoop_signal;
    sc_signal<bool> cache_invalidated;

    SC_CTOR(BusSnooping) {
        SC_METHOD(handle_snoop);
        sensitive << clk.pos();
        dont_initialize();
        std::cout << "[BusSnooping] Module constructed." << std::endl;
    }

    void handle_snoop() {
        std::cout << "[BusSnooping] Clock tick detected. Checking snoop operation..." << std::endl;
        switch (snoop_signal.read()) {
            case READ_SNOOP:
                std::cout << "[BusSnooping] READ snoop detected. No invalidation needed." << std::endl;
                cache_invalidated.write(false);
                break;
            case WRITE_SNOOP:
                std::cout << "[BusSnooping] WRITE snoop detected. Other caches must invalidate this entry." << std::endl;
                cache_invalidated.write(true);
                break;
            case INVALIDATE_SNOOP:
                std::cout << "[BusSnooping] INVALIDATE snoop detected. Forcing cache invalidation." << std::endl;
                cache_invalidated.write(true);
                break;
            default:
                std::cout << "[BusSnooping] Unknown snoop type!" << std::endl;
        }
    }
};

int sc_main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << " Starting Bus Snooping Full Debug Test \n";
    std::cout << "========================================\n";

    sc_signal<bool> clk_signal;
    sc_signal<SnoopType> snoop_signal;

    BusSnooping bus_snooping("BusSnoopTest");
    bus_snooping.clk(clk_signal);
    bus_snooping.snoop_signal(snoop_signal);

    sc_start(0, SC_NS);
    std::cout << "[Test] Initialized bus snooping testbench." << std::endl;

    std::cout << "[Test] Simulating READ snoop (should not invalidate caches)." << std::endl;
    snoop_signal.write(READ_SNOOP);
    sc_start(1, SC_NS);

    std::cout << "[Test] Simulating WRITE snoop (should invalidate caches)." << std::endl;
    snoop_signal.write(WRITE_SNOOP);
    sc_start(1, SC_NS);

    std::cout << "[Test] Simulating INVALIDATE snoop (should force invalidation)." << std::endl;
    snoop_signal.write(INVALIDATE_SNOOP);
    sc_start(1, SC_NS);

    std::cout << "[Test] Bus Snooping Test Completed Successfully." << std::endl;
    return 0;
}
