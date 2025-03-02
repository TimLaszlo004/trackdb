#include <iostream>
#include <cmath>

namespace GeometryCalculations {
    constexpr double EARTH_RADIUS = 6371000.0;

    double squaredDistance(long double x1, long double y1, long double x2, long double y2) {
        return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    }

    void gpsToMeters(long double lat1, long double lon1, long double lat2, long double lon2, long double& dx, long double& dy) {
        long double latRad1 = lat1 * M_PI / 180.0;
        long double latRad2 = lat2 * M_PI / 180.0;
        
        long double deltaLat = (lat2 - lat1) * M_PI / 180.0;
        long double deltaLon = (lon2 - lon1) * M_PI / 180.0;
    
        dx = EARTH_RADIUS * deltaLon * cos((latRad1 + latRad2) / 2.0);
        dy = EARTH_RADIUS * deltaLat;
    }

    double pointToSegmentDistance(long double Ax, long double Ay, long double Bx, long double By, long double Px, long double Py) {
        double ABx = Bx - Ax;
        double ABy = By - Ay;
        double APx = Px - Ax;
        double APy = Py - Ay;

        double AB_squared = ABx * ABx + ABy * ABy;
        if (AB_squared == 0) return sqrt(squaredDistance(Ax, Ay, Px, Py)); // A and B are the same point

        double t = (APx * ABx + APy * ABy) / AB_squared;

        t = std::max(0.0, std::min(1.0, t));

        double ClosestX = Ax + t * ABx;
        double ClosestY = Ay + t * ABy;

        return sqrt(squaredDistance(Px, Py, ClosestX, ClosestY));
    }

    bool isPointInsideRoad(long double Ax, long double Ay, long double Bx, long double By, long double Px, long double Py, int roadWidth) {
        long double Axm, Aym, Bxm, Bym, Pxm, Pym;
        gpsToMeters(Ax, Ay, Bx, By, Bxm, Bym);
        gpsToMeters(Ax, Ay, Px, Py, Pxm, Pym);
        Axm = Aym = 0;
        double distance = pointToSegmentDistance(Axm, Aym, Bxm, Bym, Pxm, Pym);
        return distance <= (roadWidth / 2.0);
    }
}
