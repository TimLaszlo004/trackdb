// #include

class RoadPoint {
    public:
        RoadPoint(int road_pnt_id,
                  int ref_road_id,
                  int sequence,
                  long double lat,
                  long double lon) : road_pnt_id(road_pnt_id),
                                     ref_road_id(ref_road_id),
                                     sequence(sequence),
                                     lat(lat),
                                     lon(lon) {}

        RoadPoint(const RoadPoint& other) : road_pnt_id(other.road_pnt_id),
                                            ref_road_id(other.ref_road_id),
                                            sequence(other.sequence),
                                            lat(other.lat),
                                            lon(other.lon) {}
        RoadPoint() = default;
        int getRoadPntId() const {
            return road_pnt_id;
        }
        int getRefRoadId() const {
            return ref_road_id;
        }
        int getSequence() const {
            return sequence;
        }
        long double getLat() const {
            return lat;
        }
        long double getLon() const {
            return lon;
        }
        // RoadPoint operator= (const RoadPoint& rp) {
        //     return RoadPoint(rp);
        // }
        friend std::ostream& operator<<(std::ostream& os, const RoadPoint& roadpoint);
    private:
        int road_pnt_id;
        int ref_road_id;
        int sequence;
        long double lat;
        long double lon;
};

std::ostream& operator<<(std::ostream& os, const RoadPoint& roadpoint) {
    os << "RoadPoint (road_pnt_id=" << roadpoint.road_pnt_id
       << ", ref_road_id=" << roadpoint.ref_road_id
       << ", sequence=" << roadpoint.sequence
       << ", lat=" << roadpoint.lat
       << ", lon=" << roadpoint.lon
       << ")";
    return os;
}