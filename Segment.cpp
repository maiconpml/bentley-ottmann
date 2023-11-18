#include "Segment.hpp"

Segment::Segment(){}

Segment::Segment(const Point& p1, const Point& p2){
    this->p1 = p1;
    this->p2 = p2;
}

Point Segment::getP1() const{
    return p1;
}

Point Segment::getP2() const{
    return p2;
}

bool Segment::operator() (const Segment& lhs, const Segment& rhs) const{

    if((float)(lhs.p1.y + ((lhs.p2.y-lhs.p1.y)/(lhs.p2.x-lhs.p1.x)*(currentX-lhs.p1.x))) != (float)(rhs.p1.y + ((rhs.p2.y-rhs.p1.y)/(rhs.p2.x-rhs.p1.x)*(currentX-rhs.p1.x)))){
        return (lhs.p1.y + ((lhs.p2.y-lhs.p1.y)/(lhs.p2.x-lhs.p1.x)*(currentX-lhs.p1.x))) < (rhs.p1.y + ((rhs.p2.y-rhs.p1.y)/(rhs.p2.x-rhs.p1.x)*(currentX-rhs.p1.x)));
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

bool Segment::equals(const Segment& s) const{

    return (s.p1.x == p1.x && s.p1.y == p1.y && s.p2.x == p2.x && s.p2.y == s.p2.y) || (s.p1.x == p2.x && s.p1.y == p2.y && s.p2.x == p1.x && s.p2.y == s.p1.y);
}

double Segment::line_side(Point p) const{
    return (p.x - p1.x)*(p2.y - p1.y) - (p.y - p1.y)*(p2.x - p1.x);
}

bool Segment::intersect(Segment s) const{

    if((line_side(s.getP1())*line_side(s.getP2())) < 0 && (s.line_side(p1)*s.line_side(p2)) < 0){
        return true;
    }

    return false;
}

bool Segment::inside_segment(Point p) const{

    if(!line_side(p)){
        if(p.x >= p1.x && p.y >= p1.y && p.x <= p2.x && p.y <= p2.y) return true;
        if(p.x >= p1.x && p.y <= p1.y && p.x <= p2.x && p.y >= p2.y) return true;
        if(p.x <= p1.x && p.y <= p1.y && p.x >= p2.x && p.y >= p2.y) return true;
        if(p.x <= p1.x && p.y >= p1.y && p.x >= p2.x && p.y <= p2.y) return true;
    }

    return false;
}

Point Segment::point_of_intersection(Segment s) const{

    Point p = {0,0};

    double denominator = (p1.x - p2.x)*(s.getP1().y - s.getP2().y) - (p1.y - p2.y)*(s.getP1().x - s.getP2().x);
    
    if(!denominator){
    
        p = p1;
        return p;
    }

    p.x = ((p1.x*p2.y-p1.y*p2.x)*(s.getP1().x - s.getP2().x) - (p1.x - p2.x)*(s.getP1().x*s.getP2().y - s.getP1().y*s.getP2().x)) / denominator;

    p.y = ((p1.x*p2.y-p1.y*p2.x)*(s.getP1().y - s.getP2().y) - (p1.y - p2.y)*(s.getP1().x*s.getP2().y - s.getP1().y*s.getP2().x)) / denominator;

    return p;
}

double Segment::getValueOnX(const double x) const{
    return (p1.y + ((p2.y-p1.y)/(p2.x-p1.x)*(x-p1.x)));
}

std::ostream& operator<<(std::ostream& os, const Segment& s){

    os << s.getP1().x << " " << s.getP1().y << " " << s.getP2().x << " " << s.getP2().y;
    return os;
}
