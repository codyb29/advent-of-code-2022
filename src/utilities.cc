#include <string>
#include <fstream>
#include "utilities.h"

std::vector<std::string> Utilities::ProcessInputFile(std::string filename) {
    std::vector<std::string> processed_strings;
    std::ifstream input_file(filename);
    std::string line;
    while (std::getline(input_file, line)) {
        processed_strings.push_back(line);
    }
    return processed_strings;

}
