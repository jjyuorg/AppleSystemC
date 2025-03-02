// File 10: cache_eviction_policy.cpp - Implements Least Recently Used (LRU) for cache eviction

#include <systemc.h>
#include <iostream>
#include <vector>
#include <algorithm>

const int CACHE_SIZE = 4; // 4-way set associative cache

struct CacheLine {
    uint64_t tag;
    uint64_t last_access_time;
    bool valid;
};

SC_MODULE(CacheEviction) {
    sc_in<bool> clk;
    sc_in<bool> access;
    sc_in<uint64_t> address;
    sc_signal<uint64_t> time_counter;

    std::vector<CacheLine> cache;

    SC_CTOR(CacheEviction) {
        cache.resize(CACHE_SIZE, {0, 0, false});
        SC_METHOD(handle_cache_access);
        sensitive << clk.pos();
        dont_initialize();
        std::cout << "[CacheEviction] Module constructed." << std::endl;
    }

    void handle_cache_access() {
        if (!access.read()) return;

        uint64_t addr = address.read();
        auto it = std::find_if(cache.begin(), cache.end(), 
                               [&](const CacheLine& line) { return line.valid && line.tag == addr; });

        if (it != cache.end()) {
            std::cout << "[CacheEviction] Cache HIT: Address " << addr << std::endl;
            it->last_access_time = time_counter.read();
        } else {
            std::cout << "[CacheEviction] Cache MISS: Address " << addr << std::endl;
            evict_and_replace(addr);
        }
        time_counter.write(time_counter.read() + 1);
    }

    void evict_and_replace(uint64_t addr) {
        auto lru_it = std::min_element(cache.begin(), cache.end(), 
                                       [](const CacheLine& a, const CacheLine& b) {
                                           return a.last_access_time < b.last_access_time;
                                       });

        std::cout << "[CacheEviction] Evicting Address: " << lru_it->tag << " (LRU)" << std::endl;
        lru_it->tag = addr;
        lru_it->last_access_time = time_counter.read();
        lru_it->valid = true;
    }
};

int sc_main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << " Starting Cache Eviction Policy Full Debug Test \n";
    std::cout << "========================================\n";

    sc_signal<bool> clk_signal;
    sc_signal<bool> access_signal;
    sc_signal<uint64_t> address_signal;

    CacheEviction cache_eviction("CacheEvictionTest");
    cache_eviction.clk(clk_signal);
    cache_eviction.access(access_signal);
    cache_eviction.address(address_signal);

    sc_start(0, SC_NS);
    std::cout << "[Test] Initialized cache eviction testbench." << std::endl;

    std::vector<uint64_t> test_addresses = {10, 20, 30, 40, 50, 20, 10, 60};
    for (auto addr : test_addresses) {
        std::cout << "[Test] Accessing Address " << addr << std::endl;
        access_signal.write(true);
        address_signal.write(addr);
        sc_start(1, SC_NS);
    }

    std::cout << "[Test] Cache Eviction Policy Test Completed Successfully." << std::endl;
    return 0;
}
