#include <vector>
#include <algorithm>
#include "catch.hpp"
#include "bounds.hpp"

TEST_CASE("Point comparison returns true for equivalent points and false otherwise", "[point]") {
    Point p1(0, 0);
    Point p2(0, 0);
    Point p3(0, 1);
    Point p4(1, 0);

    REQUIRE(p1 == p2);
    REQUIRE((p1 == p3) == false);
    REQUIRE((p1 == p4) == false);
}

TEST_CASE("Bounds.contain returns true for points inside", "[bounds]") {
    Bounds bounds(Point(-1, -1), Point(1, 1));

    SECTION("definitely inside") {
        Point p(0, 0);
        REQUIRE(bounds.contain(p)==true);
    }

    SECTION("on the left edge") {
        Point l(-1, 0);
        REQUIRE(bounds.contain(l)==true);
    }

    SECTION("on the top edge") {
        Point t(0, -1);
        REQUIRE(bounds.contain(t)==true);
    }

    SECTION("top-left corner") {
        Point tl(-1, -1);
        REQUIRE(bounds.contain(tl)==true);
    }
}

TEST_CASE("Bounds.contain returns false for points outside", "[bounds]") {
    Bounds bounds(Point(-1, -1), Point(1, 1));

    SECTION("definitely outside") {
        Point p(2, 2);
        REQUIRE(bounds.contain(p)==false);
    }

    SECTION("on the right edge") {
        Point l(1, 0);
        REQUIRE(bounds.contain(l)==false);
    }

    SECTION("on the bottom edge") {
        Point t(0, 1);
        REQUIRE(bounds.contain(t)==false);
    }

    SECTION("top-right corner") {
        Point tr(1, -1);
        REQUIRE(bounds.contain(tr)==false);
    }

    SECTION("bottom-right corner") {
        Point br(1, 1);
        REQUIRE(bounds.contain(br)==false);
    }

    SECTION("bottom-left corner") {
        Point bl(-1, 1);
        REQUIRE(bounds.contain(bl)==false);
    }
}

void shiftPoints(std::vector<Point>& points, double xShift, double yShift) {
    for (auto& p : points) {
        p.x += xShift;
        p.y += yShift;
    }
}

TEST_CASE("Bounds quadrant getters return correct sub-bounds.", "[bounds]") {
    Bounds bounds(Point(0, 0), Point(2, 2));

    std::vector<Point> insidePoints = {
        Point(0.0, 0.0), Point(0.5, 0.0),
        Point(0.0, 0.5), Point(0.5, 0.5)
    };
    std::vector<Point> outsidePoints = {
        Point(-0.5, -0.5), Point(0.0, -0.5), Point(0.5, -0.5), Point(1.0, -0.5), Point(1.5, -0.5),
        Point(-0.5,  0.0),                                     Point(1.0,  0.0), Point(1.5,  0.0),
        Point(-0.5,  0.5),                                     Point(1.0,  0.5), Point(1.5,  0.5),
        Point(-0.5,  1.0),                                     Point(1.0,  1.0), Point(1.5,  1.0),
        Point(-0.5,  1.5), Point(0.0,  1.5), Point(0.5,  1.5), Point(1.0,  1.5), Point(1.5,  1.5)
    };

    SECTION("top-left quadrant") {
        auto q = bounds.getTopLeftQuadrant();

        double xShift = 0.0;
        double yShift = 0.0;

        shiftPoints(insidePoints, xShift, yShift);
        shiftPoints(outsidePoints, xShift, yShift);

        REQUIRE(std::all_of(insidePoints.begin(), insidePoints.end(), [&q] (const Point& p) { return q.contain(p); }) == true);
        REQUIRE(std::all_of(outsidePoints.begin(), outsidePoints.end(), [&q] (const Point& p) { return !q.contain(p); }) == true);
    }

    SECTION("top-right quadrant") {
        auto q = bounds.getTopRightQuadrant();

        double xShift = 1.0;
        double yShift = 0.0;

        shiftPoints(insidePoints, xShift, yShift);
        shiftPoints(outsidePoints, xShift, yShift);

        REQUIRE(std::all_of(insidePoints.begin(), insidePoints.end(), [&q] (const Point& p) { return q.contain(p); }) == true);
        REQUIRE(std::all_of(outsidePoints.begin(), outsidePoints.end(), [&q] (const Point& p) { return !q.contain(p); }) == true);
    }

    SECTION("bottom-right quadrant") {
        auto q = bounds.getBottomRightQuadrant();

        double xShift = 1.0;
        double yShift = 1.0;

        shiftPoints(insidePoints, xShift, yShift);
        shiftPoints(outsidePoints, xShift, yShift);

        REQUIRE(std::all_of(insidePoints.begin(), insidePoints.end(), [&q] (const Point& p) { return q.contain(p); }) == true);
        REQUIRE(std::all_of(outsidePoints.begin(), outsidePoints.end(), [&q] (const Point& p) { return !q.contain(p); }) == true);
    }

    SECTION("bottom-left quadrant") {
        auto q = bounds.getBottomLeftQuadrant();

        double xShift = 0.0;
        double yShift = 1.0;

        shiftPoints(insidePoints, xShift, yShift);
        shiftPoints(outsidePoints, xShift, yShift);

        REQUIRE(std::all_of(insidePoints.begin(), insidePoints.end(), [&q] (const Point& p) { return q.contain(p); }) == true);
        REQUIRE(std::all_of(outsidePoints.begin(), outsidePoints.end(), [&q] (const Point& p) { return !q.contain(p); }) == true);
    }
}