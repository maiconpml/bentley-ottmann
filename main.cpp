#include <vector>
#include <set>
#include <iostream>
#include "BentleyOttmann.hpp"

using namespace std;

double Segment::currentX = 0;
bool Segment::after = false;

int main(){

    int nSegs;
    cin >> nSegs;

    vector<Segment> segs;

    double x1=0,y1=0,x2=0,y2=0;

    for(int i=0; i<nSegs; ++i){

        cin >> x1 >> y1 >> x2 >> y2;
       
        segs.push_back(Segment((Point){x1,y1}, (Point){x2,y2}));   
    }

    BentleyOttmann bent(segs);
    bent.find_intersections();

    bent.display_intersections();

    return 0;
}