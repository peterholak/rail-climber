#include "GameRenderer.h"
#include "../../graphics/Painter.h"
#include "../../Game.h"
#include "../../Logging.h"
#include "RailClimber.h"

GameRenderer::GameRenderer(RailClimber *rc) : mApp(rc) {

}

GameRenderer::~GameRenderer() {

}

bool
GameRenderer::InitializeGl(void)
{
	if (mApp->mTerminating)
		return true;

	Log::debug("InitGL");
	glViewport(0, 0, GetTargetControlWidth(), GetTargetControlHeight());
	return true;
}

bool
GameRenderer::TerminateGl(void)
{
	Log::debug("TermGL");
	return true;
}

bool
GameRenderer::Draw(void)
{
	if (mApp->mTerminating)
		return true;

	mApp->Update();

	glClear(GL_COLOR_BUFFER_BIT);

	Game *game = Game::getInstance();
	Painter *painter = Painter::getInstance();

	painter->setScroll(game);
	painter->drawFrame(game);

	return true;
}

bool
GameRenderer::Pause(void)
{
	Log::debug("Paused");
	// TODO:
	// Do something necessary when Plyaer is paused.

	return true;
}

bool
GameRenderer::Resume(void)
{
	Log::debug("Resumed");
	// TODO:
	// Do something necessary when Plyaer is resumed.

	return true;
}

int
GameRenderer::GetTargetControlWidth(void)
{
	// TODO:
	// Return target control width

	return __controlWidth;
}

int
GameRenderer::GetTargetControlHeight(void)
{
	// TODO:
	// Return target control height

	return __controlHeight;
}

void
GameRenderer::SetTargetControlWidth(int width)
{
	// TODO:
	// Assign target control width

	__controlWidth = width;
}

void
GameRenderer::SetTargetControlHeight(int height)
{
	// TODO:
	// Assign target control height

	__controlHeight = height;
}
