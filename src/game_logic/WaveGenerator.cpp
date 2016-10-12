#include "WaveGenerator.h"

float WaveGenerator::radius = 0.3;
float WaveGenerator::touchRadius = 0.5;
float WaveGenerator::maxRadius = 1.0;
int WaveGenerator::duration = 25;
float WaveGenerator::power = 0.15f;

WaveGenerator::WaveGenerator() : x(0), y(0), active(false), state(0), disabled(false) {
}

WaveGenerator::~WaveGenerator() {

}

void WaveGenerator::activate() {
    active = true;
    state = 1;
}

bool WaveGenerator::step() {
    if (active) {
        /*if ((player.position - position).length() < 8+stateStep) {
            Vector v = player.position - position;
            v.setLength((50-stateStep)*0.8);
            player.velocity += v;
        }*/
        state *= 1.14f;
        if (state >= duration) {
            active = false;
            disabled = false;
            state = 0;
        }
    }
    if (disabled) {
        return false;
    }
    return active;
}

float WaveGenerator::currentRadius() const {
    return radius + ((state / duration) * (maxRadius - radius));
}
