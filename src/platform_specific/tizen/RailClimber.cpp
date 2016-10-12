#include "RailClimber.h"
#include "GameRenderer.h"
#include "GameFrame.h"
#include "../../World.h"
#include "../../Game.h"
#include "../../graphics/Painter.h"
#include "../../Sound.h"
#include "../../Settings.h"
#include "../../Translation.h"
#include "../../Logging.h"
#include <sstream>
#include <string>
#include <FGraphics.h>
#include <FGrpIGlRenderer.h>
#include <FGrpGlPlayer.h>

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Runtime;
using namespace Tizen::System;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;
using namespace std;
using namespace RailClimberPlatform;

RailClimber::RailClimber() {
	mFrame = 0;
	mRenderer = 0;
	g_app = this;
	//g_vibrator = 0;
	mTerminating = false;
	mGame = 0;
	mPainter = 0;
	mGlPlayer = 0;
	mSound = 0;
	//mVibrateWorkaroundTimer = 0;
}

RailClimber::~RailClimber() {
}

UiApp* RailClimber::CreateInstance(void) {
	return new RailClimber();
}

bool RailClimber::OnAppInitializing(AppRegistry& appRegistry) {
	if (mTerminating)
		return false;
	return true;
}

bool RailClimber::OnAppInitialized() {
	if (mTerminating)
		return false;

	setlocale(LC_NUMERIC, "C");
	AppLog("RailClimber init");

	mFrame = new GameFrame(this);
	mFrame->Construct();
	mFrame->SetOrientation(Tizen::Ui::ORIENTATION_PORTRAIT);
	mFrame->SetName(L"RailClimber");
	this->AddFrame(*mFrame);

	int x, y, width, height;
	mFrame->GetBounds(x, y, width, height);

	Tr::initialize();

	Settings::load("railclimber.xml");
	Settings::loadSaved("RailClimber");

	mFrames = 0;
	SystemTime::GetTicks(mTicks);

	mPainter = Painter::getInstance();
	mPainter->initialize(width, height, 1.0);

	//g_vibrator = new Tizen::System::Vibrator();
	//g_vibrator->Construct();

	mGame = Game::getInstance();
	mGame->initialize(width, height);
	mGame->paused = false;

	bool silent = false;
	Tizen::System::SettingInfo::GetValue("http://tizen.org/setting/sound.silent_mode", silent);
	Sound::getInstance()->isSilent = silent;
	if (silent) {
		//Sound::getInstance()->stopSilence();
	}

	//mVibrateWorkaroundTimer = new Timer();
	//mVibrateWorkaroundTimer->Construct(*this);

	/*mGameTimer = new Timer();
	mGameTimer->Construct(*this);*/
	//mGameTimer->Start(16); //aim for 60 fps

	/*if (!silent && Sound::getInstance()->enabled)
		RailClimberPlatform::loadSilence();*/
	mWasPartial = false;

	return true;
}

bool RailClimber::OnAppWillTerminate() {
	Log::debug("Will terminate");
	//mGameTimer->Cancel();
	//delete mGameTimer;
	return true;
}

bool RailClimber::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination) {
	if (mTerminating)
		return true;

	Log::debug("OnAppTerminating");
	mTerminating = true;

	if (mGlPlayer)
		mGlPlayer->Stop();

	delete mRenderer;
	delete mGlPlayer;
	mGlPlayer = 0;
	mRenderer = 0;

	//delete mVibrateWorkaroundTimer;

	/*mGame->cleanup();
	mPainter->cleanup();*/
	mGame = 0;
	mPainter = 0;

	Settings::unloadSaved();
	//delete g_vibrator;
	return true;
}

void RailClimber::InitOpenGL() {
/*	AppLog("Initializing OpenGL");
	eglBindAPI(EGL_OPENGL_ES_API);
	mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(mEglDisplay, null, null);

	EGLint numConfigs = 1;
	EGLint eglConfigList[] = {
		EGL_RED_SIZE,	5,
		EGL_GREEN_SIZE,	6,
		EGL_BLUE_SIZE,	5,
		EGL_ALPHA_SIZE,	0,
		EGL_DEPTH_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};
	EGLint eglContextList[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	eglChooseConfig(mEglDisplay, eglConfigList, &mEglConfig, 1, &numConfigs);
	eglSwapInterval(mEglDisplay, 1);
	mEglSurface = eglCreateWindowSurface(mEglDisplay, mEglConfig, (EGLNativeWindowType)mForm, null);
	mEglContext = eglCreateContext(mEglDisplay, mEglConfig, EGL_NO_CONTEXT, eglContextList);
	eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext);*/
}

void RailClimber::CleanupOpenGL() {
	Log::debug("CleanupOpenGL");
	/*eglMakeCurrent(mEglDisplay, null, null, null);
	eglDestroyContext(mEglDisplay, mEglContext);
	eglDestroySurface(mEglDisplay, mEglSurface);
	eglTerminate(mEglDisplay);*/
}

void RailClimber::Draw() {
	//mGlPlayer->Redraw();
}

void RailClimber::Update() {
	if (mTerminating)
		return;

	long long ticks;
	SystemTime::GetTicks(ticks);
	if (ticks - mTicks >= 1000 || mGame->getWorld()->secondsUpdated) {
		bool showFps = Settings::getBool("Graphics/ShowFPS");
		mGame->getWorld()->secondsUpdated = false;
		if (!mGame->inMenu && !mGame->paused)
			mPainter->updateTimeTextures( /*( ((float)mFrames) / ((ticks - mTicks)/1000) )*/0, mGame->getWorld()->levelTimer, showFps);
		mTicks = ticks;
		mFrames = 0;
	}
	mGame->updateGame(20);
	//mFrames++;
}

void RailClimber::OnTouchPressed(const Control& control, const Point& point, const TouchEventInfo &touchInfo) {
	if (mTerminating)
		return;
	if (GetAppUiState() == APP_UI_STATE_FOREGROUND) {
		mGame->handleTouchPress(point.x, point.y);
	}
}

void RailClimber::OnTouchReleased(const Control& control, const Point& point, const TouchEventInfo &touchInfo) {
	if (mTerminating)
		return;
	if (GetAppUiState() == APP_UI_STATE_FOREGROUND) {
		mGame->handleTouchRelease(point.x, point.y);
	}
}

void RailClimber::OnTouchMoved(const Control& control, const Point& point, const TouchEventInfo &touchInfo) {
	if (mTerminating)
		return;
	if (GetAppUiState() == APP_UI_STATE_FOREGROUND) {
		mGame->handleTouchMove(point.x, point.y);
	}
}

void RailClimber::OnTimerExpired(Tizen::Base::Runtime::Timer &timer) {
	/*if (!mGame->paused) {
		timer.Start(16);
		Update();
		Draw();
		mFrames++;
	}*/
	//g_vibrator->Stop();
}

void RailClimber::OnForeground(void) {
	if (mTerminating)
		return;

	Log::debug("OnForeground");
	if (!mWasPartial) {
		mWasPartial = true;
		if (mGame)
		{
			//Sound::getInstance()->initialize();
			mGame->initSound();
			mGlPlayer = new Tizen::Graphics::Opengl::GlPlayer();
			mGlPlayer->Construct(Tizen::Graphics::Opengl::EGL_CONTEXT_CLIENT_VERSION_2_X, mFrame);
			mGlPlayer->SetEglAttributePreset(Tizen::Graphics::Opengl::EGL_ATTRIBUTES_PRESET_DONT_CARE);
			mGlPlayer->SetFps(60);
		}
		mRenderer = new GameRenderer(this);
		mGlPlayer->SetIGlRenderer(mRenderer);
		int x, y, width, height;
		mFrame->GetBounds(x, y, width, height);
		mRenderer->SetTargetControlWidth(width);
		mRenderer->SetTargetControlHeight(height);
		mGlPlayer->Start();
		//mGlPlayer->Pause();

		mPainter->createGraphics();
		mGame->createGUI();

		bool silent = false;
		Tizen::System::SettingInfo::GetValue("http://tizen.org/setting/sound.silent_mode", silent);
		Sound::getInstance()->isSilent = silent;

		mGame->pauseGame(false);

		//mGlPlayer->Resume();
		//mGameTimer->Start(16);

		/*if (!silent && Sound::getInstance()->enabled)
			RailClimberPlatform::playSilence();*/
	}
}

void RailClimber::OnBackground(void) {
	if (mTerminating)
		return;

	Log::debug("OnBackground");
	if (GetAppUiState() == APP_UI_STATE_PARTIAL_BACKGROUND) {
		mWasPartial = true;
	}else{
		mGame->pauseGame(true);
		if (mGlPlayer) {
			mGlPlayer->Pause();
			mGame->destroyGUI();
			mPainter->destroyGraphics();
			mGlPlayer->Stop();
			delete mRenderer;
			delete mGlPlayer;
			mGlPlayer = 0;
			mRenderer = 0;
			Sound::getInstance()->cleanup();
		}

		mWasPartial = false;
		//mGameTimer->Cancel();
	}
}

void RailClimber::OnScreenOn (void) {
/*	Log("OnScreenOn");
	if (GetAppUiState() == APP_UI_STATE_FOREGROUND) {
		bool silent = false;
		Tizen::System::SettingInfo::GetValue("SilentMode", silent);
		Sound::getInstance()->isSilent = silent;

		mGame->pauseGame(false);
		//mGameTimer->Start(16);

		//if (!silent && Sound::getInstance()->enabled)
			//RailClimberPlatform::playSilence();
	}*/
}

void RailClimber::OnScreenOff (void) {
	//Log("OnScreenOff");
	//mGameTimer->Cancel();
	//mGame->pauseGame(true);
	//RailClimberPlatform::stopSilence();
}

void RailClimber::OnVolumeChanged(int dir) {
	/*RailClimberPlatform::setVolume(RailClimberPlatform::getVolume() + 5*dir);
	mGame->showVolumeIndicator();*/
}

void RailClimber::OnLowMemory() {
	Log::debug("OnLowMemory");
}

void RailClimber::OnKeyPressed(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode) {
	if (keyCode == KEY_BACK) {
		if (mGame)
			mGame->handleBackAction();
	}else if (keyCode == KEY_MENU) {
		if (mGame)
			mGame->handleMenuAction();
	}
}
