// File 12: memory_latency_model.cpp - Simulates different memory access latencies

#include <systemc.h>
#include <iostream>

SC_MODULE(MemoryLatencyModel) {
    sc_in<bool> clk;
    sc_in<bool> read_enable;
    sc_in<bool> write_enable;
    sc_in<uint64_t> address;
    sc_in<uint64_t> write_data;
    sc_out<uint64_t> read_data;
    sc_out<bool> operation_done;

    const int READ_LATENCY = 5;
    const int WRITE_LATENCY = 3;
    uint64_t memory[1024]; // 1KB memory storage
    int latency_counter;
    bool in_operation;

    SC_CTOR(MemoryLatencyModel) {
        SC_METHOD(memory_operation);
        sensitive << clk.pos();
        dont_initialize();
        latency_counter = 0;
        in_operation = false;
        std::cout << "[MemoryLatencyModel] Module constructed." << std::endl;
    }

    void memory_operation() {
        if (in_operation) {
            latency_counter--;
            if (latency_counter == 0) {
                in_operation = false;
                operation_done.write(true);
                std::cout << "[MemoryLatencyModel] Operation complete." << std::endl;
            }
            return;
        }

        if (write_enable.read()) {
            latency_counter = WRITE_LATENCY;
            in_operation = true;
            memory[address.read() % 1024] = write_data.read();
            std::cout << "[MemoryLatencyModel] WRITE operation initiated: Address=" << address.read()
                      << " Data=" << write_data.read() << " Latency=" << WRITE_LATENCY << " cycles" << std::endl;
        } else if (read_enable.read()) {
            latency_counter = READ_LATENCY;
            in_operation = true;
            read_data.write(memory[address.read() % 1024]);
            std::cout << "[MemoryLatencyModel] READ operation initiated: Address=" << address.read()
                      << " Latency=" << READ_LATENCY << " cycles" << std::endl;
        }
    }
};

int sc_main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << " Starting Memory Latency Model Full Debug Test \n";
    std::cout << "========================================\n";

    sc_signal<bool> clk_signal;
    sc_signal<bool> read_signal;
    sc_signal<bool> write_signal;
    sc_signal<uint64_t> address_signal;
    sc_signal<uint64_t> write_data_signal;
    sc_signal<uint64_t> read_data_signal;
    sc_signal<bool> operation_done_signal;

    MemoryLatencyModel memory_model("MemoryLatencyTest");
    memory_model.clk(clk_signal);
    memory_model.read_enable(read_signal);
    memory_model.write_enable(write_signal);
    memory_model.address(address_signal);
    memory_model.write_data(write_data_signal);
    memory_model.read_data(read_data_signal);
    memory_model.operation_done(operation_done_signal);

    sc_start(0, SC_NS);
    std::cout << "[Test] Initialized memory latency model testbench." << std::endl;

    std::cout << "[Test] Writing value 88 to address 500." << std::endl;
    write_signal.write(true);
    read_signal.write(false);
    address_signal.write(500);
    write_data_signal.write(88);
    sc_start(WRITE_LATENCY, SC_NS);
    write_signal.write(false);

    std::cout << "[Test] Reading from address 500." << std::endl;
    write_signal.write(false);
    read_signal.write(true);
    address_signal.write(500);
    sc_start(READ_LATENCY, SC_NS);
    read_signal.write(false);

    std::cout << "[Test] Verifying read value: " << read_data_signal.read() << std::endl;
    if (read_data_signal.read() == 88) {
        std::cout << "[Test] SUCCESS: Read value matches expected result!" << std::endl;
    } else {
        std::cout << "[Test] FAILURE: Read value mismatch!" << std::endl;
    }

    std::cout << "[Test] Memory Latency Model Test Completed Successfully." << std::endl;
    return 0;
}
