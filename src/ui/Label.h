#pragma once

#include "ui/Widget.h"
#include <string>
#include "graphics/Color.h"
#include "graphics/Texture.h"

class Font;
class BitmapRenderer;

class Label : public Widget {
public:

    enum RenderingStrategy {
        SingleLine, MultiLine
    };

    Label(
        const std::string &text,
        const Vec2& position,
        const Font *font,
        Color color = Color::black,
        Alignment alignment = BottomLeft,
        const Vec2& size = Vec2(Widget::SIZE_AUTO, Widget::SIZE_AUTO),
        RenderingStrategy rendering = SingleLine
    );
    virtual ~Label();

    void draw(const BitmapRenderer &bitmapRenderer);
    void reload();
    void unload();

    void setText(const std::string &newText);
    void setColor(const Color &newColor);

protected:
    virtual Vec2 calculateSize() const;

private:
    void updateCachedSize();

    Texture mTextTexture;
    std::string mText;
    Vec2 mCachedActualSize;
    const Font *mFont;
    Color mColor;
};
