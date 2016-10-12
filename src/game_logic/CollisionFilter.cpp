#include "CollisionFilter.h"
#include <Box2D/Box2D.h>

CollisionFilter::CollisionFilter(b2Fixture *fixture) {
    filter = fixture->GetFilterData();
}

CollisionFilter::CollisionFilter(Category category, Mask mask) {
    filter.categoryBits = category;
    filter.maskBits = mask;
}

b2Filter CollisionFilter::get() {
    return filter;
}

void CollisionFilter::applyTo(b2Fixture *fixture) {
    fixture->SetFilterData(filter);
}

CollisionFilter& CollisionFilter::collideWith(Category category) {
    filter.maskBits |= category;
    return *this;
}

CollisionFilter& CollisionFilter::doNotCollideWith(Category category) {
    filter.maskBits = filter.maskBits & ~category;
    return *this;
}

unsigned short CollisionFilter::getMask() {
    return filter.maskBits;
}
