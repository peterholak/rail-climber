#include "Widget.h"
#include "InvalidCoordinatesException.h"

Vec2 Widget::calculatePosition(Widget::Alignment alignment) const {
    if (mRequestedCoordinates.alignment == alignment) {
        return mRequestedCoordinates.position;
    }

    Vec2 baseBottomLeft = mRequestedCoordinates.position;
    switch (mRequestedCoordinates.alignment) {
        case TopCenter:
            baseBottomLeft -= Vec2(width() / 2, height());
            break;

        case TopLeft:
            baseBottomLeft -= Vec2(0, height());
            break;

        case Center:
            baseBottomLeft -= Vec2(width() / 2, height() / 2);
            break;

        case BottomCenter:
            baseBottomLeft -= Vec2(width() / 2, 0);
            break;

        case BottomLeft:
            break;
    }

    switch (alignment) {
        case TopCenter:
            return baseBottomLeft + Vec2(width() / 2, height());

        case TopLeft:
            return baseBottomLeft + Vec2(0, height());

        case Center:
            return baseBottomLeft + Vec2(width() / 2, height() / 2);

        case BottomCenter:
            return baseBottomLeft + Vec2(width() / 2, 0);

        case BottomLeft:
            return baseBottomLeft;
    }
    throw InvalidCoordinatesException("Invalid alignment");
}

Vec2 Widget::calculateSize() const {
    if (mRequestedCoordinates.size == Vec2(Widget::SIZE_AUTO, Widget::SIZE_AUTO)) {
        throw InvalidCoordinatesException("Width and height of a widget can not both be auto (unless overridden by a specific widget class)");
    }

    float actualWidth =
        mRequestedCoordinates.size.x() != Widget::SIZE_AUTO
        ? mRequestedCoordinates.size.x() : mRequestedCoordinates.size.y();

    float actualHeight =
        mRequestedCoordinates.size.y() != Widget::SIZE_AUTO
        ? mRequestedCoordinates.size.y() : mRequestedCoordinates.size.y();

    return Vec2(actualWidth, actualHeight);
}

Widget::Widget(const Vec2 &position, const Vec2 &size, Widget::Alignment alignment) {
    mRequestedCoordinates.position = position;
    mRequestedCoordinates.size = size;
    mRequestedCoordinates.alignment = alignment;
}
