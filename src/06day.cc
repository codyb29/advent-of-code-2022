#include <iostream>
#include <unordered_map>

#include "06day.h"

int TuningTrouble::GetStartOfPacket(std::string datastream_buffer, int start) {
    std::unordered_map<char, int> unique;
    int total_unique = 0;
    for (std::string::size_type i = 0; i < datastream_buffer.size(); i++) {
        if (i > 3) {
            unique[datastream_buffer[i - start]]--;
            if (unique[datastream_buffer[i - start]] == 0) {
                total_unique--;
            }
        }
        if (unique[datastream_buffer[i]] == 0) {
            total_unique++;
        }
        unique[datastream_buffer[i]]++;
        if (total_unique == start) {
            return i + 1;
        }
    }
    return 0;
}

