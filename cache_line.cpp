// File 2: cache_line.cpp
struct CacheLine {
    uint64_t tag;
    bool valid;
    bool dirty;
    uint64_t last_access_time;
};

#include <systemc.h>
int sc_main(int argc, char* argv[]) {
    cout << "Testing Cache Line Structure" << endl;
    return 0;
}