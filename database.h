#include <list>
#include "road.h"
#include "roadpoint.h"

class Database {
    public:
        Database() {}
        void insertRoad(const Road& road) {
            roads.push_back(road);
        }
        void insertRoadPoint(const RoadPoint& roadPoint) {
            roadPoints.push_back(roadPoint);
        }
        std::list<Road> getRoads() const {
            return roads;
        }
        std::list<RoadPoint> getRoadPoints() const {
            return roadPoints;
        }
    private:
        std::list<Road> roads;
        std::list<RoadPoint> roadPoints;

};