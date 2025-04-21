#include "Field.h"

#include <iostream>
#include <fstream>
#include <sstream>

void Field::UpExtension(uint16_t shift) {
    auto* array = new uint64_t[width_ * (height_ + shift)]{};
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            array[y * width_ + x] = data_[y * width_ + x];
        }
    }
    delete [] data_;
    data_ = array;
    height_ += shift;
}

void Field::RightExtension(uint16_t shift) {
    auto* array = new uint64_t[(width_ + shift) * height_]{};
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            array[y * (width_ + shift) + x] = data_[y * width_ + x];
        }
    }
    delete [] data_;
    data_ = array;
    width_ += shift;
}

void Field::DownExtension(uint16_t shift) {
    auto* array = new uint64_t[width_ * (height_ + shift)]{};
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            array[(y + shift) * width_ + x] = data_[y * width_ + x];
        }
    }
    delete [] data_;
    data_ = array;
    height_ += shift;
    y_offset_ += shift;
}

void Field::LeftExtension(uint16_t shift) {
    auto* array = new uint64_t[(width_ + shift) * height_]{};
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            array[y * (width_ + shift) + x + shift] = data_[y * width_ + x];
        }
    }
    delete [] data_;
    data_ = array;
    width_ += shift;
    x_offset_ += shift;
}

void Field::Add(int16_t x, int16_t y, uint64_t value) {
    if (x > 0 && width_ - x_offset_ <= x) {
        uint16_t shift = x - width_ + x_offset_ + 1;
        RightExtension(shift);
    } else if (x < -x_offset_) {
        uint16_t shift = -x;
        LeftExtension(shift);
    }
    if (y > 0 && height_ - y_offset_ <= y) {
        uint16_t shift = y - height_ + y_offset_ + 1;
        UpExtension(shift);
    } else if (y < -y_offset_) {
        uint16_t shift = -y;
        DownExtension(shift);
    }
    data_[(y + y_offset_) * width_ + x + x_offset_] += value;
}

bool Field::ReadFile(const AnalyzedArgs& args) {
    if (args.input_filename_.empty()) {
        return false;
    }
    std::ifstream file(args.input_filename_);
    if (!file.is_open()) {
        return false;
    }
    std::string line;
    while (getline(file, line)) {
        Dot dot = DotParser(line);
        Add(dot.x, dot.y, dot.value);
    }

    return true;
}

void Field::SandFallManager(const AnalyzedArgs& args) {
    size_t j = 0;
    size_t k = 1;
    if (args.max_iterations_ == -1) {
        while (!FallIteration()) {
            if (args.frequency_ != 0) {
                ++j;
                if (j == args.frequency_) {
                    j = 0;
                    SaveBMP(args.output_bmp_directory_path_ + "temp" + std::to_string(k) + ".bmp");
                    ++k;
                }
            }
        }
    } else {
        for (size_t i = 0; i < args.max_iterations_; ++i) {
            if (FallIteration()) {
                break;
            }
            if (args.frequency_ != 0) {
                ++j;
                if (j == args.frequency_) {
                    j = 0;
                    SaveBMP(args.output_bmp_directory_path_ + "temp" + std::to_string(k) + ".bmp");
                    ++k;
                }
            }
        }
    }
}



bool Field::FallIteration() {
    bool done_flag = true;
    Field new_field(width_, height_);
    for (size_t y = 0; y < height_; ++y) {
        for (size_t x = 0; x < width_; ++x) {
            new_field.data_[y * width_ + x] = 0;
        }
    }
    for (int16_t y = 0; y < height_; ++y) {
        for (int16_t x = 0; x < width_; ++x) {
            if (data_[y * width_ + x] >= 4) {
                done_flag = false;
                new_field.Add(x, y, data_[y * width_ + x] - 4);
                new_field.Add(x, y + 1, 1);
                new_field.Add(x + 1, y, 1);
                new_field.Add(x - 1, y, 1);
                new_field.Add(x, y - 1, 1);
            } else {
                new_field.Add(x, y, data_[y * width_ + x]);
            }
        }
    }
    width_ = new_field.width_;
    height_ = new_field.height_;
    x_offset_ += new_field.x_offset_;
    y_offset_ += new_field.y_offset_;

    uint64_t* temp = data_;
    data_ = new_field.data_;
    new_field.data_ = temp;

    return done_flag;
}

void Field::SaveBMP(const std::string &filename) const {
    uint32_t size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + width_ * height_ + sizeof(uint8_t[20]);
    BMPFileHeader fileHeader{};
    fileHeader.bfType_ = 0x4D42;
    fileHeader.bfOffBits_ = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(uint8_t[20]);
    fileHeader.bfSize_ = fileHeader.bfOffBits_ + size;
    fileHeader.bfReserved1_ = 0;
    fileHeader.bfReserved2_ = 0;

    BMPInfoHeader infoHeader{};
    infoHeader.biSize_ = sizeof(BMPInfoHeader);
    infoHeader.biWidth_ = width_;
    infoHeader.biHeight_ = height_;
    infoHeader.biPlanes_ = 1;
    infoHeader.biBitCount_ = 4;
    infoHeader.biCompression_ = 0;
    infoHeader.biSizeImage_ = 0;
    infoHeader.biXPelsPerMeter_ = 0;
    infoHeader.biYPelsPerMeter_ = 0;
    infoHeader.biClrUsed_ = 5;
    infoHeader.biClrImportant_ = 0;

    uint8_t palette[] = {
        0xFF, 0xFF, 0xFF, 0x00, // White
        0x00, 0xFF, 0x00, 0x00, // Green
        0xFF, 0x00, 0xFF, 0x00, // Purple
        0x00, 0xFF, 0xFF, 0x00, // Yellow
        0x00, 0x00, 0x00, 0x00  // Black
    };

    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    file.write(reinterpret_cast<char*>(&palette), sizeof(palette));

    int paddingSize = 4 - width_ / 2 % 4;
    for (int32_t y = 0; y < height_; ++y) {
        uint8_t buffer[width_ / 2];
        for (int32_t i = 0; i < width_ / 2; ++i) {
            buffer[i] = 0;
        }
        for (int32_t x = 0; x < width_; x++) {
            uint8_t encodedColor = data_[y * width_ + x];
            if (encodedColor >= 4) {
                encodedColor = 4;
            }
            buffer[x / 2] |= encodedColor << 4 * ((x + 1) % 2);
        }
        file.write(reinterpret_cast<char*>(buffer), width_ / 2 + paddingSize);
    }

    file.close();
}
