#include <inttypes.h>
#include "GameFrame.h"
#include "RailClimber.h"

GameFrame::GameFrame(RailClimber *rc) : mApp(rc) {


}

GameFrame::~GameFrame() {

}

result
GameFrame::OnInitializing(void)
{
	result r = E_SUCCESS;

	AddTouchEventListener(*this);
	AddKeyEventListener(*this);

	return r;
}

result
GameFrame::OnTerminating(void)
{
	result r = E_SUCCESS;



	return r;
}

void GameFrame::OnTouchPressed(const Control& control, const Point& point, const TouchEventInfo &touchInfo) {
	if (mApp)
		mApp->OnTouchPressed(control, point, touchInfo);
}

void GameFrame::OnTouchReleased(const Control& control, const Point& point, const TouchEventInfo &touchInfo) {
	if (mApp)
		mApp->OnTouchReleased(control, point, touchInfo);
}
void GameFrame::OnTouchMoved(const Control& control, const Point& point, const TouchEventInfo &touchInfo) {
	if (mApp)
		mApp->OnTouchMoved(control, point, touchInfo);
}

void GameFrame::OnKeyPressed(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode) {
	if (mApp)
		mApp->OnKeyPressed(source, keyCode);
}
