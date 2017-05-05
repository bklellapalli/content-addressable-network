//
//  Zone.hpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef ZONE_HPP
#define ZONE_HPP

#include <list>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

class Zone
{
    public:
        boost::geometry::model::d2::point_xy<int> p1, p2, p3, p4;
    
    public:
        Zone();
        Zone operator=(const Zone& obj);

        Zone splitZone();
        void setZone(boost::geometry::model::d2::point_xy<int> c1,
                     boost::geometry::model::d2::point_xy<int> c2,
                     boost::geometry::model::d2::point_xy<int> c3,
                     boost::geometry::model::d2::point_xy<int> c4);
    
        bool isCoordinateInZone(boost::geometry::model::d2::point_xy<int> point);
        bool is_share_axis(Zone zone);
    
    private:
        bool is_share_x_axis(boost::geometry::model::d2::point_xy<int> c1,
                         boost::geometry::model::d2::point_xy<int> c2,
                         boost::geometry::model::d2::point_xy<int> target_c1,
                         boost::geometry::model::d2::point_xy<int> target_c2);
    
        bool is_share_y_axis(boost::geometry::model::d2::point_xy<int> c1,
                         boost::geometry::model::d2::point_xy<int> c2,
                         boost::geometry::model::d2::point_xy<int> target_c1,
                         boost::geometry::model::d2::point_xy<int> target_c2);
};

#endif /* ZONE_HPP */
