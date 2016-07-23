#include "bounds.hpp"
#include <cassert>


Point::Point(double x, double y)
: x(x), y(y)
{ }

Bounds::Bounds(const Point& topLeft, const Point& bottomRight)
: topLeft_(topLeft), bottomRight_(bottomRight)
{
    assert(topLeft_.x <= bottomRight_.x && topLeft_.y <= bottomRight_.y);
}

bool Bounds::contain(const Point& p) const {
    return topLeft_.x <= p.x && p.x < bottomRight_.x
        && topLeft_.y <= p.y && p.y < bottomRight_.y;
}

Bounds Bounds::getTopLeftQuadrant() const {
    const auto& tl = topLeft_;
    const auto& br = bottomRight_;

    return Bounds(tl,
                  Point((tl.x + br.x) / 2.0, (tl.y + br.y) / 2.0));
}

Bounds Bounds::getTopRightQuadrant() const {
    const auto& tl = topLeft_;
    const auto& br = bottomRight_;

    return Bounds(Point((tl.x + br.x) / 2.0, tl.y),
                  Point(br.x, (tl.y + br.y) / 2.0));
}

Bounds Bounds::getBottomRightQuadrant() const {
    const auto& tl = topLeft_;
    const auto& br = bottomRight_;

    return Bounds(Point((tl.x + br.x) / 2.0, (tl.y + br.y) / 2.0),
                  br);
}

Bounds Bounds::getBottomLeftQuadrant() const {
    const auto& tl = topLeft_;
    const auto& br = bottomRight_;

    return Bounds(Point(tl.x, (tl.y + br.y) / 2.0),
                  Point((tl.x + br.x) / 2.0, br.y));
}