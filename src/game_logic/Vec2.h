#pragma once

#include <Box2D/Box2D.h>
#include <cmath>

template<typename T, typename C = double> class Vec2G {
public:
    Vec2G() : mX(0), mY(0) { }
    Vec2G(T _x, T _y) : mX(_x), mY(_y) { }

    Vec2G(const Vec2G<T, C>& other) {
        mX = other.mX;
        mY = other.mY;
    }

    Vec2G(const b2Vec2& bvec) {
        mX = bvec.x;
        mY = bvec.y;
    }

    T x() const {
        return mX;
    }

    T y() const {
        return mY;
    }

    C length() const {
        return std::sqrt((C)(mX * mX + mY * mY));
    }

    C angle() const {
        return std::atan2(mY, mX);
    }

    void set(T _x, T _y) {
        mX = _x;
        mY = _y;
    }

    Vec2G<T, C> perpendicular(bool cw) const {
        return (cw ? Vec2G<T, C>(mY, -mX) : Vec2G<T, C>(-mY, mX));
    }
    Vec2G<T, C> toDistance(C dist);
    static void boxAroundLine(C thickness, const Vec2G<T, C> &p1, const Vec2G<T, C> &p2, Vec2G<T, C> &t1, Vec2G<T, C> &t2, Vec2G<T, C> &t3, Vec2G<T, C> &t4);

    Vec2G<T, C> operator+(const Vec2G<T, C> &other) const {
        return Vec2G<T, C>(mX + other.mX, mY + other.mY);
    }

    Vec2G<T, C> operator-(const Vec2G<T, C> &other) const {
        return Vec2G<T, C>(mX - other.mX, mY - other.mY);
    }

    Vec2G<T, C> operator-() const {
        return Vec2G<T, C>(-mX, -mY);
    }

    Vec2G<T, C> operator*(float f) const {
        return Vec2G<T, C>(f*mX, f * mY);
    }

    Vec2G<T, C>& operator=(const Vec2G<T, C>& v) {
        mX = v.mX;
        mY = v.mY;
        return *this;
    }

    void operator=(const b2Vec2& v) {
        mX = v.x;
        mY = v.y;
    }

    bool operator==(const Vec2G<T, C>& v) const {
        return (mX == v.mX && mY == v.mY);
    }

    operator b2Vec2() {
        return b2Vec2(mX, mY);
    }

    Vec2G<T, C>& operator+=(const Vec2G<T, C> &other) {
        mX += other.mX;
        mY += other.mY;
        return *this;
    }

    Vec2G<T, C>& operator-=(const Vec2G<T, C> &other) {
        mX -= other.mX;
        mY -= other.mY;
        return *this;
    };

private:
    T mX, mY;
};

typedef Vec2G<float, float> Vec2;