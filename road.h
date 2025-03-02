#include <string>

class Road {
    public:
        Road(int road_id,
             std::string road_name,
             int road_category,
             bool is_oneway,
             int speed_limit_in_kph,
             int road_width_in_meter) : road_id(road_id),
                                        road_name(road_name),
                                        road_category(road_category),
                                        is_oneway(is_oneway),
                                        speed_limit_in_kph(speed_limit_in_kph),
                                        road_width_in_meter(road_width_in_meter) {}
        Road(const Road& other) : road_id(other.road_id),
                                  road_name(other.road_name),
                                  road_category(other.road_category),
                                  is_oneway(other.is_oneway),
                                  speed_limit_in_kph(other.speed_limit_in_kph),
                                  road_width_in_meter(other.road_width_in_meter){}
        Road() = default;
        int getRoadId() const {
            return road_id;
        }
        std::string getRoadName() const {
            return road_name;
        }
        int getRoadCategory() const {
            return road_category;
        }
        bool getIsOneway() const {
            return is_oneway;
        }
        int getSpeedLimitInKph() const {
            return speed_limit_in_kph;
        }
        int getRoadWidthInMeter() const {
            return road_width_in_meter;
        }
        friend std::ostream& operator<<(std::ostream& os, const Road& road);
    private:
        int road_id;
        std::string road_name;
        int road_category;
        bool is_oneway;
        int speed_limit_in_kph;
        int road_width_in_meter;
};

std::ostream& operator<<(std::ostream& os, const Road& road) {
    os << "Road (road_id=" << road.road_id
       << ", road_name=" << road.road_name
       << ", road_category=" << road.road_category
       << ", is_oneway=" << road.is_oneway
       << ", speed_limit_in_kph=" << road.speed_limit_in_kph
       << ", road_width_in_meter=" << road.road_width_in_meter
       << ")";
    return os;
}