#include "Vec2.h"

template<typename T, typename C> void Vec2G<T, C>::boxAroundLine(C thickness, const Vec2G<T, C> &p1, const Vec2G<T, C> &p2, Vec2G<T, C> &t1, Vec2G<T, C> &t2, Vec2G<T, C> &t3, Vec2G<T, C> &t4) {
    if (std::abs(p1.y() - p2.y()) <= 0.0000001f) { //horizontal line
        t1.set(p1.x(), p1.y() - thickness);
        t2.set(p2.x(), p2.y() - thickness);
        t3.set(p1.x(), p1.y() + thickness);
        t4.set(p2.x(), p2.y() + thickness);
    } else {
        C m = (C)(p1.x() - p2.x()) / (p2.y() - p1.y());
        C s = thickness / std::sqrt((C)(1 + m * m));
        C ms = m * s;
        t1.set(p1.x() - s, p1.y() - ms);
        t2.set(p2.x() - s, p2.y() - ms);
        t3.set(p1.x() + s, p1.y() + ms);
        t4.set(p2.x() + s, p2.y() + ms);
    }
}

template<typename T, typename C> Vec2G<T, C> Vec2G<T, C>::toDistance(C dist) {
    if (dist == 0) {
        return Vec2G<T, C>(0, 0);
    }
    C r = dist / std::sqrt((C)(mX * mX + mY * mY));
    return Vec2G<T, C>(mX * r, mY * r);
}

template class Vec2G<float, float>;