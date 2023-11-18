#pragma once

#include <iostream>

#include "Point.hpp"

class Segment{

private:
    Point p1, p2;

public:
    static double currentX;
    static bool after;

    Segment();

    Segment(const Point& p1, const Point& p2);

    Point getP1() const;

    Point getP2() const;

    void swap_points();

    bool operator() (const Segment& lhs, const Segment& rhs) const;

    bool equals(const Segment& s) const;

    double getValueOnX(const double x) const;

    double line_side(Point p) const;

    bool intersect(Segment seg2) const;

    bool inside_segment(Point p) const;

    Point point_of_intersection(Segment s2) const;

    friend std::ostream& operator<<(std::ostream& os, const Segment& s);
};