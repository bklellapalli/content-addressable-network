#ifndef ZONE_HPP
#define ZONE_HPP

#include <list>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

namespace boost_geometry = boost::geometry::model::d2;

class Zone
{
    public:
        boost_geometry::point_xy<short> p1, p2, p3, p4;
    
    public:
        Zone();
        Zone operator=(const Zone& obj);
        Zone splitZone();
        bool is_share_axis(Zone zone);
        bool canMergeZone(Zone zone);
        void mergeZone(Zone& zone);
        void setZone(boost_geometry::point_xy<short> c1, boost_geometry::point_xy<short> c2,
                     boost_geometry::point_xy<short> c3, boost_geometry::point_xy<short> c4);
    
        bool isCoordinateInZone(boost_geometry::point_xy<short> point);
        short minDistance(boost_geometry::point_xy<short> pt);
        std::string to_String();
    
    private:
        bool is_share_x_axis(boost_geometry::point_xy<short> c1, boost_geometry::point_xy<short> c2,
                             boost_geometry::point_xy<short> target_c1, boost_geometry::point_xy<short> target_c2);
    
        bool is_share_y_axis(boost_geometry::point_xy<short> c1, boost_geometry::point_xy<short> c2,
                             boost_geometry::point_xy<short> target_c1, boost_geometry::point_xy<short> target_c2);
};

#endif /* ZONE_HPP */
