enum TrakcPointError {
    NO_ERROR,
    OUT_OF_ROAD,
};

class GPSTrackPoint {
    public:
        GPSTrackPoint(long double timestamp,
                      long double lat,
                      long double lon,
                      long double heading) : timestamp(timestamp), lat(lat), lon(lon), heading(heading) {
            ref_road_id = -1;
            error = TrakcPointError::NO_ERROR;
        }
        GPSTrackPoint(long double timestamp, 
                      long double lat, 
                      long double lon, 
                      long double heading, 
                      int ref_road_id, 
                      TrakcPointError error) : timestamp(timestamp), lat(lat), lon(lon), heading(heading), ref_road_id(ref_road_id), error(error) {}

        long double getTimestamp() const {
            return timestamp;
        }
        long double getLat() const {
            return lat;
        }
        long double getLon() const {
            return lon;
        }
        long double getHeading() const {
            return heading;
        }
        int getRefRoadId() const {
            return ref_road_id;
        }
        TrakcPointError getError() const {
            return error;
        }
        void setError(TrakcPointError error) {
            if (error == TrakcPointError::NO_ERROR) {
                this->error = error;
            }
        }
        void setRefRoadId(int ref_road_id) {
            this->ref_road_id = ref_road_id;
        }
    private:
        long double timestamp;
        long double lat;
        long double lon;
        long double heading;

        // optional
        int ref_road_id;
        TrakcPointError error;
};