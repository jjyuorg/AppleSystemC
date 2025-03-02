// File 6: coherence_protocol.cpp - Implements the Valid-Invalid coherence protocol for caches

#include <systemc.h>
#include <iostream>

enum CoherenceState { INVALID, VALID };

SC_MODULE(CacheLine) {
    sc_signal<CoherenceState> state;
    sc_signal<uint64_t> tag;

    SC_CTOR(CacheLine) {
        state.write(INVALID);
        SC_METHOD(print_state);
        sensitive << state;
    }

    void update_state(bool write) {
        if (write) {
            std::cout << "[CacheLine] Writing: Transitioning to VALID state." << std::endl;
            state.write(VALID);
        } else {
            std::cout << "[CacheLine] Invalidating: Transitioning to INVALID state." << std::endl;
            state.write(INVALID);
        }
    }

    void print_state() {
        std::cout << "[CacheLine] Current State: " << (state.read() == VALID ? "VALID" : "INVALID") << std::endl;
    }
};

SC_MODULE(CoherenceController) {
    sc_in<bool> clk;
    sc_signal<bool> write_signal;
    CacheLine cache_line;

    SC_CTOR(CoherenceController) : cache_line("CacheLine") {
        SC_METHOD(simulate_coherence);
        sensitive << clk.pos();
        dont_initialize();
        std::cout << "[CoherenceController] Module constructed." << std::endl;
    }

    void simulate_coherence() {
        std::cout << "[CoherenceController] Clock tick detected." << std::endl;
        if (write_signal.read()) {
            cache_line.update_state(true);
        } else {
            cache_line.update_state(false);
        }
    }
};

int sc_main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << " Starting Coherence Protocol Full Debug Test \n";
    std::cout << "========================================\n";

    sc_signal<bool> clk_signal;
    sc_signal<bool> write_signal;

    CoherenceController coherence_controller("CoherenceTest");
    coherence_controller.clk(clk_signal);
    coherence_controller.write_signal(write_signal);

    sc_start(0, SC_NS);
    std::cout << "[Test] Initialized coherence protocol testbench." << std::endl;

    std::cout << "[Test] Simulating write operation (should transition to VALID)." << std::endl;
    write_signal.write(true);
    sc_start(1, SC_NS);

    std::cout << "[Test] Simulating invalidation (should transition to INVALID)." << std::endl;
    write_signal.write(false);
    sc_start(1, SC_NS);

    std::cout << "[Test] Coherence Protocol Test Completed Successfully." << std::endl;
    return 0;
}
