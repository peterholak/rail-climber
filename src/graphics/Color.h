#pragma once

#include <cinttypes>

class Color {
public:
    Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 255)
        : mRed(red / 255.0f), mGreen(green / 255.0f), mBlue(blue / 255.0f), mAlpha(alpha / 255.0f) { }
    Color(int red, int green, int blue, int alpha = 255)
        : Color(std::uint8_t(red), std::uint8_t(green), std::uint8_t(blue), std::uint8_t(alpha)) { }
    Color(float red, float green, float blue, float alpha = 1.0f)
        : mRed(red), mGreen(green), mBlue(blue), mAlpha(alpha) { }

    std::uint32_t rgba() const {
        return
            std::uint32_t(mRed * 255.0f) << 24 |
            std::uint32_t(mGreen * 255.0f) << 16 |
            std::uint32_t(mBlue * 255.0f) << 8 |
            std::uint32_t(mAlpha * 255.0f);
    }

    float red() const { return mRed; }
    float green() const { return mGreen; }
    float blue() const { return mBlue; }
    float alpha() const { return mAlpha; }

    bool isTransparent() const { return mAlpha == 0.0f; }

    bool operator==(const Color &other) const {
        return other.mRed == mRed && other.mGreen == mGreen && other.mBlue == mBlue && other.mAlpha == mAlpha;
    }

    bool operator!=(const Color &other) const {
        return !operator==(other);
    }

    static Color black;
    static Color white;
    static Color transparent;

private:
    float mRed;
    float mGreen;
    float mBlue;
    float mAlpha;
};
