#pragma once

class Rail;
class StaticObject;
class WaveGenerator;
class Grass;
class Enemy;
class Bullet;
class World;

class RenderDriverInterface {
public:
    virtual void startWorld() = 0;
    virtual void endWorld() = 0;
    virtual void drawRail(const Rail *rail) = 0;
    virtual void drawStaticObject(const StaticObject *staticObject) = 0;
    virtual void drawWaveGenerator(const WaveGenerator *waveGenerator) = 0;
    virtual void drawGrass(const Grass *grass) = 0;
    virtual void drawEnemy(const Enemy *enemy) = 0;
    virtual void drawBullet(const Bullet *bullet) = 0;

    virtual void drawFinishLine(const World *world) = 0;
    virtual void drawPlayer(const World *world) = 0;
    virtual void drawFloor(const World *world) = 0;

    virtual ~RenderDriverInterface() { }
};
