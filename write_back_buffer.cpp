// File 9: write_back_buffer.cpp - Implements a write-back buffer for optimizing memory writes

#include <systemc.h>
#include <iostream>
#include <queue>

SC_MODULE(WriteBackBuffer) {
    sc_in<bool> clk;
    sc_in<bool> write_enable;
    sc_in<uint64_t> address;
    sc_in<uint64_t> data;
    sc_out<bool> buffer_full;
    sc_out<bool> memory_write;
    sc_signal<bool> is_flushing;

    std::queue<std::pair<uint64_t, uint64_t>> buffer;
    const size_t BUFFER_SIZE = 4;

    SC_CTOR(WriteBackBuffer) {
        SC_METHOD(write_to_buffer);
        sensitive << clk.pos();
        dont_initialize();
        SC_METHOD(flush_buffer);
        sensitive << clk.pos();
        dont_initialize();
        std::cout << "[WriteBackBuffer] Module constructed." << std::endl;
    }

    void write_to_buffer() {
        if (write_enable.read()) {
            if (buffer.size() < BUFFER_SIZE) {
                buffer.push({address.read(), data.read()});
                std::cout << "[WriteBackBuffer] Buffered write: Address=" << address.read()
                          << " Data=" << data.read() << std::endl;
                buffer_full.write(false);
            } else {
                std::cout << "[WriteBackBuffer] Buffer is full! Write stalled." << std::endl;
                buffer_full.write(true);
            }
        }
    }

    void flush_buffer() {
        if (!buffer.empty()) {
            std::pair<uint64_t, uint64_t> entry = buffer.front();
            buffer.pop();
            std::cout << "[WriteBackBuffer] Flushing write to memory: Address=" << entry.first
                      << " Data=" << entry.second << std::endl;
            memory_write.write(true);
        } else {
            memory_write.write(false);
        }
    }
};

int sc_main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << " Starting Write-Back Buffer Full Debug Test \n";
    std::cout << "========================================\n";

    sc_signal<bool> clk_signal;
    sc_signal<bool> write_enable_signal;
    sc_signal<uint64_t> address_signal;
    sc_signal<uint64_t> data_signal;
    sc_signal<bool> buffer_full_signal;
    sc_signal<bool> memory_write_signal;

    WriteBackBuffer wb_buffer("WriteBackTest");
    wb_buffer.clk(clk_signal);
    wb_buffer.write_enable(write_enable_signal);
    wb_buffer.address(address_signal);
    wb_buffer.data(data_signal);
    wb_buffer.buffer_full(buffer_full_signal);
    wb_buffer.memory_write(memory_write_signal);

    sc_start(0, SC_NS);
    std::cout << "[Test] Initialized write-back buffer testbench." << std::endl;

    for (int i = 0; i < 6; i++) {
        std::cout << "[Test] Writing to buffer: Address " << i * 10 << " Data " << i * 100 << std::endl;
        write_enable_signal.write(true);
        address_signal.write(i * 10);
        data_signal.write(i * 100);
        sc_start(1, SC_NS);
    }

    std::cout << "[Test] Flushing buffer contents to memory." << std::endl;
    for (int i = 0; i < 5; i++) {
        sc_start(1, SC_NS);
    }

    std::cout << "[Test] Write-Back Buffer Test Completed Successfully." << std::endl;
    return 0;
}
