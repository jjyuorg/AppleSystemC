// File 4: cpu_module.cpp - Implements a simple CPU module that interacts with the cache

#include <systemc.h>
#include <iostream>

SC_MODULE(CPU_Module) {
    sc_in<bool> clk;
    sc_out<bool> cache_read;
    sc_out<bool> cache_write;
    sc_out<uint64_t> address;
    sc_out<uint64_t> write_data;
    sc_in<uint64_t> read_data;
    sc_in<bool> operation_done;

    SC_CTOR(CPU_Module) {
        SC_THREAD(run);
        sensitive << clk.pos();
        dont_initialize();
        std::cout << "[CPU] Module constructed." << std::endl;
    }

    void run() {
        wait(1, SC_NS);
        std::cout << "[CPU] Writing value 99 to address 200." << std::endl;
        cache_write.write(true);
        cache_read.write(false);
        address.write(200);
        write_data.write(99);
        wait(1, SC_NS);

        std::cout << "[CPU] Waiting for operation completion..." << std::endl;
        while (!operation_done.read()) wait(1, SC_NS);

        std::cout << "[CPU] Read request for address 200." << std::endl;
        cache_write.write(false);
        cache_read.write(true);
        address.write(200);
        wait(1, SC_NS);

        std::cout << "[CPU] Waiting for operation completion..." << std::endl;
        while (!operation_done.read()) wait(1, SC_NS);
        std::cout << "[CPU] Read value: " << read_data.read() << std::endl;

        if (read_data.read() == 99) {
            std::cout << "[CPU] SUCCESS: Data matches expected value!" << std::endl;
        } else {
            std::cout << "[CPU] FAILURE: Data mismatch!" << std::endl;
        }
    }
};

int sc_main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << " Starting CPU Module Full Debug Test \n";
    std::cout << "========================================\n";

    sc_signal<bool> clk_signal;
    sc_signal<bool> cache_read_signal;
    sc_signal<bool> cache_write_signal;
    sc_signal<uint64_t> address_signal;
    sc_signal<uint64_t> write_data_signal;
    sc_signal<uint64_t> read_data_signal;
    sc_signal<bool> operation_done_signal;

    CPU_Module cpu("CPUTest");
    cpu.clk(clk_signal);
    cpu.cache_read(cache_read_signal);
    cpu.cache_write(cache_write_signal);
    cpu.address(address_signal);
    cpu.write_data(write_data_signal);
    cpu.read_data(read_data_signal);
    cpu.operation_done(operation_done_signal);

    sc_start(0, SC_NS);
    std::cout << "[Test] Initialized CPU module testbench." << std::endl;
    sc_start(10, SC_NS);

    std::cout << "[Test] CPU Module Test Completed Successfully." << std::endl;
    return 0;
}