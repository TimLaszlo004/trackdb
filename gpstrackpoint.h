enum TrackPointError {
    NO_ERROR = 0,
    NOT_FOUND = 1
};

class GPSTrackPoint {
    public:
        GPSTrackPoint(long double timestamp,
                      long double lat,
                      long double lon,
                      long double heading) : timestamp(timestamp), lat(lat), lon(lon), heading(heading) {
            ref_road_id = -1;
            error = TrackPointError::NO_ERROR;
        }
        GPSTrackPoint(long double timestamp, 
                      long double lat, 
                      long double lon, 
                      long double heading, 
                      int ref_road_id, 
                      TrackPointError error) : timestamp(timestamp), lat(lat), lon(lon), heading(heading), ref_road_id(ref_road_id), error(error) {}

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
        TrackPointError getError() const {
            return error;
        }
        void setError(TrackPointError error) {
            if (this->error == TrackPointError::NO_ERROR) {
                this->error = error;
            }
        }
        void setRefRoadId(int ref_road_id) {
            this->ref_road_id = ref_road_id;
        }
        friend std::ostream& operator<<(std::ostream& os, const GPSTrackPoint& point);
    private:
        long double timestamp;
        long double lat;
        long double lon;
        long double heading;

        // optional
        int gps_trk_id = -1; // not used
        int ref_road_id = -1;
        TrackPointError error = NO_ERROR;
};

std::ostream& operator<<(std::ostream& os, const GPSTrackPoint& point) {
    os << "GPSTrackPoint(gps_trk_id (not used)=" << point.gps_trk_id
       << ", ref_road_id=" << point.ref_road_id
       << ", timestamp=" << point.timestamp
       << ", lat=" << point.lat
       << ", lon=" << point.lon
       << ", heading=" << point.heading
       << ", error=" << point.error
       << ")";
    return os;
}