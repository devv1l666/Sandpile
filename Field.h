#pragma once

#include "Parsing.h"

#include <cstdint>

#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t bfType_;
    uint32_t bfSize_;
    uint16_t bfReserved1_;
    uint16_t bfReserved2_;
    uint32_t bfOffBits_;
};

struct BMPInfoHeader {
    uint32_t biSize_;
    int32_t biWidth_;
    int32_t biHeight_;
    uint16_t biPlanes_;
    uint16_t biBitCount_;
    uint32_t biCompression_;
    uint32_t biSizeImage_;
    int32_t biXPelsPerMeter_;
    int32_t biYPelsPerMeter_;
    uint32_t biClrUsed_;
    uint32_t biClrImportant_;
};

#pragma pack(pop)

class Field {
public:
    Field(uint16_t width, uint16_t height)
        : width_(width)
        , height_(height)
        , x_offset_(0)
        , y_offset_(0)
        , data_(new uint64_t[width * height]) {}

    ~Field() {
        delete [] data_;
    }

    Field(const Field& other)
        : width_(other.width_)
        , height_(other.height_)
        , x_offset_(other.x_offset_)
        , y_offset_(other.y_offset_)
        , data_(new uint64_t[other.width_ * other.height_]) {
        std::copy(other.data_, other.data_ + width_ * height_, data_);
    }

    Field& operator=(const Field& other) {
        if (this != &other) {
            delete[] data_;
            width_ = other.width_;
            height_ = other.height_;
            x_offset_ = other.x_offset_;
            y_offset_ = other.y_offset_;
            data_ = new uint64_t[width_ * height_];
            std::copy(other.data_, other.data_ + width_ * height_, data_);
        }
        return *this;
    }

    Field(Field&& other) noexcept
        : width_(other.width_)
        , height_(other.height_)
        , x_offset_(other.x_offset_)
        , y_offset_(other.y_offset_)
        , data_(other.data_) {
        other.data_ = nullptr;
    }

    Field& operator=(Field&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            width_ = other.width_;
            height_ = other.height_;
            x_offset_ = other.x_offset_;
            y_offset_ = other.y_offset_;
            data_ = other.data_;
            other.data_ = nullptr;
        }
        return *this;
    }

    void SaveBMP(const std::string& filename) const;
    bool ReadFile(const AnalyzedArgs& args);
    void SandFallManager(const AnalyzedArgs& args);

private:
    uint16_t width_;
    uint16_t height_;
    uint16_t x_offset_;
    uint16_t y_offset_;
    uint64_t* data_;

    void UpExtension(uint16_t shift);
    void DownExtension(uint16_t shift);
    void LeftExtension(uint16_t shift);
    void RightExtension(uint16_t shift);
    bool FallIteration();
    void Add(int16_t x, int16_t y, uint64_t value);
};
