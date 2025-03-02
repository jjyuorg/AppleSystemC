// File 1: memory_module.cpp - Implements a basic SystemC memory model

#include <systemc.h>

SC_MODULE(Memory) {
    SC_CTOR(Memory) {
        SC_METHOD(memory_operation);
        sensitive << clk.pos();
        dont_initialize();
        cout << "[Memory] Module constructed." << endl;
    }

    sc_in<bool> clk;
    sc_in<bool> read_enable;
    sc_in<bool> write_enable;
    sc_in<uint64_t> address;
    sc_in<uint64_t> write_data;
    sc_out<uint64_t> read_data;
    sc_out<bool> operation_done;

    uint64_t memory[1024]; // Simple 1KB memory storage

    void memory_operation() {
        cout << "[Memory] Clock tick detected. Checking for operations..." << endl;
        if (write_enable.read()) {
            memory[address.read() % 1024] = write_data.read();
            operation_done.write(true);
            cout << "[Memory] WRITE operation: Address=" << address.read()
                 << " Data=" << write_data.read() << endl;
        }
        else if (read_enable.read()) {
            read_data.write(memory[address.read() % 1024]);
            operation_done.write(true);
            cout << "[Memory] READ operation: Address=" << address.read()
                 << " Data=" << memory[address.read() % 1024] << endl;
        }
        else {
            operation_done.write(false);
            cout << "[Memory] No operation detected." << endl;
        }
    }
};

int sc_main(int argc, char* argv[]) {
    cout << "======================================\n";
    cout << " Starting Memory Module Full Debug Test \n";
    cout << "======================================\n";

    sc_signal<bool> clk_signal;
    sc_signal<bool> read_signal;
    sc_signal<bool> write_signal;
    sc_signal<uint64_t> address_signal;
    sc_signal<uint64_t> write_data_signal;
    sc_signal<uint64_t> read_data_signal;
    sc_signal<bool> operation_done_signal;

    Memory mem("MemoryTest");
    mem.clk(clk_signal);
    mem.read_enable(read_signal);
    mem.write_enable(write_signal);
    mem.address(address_signal);
    mem.write_data(write_data_signal);
    mem.read_data(read_data_signal);
    mem.operation_done(operation_done_signal);

    sc_start(0, SC_NS);
    cout << "[Test] Initialized memory module testbench." << endl;

    cout << "[Test] Writing value 42 to address 100." << endl;
    write_signal.write(true);
    read_signal.write(false);
    address_signal.write(100);
    write_data_signal.write(42);
    sc_start(1, SC_NS);

    cout << "[Test] Reading from address 100." << endl;
    write_signal.write(false);
    read_signal.write(true);
    address_signal.write(100);
    sc_start(1, SC_NS);

    cout << "[Test] Verifying read value: " << read_data_signal.read() << endl;
    if (read_data_signal.read() == 42) {
        cout << "[Test] SUCCESS: Read value matches expected result!" << endl;
    } else {
        cout << "[Test] FAILURE: Read value mismatch!" << endl;
    }

    cout << "[Test] Memory Module Test Completed." << endl;
    return 0;
}
