#pragma once

#include <vector>
#include <set>
#include <queue>
#include <iostream>

#include "Event.hpp"
#include "Segment.hpp"

using namespace std;

class BentleyOttmann{

private:

    set<Segment, Segment> activeSegments;
    priority_queue<Event, vector<Event>, Event> events;
    vector<pair<Segment, Segment>> intersections;

public:

    BentleyOttmann();

    BentleyOttmann(vector<Segment> segments);

    vector<pair<Segment, Segment>> getIntersections() const;

    void find_intersections();

    void display_set() const;

    void display_intersections() const;

    void display_events() const;

private:

    bool verify_events(const Segment& s1, const Segment& s2, const Event& e) const;

    void verify_intersection(const Segment& s1, const Segment& s2, const Event& e);

    void adjust_segs(Segment& s1, Segment& s2, const Event& e);
};