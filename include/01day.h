#ifndef DAY01_H
#define DAY01_H



const std::vector<std::string> ConvertInputFileToVector(
    const std::string file_name);

bool ConvertStringToInt(const std::string string_to_convert, int& output);

int ElfWithMostCalories(const std::vector<std::string>& elf_calories);

int ElvesWithMostCalories(const std::vector<std::string>& elf_calories);

#endif
