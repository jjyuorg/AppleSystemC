// File 2: cache_line.cpp
#include <cstdint>
#include <iostream>
struct CacheLine {
    uint64_t tag;
    bool valid;
    bool dirty;
    uint64_t last_access_time;
};

#include <systemc.h>
int sc_main(int argc, char* argv[]) {
    cout << "===============================\n";
    cout << " Starting Cache Line Test \n";
    cout << "===============================\n";
    CacheLine line;
    line.tag = 0xABC;
    line.valid = true;
    line.dirty = false;
    line.last_access_time = 42;
    cout << "[Test] CacheLine attributes: "
         << "Tag: " << hex << line.tag << ", Valid: " << line.valid
         << ", Dirty: " << line.dirty << ", Last Access: " << line.last_access_time << endl;
    cout << "[Test] CacheLine test complete." << endl;
    return 0;
}