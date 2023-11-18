#include "Event.hpp"

Event::Event(){}

Event::Event(const Point& p, const Segment& seg, EventType type){
    this->point = p;
    this->segs.push_back(seg);
    this->type = type;
    this->value = p.x;
}

Event::Event(const Point& p, const std::vector<Segment> seg, EventType type){
    this->point = p;
    this->type = type;
    this->value = p.x;
    this->segs = seg;
}

Point Event::getPoint() const{
    return point;
}

std::vector<Segment> Event::getSegments() const{
    return segs;
}

double Event::getValue() const{
    return value;
}

EventType Event::getType() const{
    return type;
}


bool Event::operator() (const Event& lhs, const Event& rhs) const{

    return lhs.value > rhs.value;
}

bool Event::equals(const Event& e) const{

    if(e.type == segsintersection){
        if((segs.at(0).equals(e.segs.at(0)) && segs.at(1).equals(e.segs.at(1))) || (segs.at(1).equals(e.segs.at(0)) && segs.at(0).equals(e.segs.at(1)))){
            return true;
        }
    }else{
        if(segs.at(0).equals(e.segs.at(0))) return true;
    }

    return false;
}

std::ostream& operator<<(std::ostream& os, const Event& e){

    os << "value = " << e.getValue() << "; ";

    if(e.getType() == segsintersection){
        os << "intersection - s1 = " << e.getSegments().at(0) << "; s2 = " << e.getSegments().at(1);
    }else{
        os << "start or end - s1 = " << e.getSegments().at(0);
    }
    os << std::endl;
    return os;
}