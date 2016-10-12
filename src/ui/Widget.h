#pragma once

#include <string>
#include <cstddef>
#include "graphics/BitmapRenderer.h"
#include "game_logic/Vec2.h"

class IWidgetHandler;
class Painter;

class Widget {
public:
    /**
     * Determines which part of the widget the provided x, y coordinates represent.
     */
    enum Alignment {
        BottomLeft, Center, BottomCenter, TopCenter, TopLeft
    };

    struct Coordinates {
        Vec2 position;
        Vec2 size = Vec2(Widget::SIZE_AUTO, Widget::SIZE_AUTO);
        Alignment alignment = BottomLeft;
    };

    Vec2 topLeft() const { return calculatePosition(TopLeft); }
    Vec2 bottomLeft() const { return calculatePosition(BottomLeft); }
    Vec2 bottomCenter() const { return calculatePosition(BottomCenter); }
    Vec2 center() const { return calculatePosition(Center); }
    Vec2 topCenter() const { return calculatePosition(TopCenter); }
    Vec2 position() const { return mRequestedCoordinates.position; }
    Alignment alignment() const { return mRequestedCoordinates.alignment; }
    Vec2 size() const { return calculateSize(); }

    virtual ~Widget() { };

    void setVisible(bool visible) { mVisible = visible; }
    bool isVisible() const { return mVisible; }

    float width() const { return size().x(); }
    float height() const { return size().y(); }

    const Coordinates& requestedCoordinates() const { return mRequestedCoordinates; }

    virtual void draw(const BitmapRenderer &bitmapRenderer) = 0;
    virtual void reload() = 0;
    virtual void unload() = 0;
    virtual void tick(bool debugManual = false) { }

    virtual void handleTouchPress(const Vec2 &position) { }
    virtual void handleTouchMove(const Vec2 &position) { }
    virtual void handleTouchRelease(const Vec2 &position) { }

    virtual bool wantsEventsOutsideOfBounds() const { return false; }
    virtual bool isPositionWithinBounds(const Vec2 &p) const {
        // TODO: lol performance
        return
            p.x() > bottomLeft().x() &&
            p.y() > bottomLeft().y() &&
            p.x() < bottomLeft().x() + width() &&
            p.y() < bottomLeft().y() + height();
    }

    static constexpr float SIZE_AUTO = BitmapRenderer::SIZE_AUTO;

protected:
    Widget() { }
    Widget(const Vec2 &position, const Vec2 &size, Alignment alignment);
    Coordinates mRequestedCoordinates;
    Vec2 calculatePosition(Alignment alignment) const;
    virtual Vec2 calculateSize() const;
    bool mVisible = true;
};
