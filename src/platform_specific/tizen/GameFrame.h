#ifndef GAMEFRAME_H_
#define GAMEFRAME_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <FGraphicsOpengl2.h>

class RailClimber;

using Tizen::Graphics::Point;
using Tizen::Ui::TouchEventInfo;
using Tizen::Ui::Control;

class GameFrame : public Tizen::Ui::Controls::Frame,
    public Tizen::Ui::ITouchEventListener,
    public Tizen::Ui::IKeyEventListener {
public:
    GameFrame(RailClimber *rc);
    virtual ~GameFrame();

    virtual result OnInitializing(void);
    virtual result OnTerminating(void);

    void OnTouchPressed(const Control& control, const Point& point, const TouchEventInfo &touchInfo);
    void OnTouchReleased(const Control& control, const Point& point, const TouchEventInfo &touchInfo);
    void OnTouchMoved(const Control& control, const Point& point, const TouchEventInfo &touchInfo);
    void OnTouchLongPressed(const Control& control, const Point& point, const TouchEventInfo &touchInfo) { }
    void OnTouchDoublePressed(const Control& control, const Point& point, const TouchEventInfo &touchInfo) { }
    void OnTouchFocusIn(const Control& control, const Point& point, const TouchEventInfo &touchInfo) { }
    void OnTouchFocusOut(const Control& control, const Point& point, const TouchEventInfo &touchInfo) { }
    void OnKeyPressed(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode);
    void OnKeyReleased(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode) { }
    void OnKeyLongPressed(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode) { }


private:
    RailClimber *mApp;
};

#endif /* GAMEFRAME_H_ */
