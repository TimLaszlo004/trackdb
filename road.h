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
    private:
        int road_id;
        std::string road_name;
        int road_category;
        bool is_oneway;
        int speed_limit_in_kph;
        int road_width_in_meter;

};