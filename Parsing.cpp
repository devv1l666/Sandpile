#include "Parsing.h"

#include <iostream>
#include <ostream>
#include <sstream>

bool IsNumber(const std::string& our_argument) {
    size_t size = our_argument.size();
    for (int64_t i = 0; i < size; ++i) {
        if (!std::isdigit(our_argument[i])) {
            return false;
        }
    }
    return true;
}

bool AnalyzedArgs::Parse(int argc, char *argv[]) {
    std::string current_arg;
    std::string next_arg;
    for (size_t i = 1; i < argc; ++i) {
        current_arg = argv[i];
        if (current_arg == kShortInputArg || current_arg == kLongInputArg) {
            if (argc - i <= 1) {
                return false;
            }
            next_arg = argv[i + 1];
            input_filename_ = std::string(next_arg);
            ++i;
        } else if (current_arg == kShortOutputArg || current_arg == kLongOutputArg) {
            if (argc - i <= 1) {
                return false;
            }
            next_arg = argv[i + 1];
            output_bmp_directory_path_ = std::string(next_arg);
            ++i;
        } else if (current_arg == kShortIterArg || current_arg == kLongIterArg) {
            if (argc - i <= 1) {
                return false;
            }
            next_arg = argv[i + 1];
            if (!IsNumber(next_arg)) {
                return false;
            }
            max_iterations_ = stoll(next_arg);
            ++i;
        } else if (current_arg == kShortFreqArg || current_arg == kLongFreqArg) {
            if (argc - i <= 1) {
               return false;
            }
            next_arg = argv[i + 1];
            if (!IsNumber(next_arg)) {
                return false;
            }
            frequency_ = stoll(next_arg);
            ++i;
        } else {
            return false;
        }
    }
    return true;
}


Dot DotParser(const std::string line) {
    Dot dot{};
    std::istringstream is(line);
    std::string s;
    std::getline(is, s, '\t');
    dot.x = std::stol(s);
    std::getline(is, s, '\t');
    dot.y = std::stol(s);
    std::getline(is, s, '\n');
    dot.value = std::stoull(s);

    return dot;
}