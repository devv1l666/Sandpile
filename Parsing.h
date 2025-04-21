#pragma once

#include <string>

class AnalyzedArgs {
public:
    std::string input_filename_;
    std::string output_bmp_directory_path_;
    uint64_t max_iterations_ = -1;
    uint64_t frequency_ = 0;
    bool Parse(int argc, char* argv[]);
};

struct Dot {
    uint64_t value = 0;
    int16_t x = 0;
    int16_t y = 0;
};

const std::string kShortInputArg = "-i";
const std::string kLongInputArg = "--input";
const std::string kShortOutputArg = "-o";
const std::string kLongOutputArg = "--output";
const std::string kShortIterArg = "-m";
const std::string kLongIterArg = "--max-iter";
const std::string kShortFreqArg = "-f";
const std::string kLongFreqArg = "--freq";

bool IsNumber(const std::string& our_argument);

Dot DotParser(std::string line);