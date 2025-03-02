// File 7: snoop_controller.cpp - Implements snooping logic for cache coherence

#include <systemc.h>
#include <iostream>

enum SnoopType { READ_SNOOP, WRITE_SNOOP, INVALIDATE_SNOOP };

SC_MODULE(SnoopController) {
    sc_in<bool> clk;
    sc_in<SnoopType> snoop_signal;
    sc_signal<bool> cache_hit;

    SC_CTOR(SnoopController) {
        SC_METHOD(handle_snoop);
        sensitive << clk.pos();
        dont_initialize();
        std::cout << "[SnoopController] Module constructed." << std::endl;
    }

    void handle_snoop() {
        std::cout << "[SnoopController] Clock tick detected. Processing snoop..." << std::endl;
        switch (snoop_signal.read()) {
            case READ_SNOOP:
                std::cout << "[SnoopController] Processing READ snoop." << std::endl;
                cache_hit.write(true);
                break;
            case WRITE_SNOOP:
                std::cout << "[SnoopController] Processing WRITE snoop." << std::endl;
                cache_hit.write(false);
                break;
            case INVALIDATE_SNOOP:
                std::cout << "[SnoopController] Processing INVALIDATE snoop. Invalidating cache line." << std::endl;
                cache_hit.write(false);
                break;
            default:
                std::cout << "[SnoopController] Unknown snoop type!" << std::endl;
        }
    }
};

int sc_main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << " Starting Snoop Controller Full Debug Test \n";
    std::cout << "========================================\n";

    sc_signal<bool> clk_signal;
    sc_signal<SnoopType> snoop_signal;

    SnoopController snoop_controller("SnoopTest");
    snoop_controller.clk(clk_signal);
    snoop_controller.snoop_signal(snoop_signal);

    sc_start(0, SC_NS);
    std::cout << "[Test] Initialized snoop controller testbench." << std::endl;

    std::cout << "[Test] Simulating READ snoop." << std::endl;
    snoop_signal.write(READ_SNOOP);
    sc_start(1, SC_NS);

    std::cout << "[Test] Simulating WRITE snoop." << std::endl;
    snoop_signal.write(WRITE_SNOOP);
    sc_start(1, SC_NS);

    std::cout << "[Test] Simulating INVALIDATE snoop." << std::endl;
    snoop_signal.write(INVALIDATE_SNOOP);
    sc_start(1, SC_NS);

    std::cout << "[Test] Snoop Controller Test Completed Successfully." << std::endl;
    return 0;
}
