#ifndef GAMERENDERER_H_
#define GAMERENDERER_H_

#include <inttypes.h>
#include <FGraphicsOpengl2.h>
#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <FGrpIGlRenderer.h>

class RailClimber;

class GameRenderer : public Tizen::Graphics::Opengl::IGlRenderer {
public:
    GameRenderer(RailClimber *rc);
    ~GameRenderer();

    virtual bool InitializeGl(void);
    virtual bool TerminateGl(void);

    virtual bool Draw(void);

    virtual bool Pause(void);
    virtual bool Resume(void);

    virtual int GetTargetControlWidth(void);
    virtual int GetTargetControlHeight(void);
    virtual void SetTargetControlWidth(int width);
    virtual void SetTargetControlHeight(int height);

private:
    int __controlWidth, __controlHeight;
    RailClimber *mApp;
};

#endif /* GAMERENDERER_H_ */
