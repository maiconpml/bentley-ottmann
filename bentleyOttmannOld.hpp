#include <vector>
#include <set>
#include <queue>

using namespace std;


typedef enum{sstart, send, intersection} EventType;

typedef struct{
    long double x, y;
}Point;

class Segment{

public:
    Point p1, p2;
    double value;

    Segment(){}

    Segment(const Point& p1, const Point& p2){
        this->p1 = p1;
        this->p2 = p2;
        calc_value(p1.x);
    }

    void calc_value(const double& value){
        this->value = p1.y + ((p2.y-p1.y)/(p2.x-p1.x)*(value-p1.x));
    }

    bool operator() (const Segment& lhs, const Segment& rhs) const{

        return lhs.value < rhs.value;
    }

    bool equals(const Segment& s) const{

        return (s.p1.x == p1.x && s.p1.y == p1.y && s.p2.x == p2.x && s.p2.y == s.p2.y) || (s.p1.x == p2.x && s.p1.y == p2.y && s.p2.x == p1.x && s.p2.y == s.p1.y);
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
};

long double line_side(Segment segment, Point p){

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

            if(seg.p1.x < seg.p2.x){

                events.push(Event(seg.p1, seg, sstart));
                events.push(Event(seg.p2, seg, send));
            }else{
                events.push(Event(seg.p1, seg, send));
                events.push(Event(seg.p2, seg, sstart));
            }
        }
    }

    void find_intersections(){

        while(!events.empty()){
            Event e = events.top();
            events.pop();

            Segment c, a, s, s1, s2;
            set<Segment,Segment>::iterator it;
            Segment dummy;

            switch(e.type){
                case sstart:

                    recalc_values(e.value);
                    e.segs.at(0).calc_value(e.value);

                    it = activeSegments.insert(e.segs.at(0)).first;

                    c = *(it);
                    a = *(--it);
                    s = *(++++it);
                    --it;
                    if(it != activeSegments.begin()){

                        verify_intersection(a, c, e.value);

                    }
                    if(++it != activeSegments.end()){

                        verify_intersection(c, s, e.value);
                    }
                    break;
                case send:

                    it = activeSegments.find(e.segs.at(0));
                    c = *(it);
                    a = *(--it);
                    s = *(++++it);

                    if(--it != activeSegments.begin() && ++it != (activeSegments.end())){

                        verify_intersection(a, s, e.value);
                    }

                    activeSegments.erase(e.segs.at(0));

                    break;
                case intersection:

                    s1 = e.segs.at(0);
                    s2 = e.segs.at(1);

                    adjust_segs(s1, s2);
                    auto it1 = activeSegments.find(s1);
                    auto it2 = activeSegments.find(s2);

                    if(s1.value < s2.value){

                        if(it1 != activeSegments.begin()){
                            a = *(--it1);
                            verify_intersection(a, s1, e.value);
                        }
                        if(++it2 != activeSegments.end()){
                            s = *(it2);
                            verify_intersection(s2, s, e.value);
                        }
                    }else{
                        
                        if(it2 != activeSegments.begin()){
                            a = *(--it2);
                            verify_intersection(a, s2, e.value);
                        }
                        if(++it1 != activeSegments.end()){
                            s = *(it1);
                            verify_intersection(s1, s, e.value);
                        }
                    }
                    intersections.push_back({e.segs.at(0), e.segs.at(1)});
                    break;
            }
        }
    }

    void display_intersections() const{

        cout << intersections.size() << endl;

        for(auto i : intersections){
            cout << i.first.p1.x << " " << i.first.p1.y << " " << i.first.p2.x << " " << i.first.p2.y << " "
            << i.second.p1.x << " " << i.second.p1.y << " " << i.second.p2.x << " " << i.second.p2.y << endl;
        }
    }

private:

    bool verify_events(Point p){

        priority_queue<Event, vector<Event>, Event> auxQ(events);

        int n=auxQ.size();
        for(int i=0; i<n; ++i){
            if(auxQ.top().point.x == p.x && auxQ.top().point.y == p.y) return false;
            auxQ.pop();
        }

        return true;
    }

    void verify_intersection(const Segment& s1, const Segment& s2, const double& x){

        if(intersect(s1, s2)){
            Point p = point_of_intersection(s1, s2);
            
            if(p.x > x && verify_events(p)){
                events.push(Event(p, {s1,s2}, intersection));
            }
        }
    }

    void adjust_segs(Segment& s1, Segment& s2){

        activeSegments.erase(s1);
        activeSegments.erase(s2);

        double aux = s1.value;
        s1.value = s2.value;
        s2.value = aux;

        activeSegments.insert(s1);
        activeSegments.insert(s2);
    }

    void recalc_values(const double& x){

        set<Segment, Segment> auxSegs(activeSegments);
        activeSegments.clear();
        Segment auxS;
        for(auto it : auxSegs){
            auxS = it;
            auxS.calc_value(x);
            activeSegments.insert(auxS);
        }

        priority_queue<Event, vector<Event>, Event> auxEv;

        int n = events.size();
        Event auxE;
        for(int i=0; i<n; ++i){
            auxE = events.top();
            events.pop();
            auxE.segs.at(0).calc_value(x);
            if(auxE.type == intersection) auxE.segs.at(1).calc_value(x);

            auxEv.push(auxE);
        }
        events.swap(auxEv);
    }
};