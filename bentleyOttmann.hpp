#include <vector>
#include <set>
#include <queue>
#include <iostream>

using namespace std;

typedef enum{sstart, send, intersection, vertical} EventType;

typedef struct{
    double x, y;
}Point;

class Segment{

public:
    Point p1, p2;
    static double value;
    static bool after;

    Segment(){}

    Segment(const Point& p1, const Point& p2){
        this->p1 = p1;
        this->p2 = p2;
    }

    bool operator() (const Segment& lhs, const Segment& rhs) const{

        if((float)(lhs.p1.y + ((lhs.p2.y-lhs.p1.y)/(lhs.p2.x-lhs.p1.x)*(value-lhs.p1.x))) != (float)(rhs.p1.y + ((rhs.p2.y-rhs.p1.y)/(rhs.p2.x-rhs.p1.x)*(value-rhs.p1.x)))){
            return (lhs.p1.y + ((lhs.p2.y-lhs.p1.y)/(lhs.p2.x-lhs.p1.x)*(value-lhs.p1.x))) < (rhs.p1.y + ((rhs.p2.y-rhs.p1.y)/(rhs.p2.x-rhs.p1.x)*(value-rhs.p1.x)));
        }else{

            double angCoefLhs = (lhs.p2.y-lhs.p1.y)/(lhs.p2.x-lhs.p1.x);
            double angCoefRhs = (rhs.p2.y-rhs.p1.y)/(rhs.p2.x-rhs.p1.x);
                        
            if(after){

                return angCoefLhs < angCoefRhs;
            }else{
                return angCoefLhs > angCoefRhs;
            }
        }
    }

    bool equals(const Segment& s) const{

        return (s.p1.x == p1.x && s.p1.y == p1.y && s.p2.x == p2.x && s.p2.y == s.p2.y) || (s.p1.x == p2.x && s.p1.y == p2.y && s.p2.x == p1.x && s.p2.y == s.p1.y);
    }

    friend ostream& operator<<(ostream& os, const Segment& s){

        os << s.p1.x << " " << s.p1.y << " " << s.p2.x << " " << s.p2.y;
        return os;
    }

    double get_value(const double value) const{
        return (p1.y + ((p2.y-p1.y)/(p2.x-p1.x)*(value-p1.x)));
    }
};

class Event{

public:

    Point point;
    vector<Segment> segs;
    double value;
    EventType type;

    Event(){}

    Event(const Point& p, const Segment& seg, EventType type){
        this->point = p;
        this->segs.push_back(seg);
        this->type = type;
        this->value = p.x;
    }

    Event(const Point& p, const vector<Segment> seg, EventType type){
        this->point = p;
        this->type = type;
        this->value = p.x;
        this->segs = seg;
    }

    bool operator() (const Event& lhs, const Event& rhs) const{

        return lhs.value > rhs.value;
    }

    bool equals(const Event& e) const{

        if(e.type == intersection){
            if((segs.at(0).equals(e.segs.at(0)) && segs.at(1).equals(e.segs.at(1))) || (segs.at(1).equals(e.segs.at(0)) && segs.at(0).equals(e.segs.at(1)))){
                return true;
            }
        }else{
            if(segs.at(0).equals(e.segs.at(0))) return true;
        }

        return false;
    }

    friend ostream& operator<<(ostream& os, const Event& e){

        os << "value=" << e.value << "; ";

        if(e.type == intersection){
            os << e.segs.at(0) << "-" << e.segs.at(1);
        }else{
            os << e.segs.at(0);
        }
        os << endl;
        return os;
    }
};

double line_side(Segment segment, Point p){

    return (p.x - segment.p1.x)*(segment.p2.y - segment.p1.y) - (p.y - segment.p1.y)*(segment.p2.x - segment.p1.x);
}

bool intersect(Segment seg1, Segment seg2){

    if((line_side(seg1, seg2.p1)*line_side(seg1, seg2.p2)) < 0 && (line_side(seg2, seg1.p1)*line_side(seg2, seg1.p2)) < 0){
        return true;
    }

    return false;
}

bool inside_segment(Segment segment, Point p){

    if(!line_side(segment, p)){
        if(p.x >= segment.p1.x && p.y >= segment.p1.y && p.x <= segment.p2.x && p.y <= segment.p2.y) return true;
        if(p.x >= segment.p1.x && p.y <= segment.p1.y && p.x <= segment.p2.x && p.y >= segment.p2.y) return true;
        if(p.x <= segment.p1.x && p.y <= segment.p1.y && p.x >= segment.p2.x && p.y >= segment.p2.y) return true;
        if(p.x <= segment.p1.x && p.y >= segment.p1.y && p.x >= segment.p2.x && p.y <= segment.p2.y) return true;
    }

    return false;
}

Point point_of_intersection(Segment s1, Segment s2){

    Point p = {0,0};

    double denominator = (s1.p1.x - s1.p2.x)*(s2.p1.y - s2.p2.y) - (s1.p1.y - s1.p2.y)*(s2.p1.x - s2.p2.x);
    
    if(!denominator){
    
        p = s1.p1;
        return p;
    }

    p.x = ((s1.p1.x*s1.p2.y-s1.p1.y*s1.p2.x)*(s2.p1.x - s2.p2.x) - (s1.p1.x - s1.p2.x)*(s2.p1.x*s2.p2.y - s2.p1.y*s2.p2.x)) / denominator;

    p.y = ((s1.p1.x*s1.p2.y-s1.p1.y*s1.p2.x)*(s2.p1.y - s2.p2.y) - (s1.p1.y - s1.p2.y)*(s2.p1.x*s2.p2.y - s2.p1.y*s2.p2.x)) / denominator;

    /*if(inside_segment(s1, s2.p1)) p = s2.p1;
    if(inside_segment(s1, s2.p2)) p = s2.p2;
    if(inside_segment(s2, s1.p1)) p = s1.p1;
    if(inside_segment(s2, s1.p2)) p = s1.p2;*/

    return p;
}

class BentleyOttmann{

public:

    vector<Segment> segments;
    set<Segment, Segment> activeSegments;
    priority_queue<Event, vector<Event>, Event> events;
    vector<pair<Segment, Segment>> intersections;

    BentleyOttmann(){}

    BentleyOttmann(vector<Segment> segments){
        this->segments = segments;

        for(auto& seg: segments){
            if(seg.p1.x == seg.p2.x){

                events.push(Event(seg.p1, seg, vertical));
            }else{

                events.push(Event(seg.p1, seg, sstart));
                events.push(Event(seg.p2, seg, send));
            }
        }
    }

    void find_intersections(){
        int k=0;
        while(!events.empty()){
            Event e = events.top();
            events.pop();

            Segment::after = false;

            Segment c, a, s, s1, s2;
            set<Segment,Segment>::iterator it;

            switch(e.type){
                case vertical:

                    s = e.segs.at(0);
                    for(auto i: activeSegments){

                        if(i.get_value(e.value) > s.p1.y && i.get_value(e.value) < s.p2.y){

                            if(intersect(i, s)){

                                intersections.push_back({i, s});
                            }
                        }
                    }
                    break;
                case sstart:
                    
                    Segment::value = e.value;

                    it = activeSegments.insert(e.segs.at(0)).first;

                    c = *(it);
                    a = *(--it);
                    s = *(++++it);
                    --it;
                    if(it != activeSegments.begin()){

                        verify_intersection(a, c, e);
                    }
                    if(++it != activeSegments.end()){

                        verify_intersection(c, s, e);
                    }
                    break;
                case send:

                    Segment::value = e.value;

                    it = activeSegments.find(e.segs.at(0));
                    c = *(it);
                    s = *(++it);
                    --it;
                    if(it != activeSegments.begin() && ++it != (activeSegments.end())){

                        a = *(----it);
                        verify_intersection(a, s, e);
                    }
                    //display_set();
                    activeSegments.erase(e.segs.at(0));
                    //display_set();

                    break;
                case intersection:

                    Segment::value = e.value;

                    s1 = e.segs.at(0);
                    s2 = e.segs.at(1);

                    adjust_segs(s1, s2, e);

                    if(s1.p1.x < s2.p1.x || ((s1.p1.x == s2.p1.x && s1.p1.y < s2.p1.y))){

                        intersections.push_back({s1, s2});
                    }else{
                        intersections.push_back({s2, s1});
                    }

                    break;
            }
        }
    }

    void display_set() const{

        cout << activeSegments.size() << endl;
        for(auto i : activeSegments){
            cout << i << endl;
        }
        cout << endl;
    }

    void display_intersections() const{

        cout << intersections.size() << endl;

        for(auto i : intersections){
            cout << i.first << " " << i.second << endl;
        }
    }

    void display_events() const{
        priority_queue<Event, vector<Event>, Event> auxQ(events);

        int n=auxQ.size();
        for(int i=0; i<n; ++i){
            cout << auxQ.top();
            auxQ.pop();
        }
    }

private:

    bool verify_events(const Segment& s1, const Segment& s2, const Event& e){

        priority_queue<Event, vector<Event>, Event> auxQ(events);

        int n=auxQ.size();
        for(int i=0; i<n; ++i){
            if(auxQ.top().type == intersection && auxQ.top().equals(e)) return false;
            auxQ.pop();
        }

        return true;
    }

    void verify_intersection(const Segment& s1, const Segment& s2, const Event& e){

        if(intersect(s1, s2)){
            Point p = point_of_intersection(s1, s2);
            Event auxE(p, {s1,s2}, intersection);
            if(p.x > e.value && verify_events(s1, s2, auxE)){
                events.push(auxE);
            }
        }
    }

    void adjust_segs(Segment& s1, Segment& s2, const Event& e){

        //display_set();
        activeSegments.erase(s1);
        activeSegments.erase(s2);
        //display_set();
        Segment::after = true;

        auto seg = activeSegments.insert(s1).first;

        auto c = *(seg);
        auto a = *(--seg);
        auto s = *(++++seg);
        //display_events();
        --seg;
        if(seg != activeSegments.begin()){

            verify_intersection(a, c, e);
        }
        if(++seg != activeSegments.end()){

            verify_intersection(c, s, e);
        }
        
        seg = activeSegments.insert(s2).first;
        c = *(seg);
        a = *(--seg);
        s = *(++++seg);

        --seg;
        if(seg != activeSegments.begin()){

            verify_intersection(a, c, e);
        }
        if(++seg != activeSegments.end()){

            verify_intersection(c, s, e);
        }
        //display_events();
        //display_set();

        Segment::after = false;
    }
};