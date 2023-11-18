#pragma once

#include <vector>

#include "Segment.hpp"

typedef enum{segstart, segend, segsintersection, segvertical} EventType;

class Event{

private:

    Point point;
    std::vector<Segment> segs;
    double value;
    EventType type;

public:

    Event();

    Event(const Point& p, const Segment& seg, EventType type);

    Event(const Point& p, const std::vector<Segment> seg, EventType type);

    Point getPoint() const;

    std::vector<Segment> getSegments() const;

    double getValue() const;

    EventType getType() const;

    bool operator() (const Event& lhs, const Event& rhs) const;

    bool equals(const Event& e) const;

    friend std::ostream& operator<<(std::ostream& os, const Event& e);
};