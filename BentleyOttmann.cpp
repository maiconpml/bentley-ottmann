#include "BentleyOttmann.hpp"

BentleyOttmann::BentleyOttmann(){}

BentleyOttmann::BentleyOttmann(vector<Segment> segments){

    adjust_segs_vector(segments);

    for(auto& seg: segments){
        if(seg.getP1().x == seg.getP2().x){

            events.push(Event(seg.getP1(), seg, segvertical));
        }else{

            events.push(Event(seg.getP1(), seg, segstart));
            events.push(Event(seg.getP2(), seg, segend));
        }
    }
}

vector<pair<Segment, Segment>> BentleyOttmann::getIntersections() const{
    return intersections;
}

void BentleyOttmann::find_intersections(){

    while(!events.empty()){

        Event e = events.top();
        events.pop();

        Segment::after = false;

        Segment c, a, s, s1, s2;
        set<Segment,Segment>::iterator it;

        switch(e.getType()){
            case segvertical:

                s = e.getSegments().at(0);
                for(auto i: activeSegments){

                    if(i.getValueOnX(e.getValue()) > s.getP1().y && i.getValueOnX(e.getValue()) < s.getP2().y){

                        if(i.intersect(s)){

                            intersections.push_back({i, s});
                        }
                    }
                }
                break;
            case segstart:
                
                Segment::currentX = e.getValue();

                it = activeSegments.insert(e.getSegments().at(0)).first;

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
            case segend:

                Segment::currentX = e.getValue();

                it = activeSegments.find(e.getSegments().at(0));
                c = *(it);
                s = *(++it);
                --it;
                if(it != activeSegments.begin() && ++it != (activeSegments.end())){

                    a = *(----it);
                    verify_intersection(a, s, e);
                }
                //display_set();
                activeSegments.erase(e.getSegments().at(0));
                //display_set();

                break;
            case segsintersection:

                Segment::currentX = e.getValue();

                s1 = e.getSegments().at(0);
                s2 = e.getSegments().at(1);

                adjust_segs(s1, s2, e);

                if(s1.getP1().x < s2.getP1().x || ((s1.getP1().x == s2.getP1().x && s1.getP1().y < s2.getP1().y))){

                    intersections.push_back({s1, s2});
                }else{
                    intersections.push_back({s2, s1});
                }

                break;
        }
    }
}

void BentleyOttmann::display_set() const{

    cout << activeSegments.size() << endl;
    for(auto i : activeSegments){
        cout << i << endl;
    }
    cout << endl;
}

void BentleyOttmann::display_intersections() const{

    cout << intersections.size() << endl;

    for(auto i : intersections){
        cout << i.first << " " << i.second << endl;
    }
}

void BentleyOttmann::display_events() const{
    priority_queue<Event, vector<Event>, Event> auxQ(events);

    int n=auxQ.size();
    for(int i=0; i<n; ++i){
        cout << auxQ.top();
        auxQ.pop();
    }
}

void BentleyOttmann::adjust_segs_vector(vector<Segment>& segments){

    auto it = segments.begin();

    for(it; it!= segments.end(); ++it){

        if((*it).getP2().x < (*it).getP1().x || ((*it).getP2().x == (*it).getP1().x && (*it).getP2().y < (*it).getP1().y)){
            (*it).swap_points();
        }
    }
}

bool BentleyOttmann::verify_events(const Segment& s1, const Segment& s2, const Event& e) const{

    priority_queue<Event, vector<Event>, Event> auxQ(events);

    int n=auxQ.size();
    for(int i=0; i<n; ++i){
        if(auxQ.top().getType() == segsintersection && auxQ.top().equals(e)) return false;
        auxQ.pop();
    }

    return true;
}

void BentleyOttmann::verify_intersection(const Segment& s1, const Segment& s2, const Event& e){

    if(s1.intersect(s2)){
        Point p = s1.point_of_intersection(s2);
        Event auxE(p, {s1,s2}, segsintersection);
        if(p.x > e.getValue() && verify_events(s1, s2, auxE)){
            events.push(auxE);
        }
    }
}

void BentleyOttmann::adjust_segs(Segment& s1, Segment& s2, const Event& e){

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