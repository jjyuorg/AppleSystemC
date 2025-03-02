// File 5: main_memory_module.cpp - Implements a main memory module that interacts with the bus

#include <systemc.h>
#include <iostream>

SC_MODULE(MainMemory) {
    sc_in<bool> clk;
    sc_in<bool> read_enable;
    sc_in<bool> write_enable;
    sc_in<uint64_t> address;
    sc_in<uint64_t> write_data;
    sc_out<uint64_t> read_data;
    sc_out<bool> operation_done;

    uint64_t memory[1024]; // 1KB memory storage

    SC_CTOR(MainMemory) {
        SC_METHOD(memory_operation);
        sensitive << clk.pos();
        dont_initialize();
        std::cout << "[MainMemory] Module constructed." << std::endl;
    }

    void memory_operation() {
        std::cout << "[MainMemory] Clock tick detected. Checking for operations..." << std::endl;
        if (write_enable.read()) {
            memory[address.read() % 1024] = write_data.read();
            operation_done.write(true);
            std::cout << "[MainMemory] WRITE operation: Address=" << address.read()
                      << " Data=" << write_data.read() << std::endl;
        }
        else if (read_enable.read()) {
            read_data.write(memory[address.read() % 1024]);
            operation_done.write(true);
            std::cout << "[MainMemory] READ operation: Address=" << address.read()
                      << " Data=" << memory[address.read() % 1024] << std::endl;
        }
        else {
            operation_done.write(false);
            std::cout << "[MainMemory] No operation detected." << std::endl;
        }
    }
};

int sc_main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << " Starting Main Memory Module Full Debug Test \n";
    std::cout << "========================================\n";

    sc_signal<bool> clk_signal;
    sc_signal<bool> read_signal;
    sc_signal<bool> write_signal;
    sc_signal<uint64_t> address_signal;
    sc_signal<uint64_t> write_data_signal;
    sc_signal<uint64_t> read_data_signal;
    sc_signal<bool> operation_done_signal;

    MainMemory memory("MemoryTest");
    memory.clk(clk_signal);
    memory.read_enable(read_signal);
    memory.write_enable(write_signal);
    memory.address(address_signal);
    memory.write_data(write_data_signal);
    memory.read_data(read_data_signal);
    memory.operation_done(operation_done_signal);

    sc_start(0, SC_NS);
    std::cout << "[Test] Initialized main memory module testbench." << std::endl;

    std::cout << "[Test] Writing value 77 to address 300." << std::endl;
    write_signal.write(true);
    read_signal.write(false);
    address_signal.write(300);
    write_data_signal.write(77);
    sc_start(1, SC_NS);

    std::cout << "[Test] Reading from address 300." << std::endl;
    write_signal.write(false);
    read_signal.write(true);
    address_signal.write(300);
    sc_start(1, SC_NS);

    std::cout << "[Test] Verifying read value: " << read_data_signal.read() << std::endl;
    if (read_data_signal.read() == 77) {
        std::cout << "[Test] SUCCESS: Read value matches expected result!" << std::endl;
    } else {
        std::cout << "[Test] FAILURE: Read value mismatch!" << std::endl;
    }

    std::cout << "[Test] Main Memory Module Test Completed." << std::endl;
    return 0;
}
