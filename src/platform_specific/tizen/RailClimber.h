#ifndef _RAILCLIMBER_H_
#define _RAILCLIMBER_H_

#include <inttypes.h>
#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FGraphicsOpengl2.h>
#include <FGrpGlPlayer.h>

using Tizen::Graphics::Point;
using Tizen::Ui::TouchEventInfo;
using Tizen::Ui::Control;

class GameFrame;
class GameRenderer;
class GameForm;
class Game;
class Painter;
class Sound;

class RailClimber : public Tizen::App::UiApp,
    public Tizen::System::IScreenEventListener/*,
    public Tizen::Base::Runtime::ITimerEventListener*/ {
public:
    static Tizen::App::UiApp* CreateInstance(void);

public:
    RailClimber();
    ~RailClimber();

public:
    bool OnAppInitializing(Tizen::App::AppRegistry& appRegistry);
    bool OnAppInitialized();
    bool OnAppWillTerminate(void);
    bool OnAppTerminating(Tizen::App::AppRegistry& appRegistry, bool forcedTermination = false);
    void OnForeground(void);
    void OnBackground(void);
    void OnScreenOn (void);
    void OnScreenOff (void);
    void OnLowMemory(void);
    void OnKeyPressed(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode);

    void OnTimerExpired(Tizen::Base::Runtime::Timer &timer);

    void OnTouchPressed(const Control& control, const Point& point, const TouchEventInfo &touchInfo);
    void OnTouchReleased(const Control& control, const Point& point, const TouchEventInfo &touchInfo);
    void OnTouchMoved(const Control& control, const Point& point, const TouchEventInfo &touchInfo);

    void OnVolumeChanged(int dir);

    void Draw();
    void Update();

    //Tizen::Base::Runtime::Timer *mVibrateWorkaroundTimer;

    bool mTerminating;

private:
    void InitOpenGL();
    void CleanupOpenGL();

    Tizen::Graphics::Opengl::GlPlayer *mGlPlayer;
    GameFrame *mFrame;
    GameRenderer *mRenderer;
    //GameForm *mForm;
    Game *mGame;
    Painter *mPainter;
    Sound *mSound;
    int mFrames;
    bool mWasPartial;
    long long mTicks;
    //Tizen::Base::Runtime::Timer *mGameTimer;
    /*Tizen::Graphics::Opengl::EGLDisplay mEglDisplay;
    Tizen::Graphics::Opengl::EGLConfig mEglConfig;
    Tizen::Graphics::Opengl::EGLSurface mEglSurface;
    Tizen::Graphics::Opengl::EGLContext mEglContext;*/
};

#endif
