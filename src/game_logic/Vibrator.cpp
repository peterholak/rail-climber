#include "platform/core.h"
#include "Vibrator.h"

void Vibrator::vibrate(int duration) const {
    if (vibrateEnabled) {
        RailClimberPlatform::vibrate(duration);
    }
}
