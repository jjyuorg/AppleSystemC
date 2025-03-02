// File 2: cache_line.cpp - Defines a CacheLine struct and tests cache attributes

#include <systemc.h>
#include <cstdint>
#include <iostream>

struct CacheLine {
    uint64_t tag;
    bool valid;
    bool dirty;
    uint64_t last_access_time;
};

int sc_main(int argc, char* argv[]) {
    cout << "===================================\n";
    cout << " Starting Cache Line Full Debug Test \n";
    cout << "===================================\n";

    CacheLine line1;
    line1.tag = 0xDEADBEEF;
    line1.valid = true;
    line1.dirty = false;
    line1.last_access_time = 100;

    CacheLine line2;
    line2.tag = 0xCAFEBABE;
    line2.valid = false;
    line2.dirty = true;
    line2.last_access_time = 200;

    cout << "[Test] Initialized two CacheLine instances." << endl;

    cout << "[Test] Checking CacheLine 1 attributes: " << endl;
    cout << "\tTag: " << hex << line1.tag << "\n"
         << "\tValid: " << line1.valid << "\n"
         << "\tDirty: " << line1.dirty << "\n"
         << "\tLast Access Time: " << line1.last_access_time << endl;

    cout << "[Test] Checking CacheLine 2 attributes: " << endl;
    cout << "\tTag: " << hex << line2.tag << "\n"
         << "\tValid: " << line2.valid << "\n"
         << "\tDirty: " << line2.dirty << "\n"
         << "\tLast Access Time: " << line2.last_access_time << endl;

    cout << "[Test] Modifying CacheLine 2 attributes..." << endl;
    line2.valid = true;
    line2.dirty = false;
    line2.last_access_time = 300;

    cout << "[Test] Rechecking CacheLine 2 attributes: " << endl;
    cout << "\tTag: " << hex << line2.tag << "\n"
         << "\tValid: " << line2.valid << "\n"
         << "\tDirty: " << line2.dirty << "\n"
         << "\tLast Access Time: " << line2.last_access_time << endl;

    cout << "[Test] Cache Line Test Completed Successfully." << endl;
    return 0;
}