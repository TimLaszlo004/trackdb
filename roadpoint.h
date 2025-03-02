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
    private:
        int road_pnt_id;
        int ref_road_id;
        int sequence;
        long double lat;
        long double lon;
};