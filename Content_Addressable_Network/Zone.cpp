//
//  Zone.cpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 5/4/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include "Zone.hpp"

Zone::Zone()
{
    boost::geometry::assign_values(p1, 0, 1);
    boost::geometry::assign_values(p2, 100, 0);
    boost::geometry::assign_values(p3, 100, 100);
    boost::geometry::assign_values(p4, 0, 100);
}

Zone Zone::operator=(const Zone& obj)
{
    setZone(obj.p1, obj.p2, obj.p3, obj.p4);
    return *this;
}

void Zone::setZone(boost::geometry::model::d2::point_xy<int> c1, boost::geometry::model::d2::point_xy<int> c2,
             boost::geometry::model::d2::point_xy<int> c3, boost::geometry::model::d2::point_xy<int> c4)
{
    boost::geometry::assign_values(p1, c1.x(), c1.y());
    boost::geometry::assign_values(p2, c2.x(), c2.y());
    boost::geometry::assign_values(p3, c3.x(), c3.y());
    boost::geometry::assign_values(p4,c4.x(), c4.y());
}

bool Zone::isCoordinateInZone(boost::geometry::model::d2::point_xy<int> point)
{
    return (p1.x() <= point.x() && point.x() <= p2.x() &&
            p1.y() <= point.y() && point.y() <= p4.y());
}

Zone Zone::splitZone()
{
    short d1 = boost::geometry::distance(p1, p2);
    short d2 = boost::geometry::distance(p1, p4);
    
    boost::geometry::model::d2::point_xy<int> c1(p1.x(), p1.y());
    boost::geometry::model::d2::point_xy<int> c2(p2.x(), p2.y());
    boost::geometry::model::d2::point_xy<int> c3(p3.x(), p3.y());
    boost::geometry::model::d2::point_xy<int> c4(p4.x(), p4.y());
    
    if(d1 > d2)
    {
        p2.x((p1.x() + p2.x())/2);
        p3.x((p4.x() + p3.x())/2);
        boost::geometry::assign_values(c1, p2.x(), p2.y());
        boost::geometry::assign_values(c4, p3.x(), p3.y());
    }
    else
    {
        p4.y((p1.y() + p4.y()) / 2);
        p3.y((p2.y() + p3.y()) /2);
        boost::geometry::assign_values(c1, p4.x(), p4.y());
        boost::geometry::assign_values(c2, p3.x(), p3.y());
    }
    Zone new_zone;
    new_zone.setZone(c1, c2, c3, c4);
    return new_zone;
}

bool Zone::is_share_x_axis(boost::geometry::model::d2::point_xy<int> c1,
                           boost::geometry::model::d2::point_xy<int> c2,
                           boost::geometry::model::d2::point_xy<int> target_c1,
                           boost::geometry::model::d2::point_xy<int> target_c2)
{
    return ( c1.y() == target_c1.y() &&
            (c1.x() <= target_c1.x() && target_c1.x() <= c2.x()) || (c1.x() <= target_c2.x() && target_c2.x() <= c2.x()) ||
            (target_c1.x() <= c1.x() && c1.x() <= target_c2.x()) );
}

bool Zone::is_share_y_axis(boost::geometry::model::d2::point_xy<int> c1,
                           boost::geometry::model::d2::point_xy<int> c2,
                           boost::geometry::model::d2::point_xy<int> target_c1,
                           boost::geometry::model::d2::point_xy<int> target_c2)
{
    
    return ( c1.x() == target_c1.x() &&
            (c1.y() <= target_c1.y() && target_c1.y() <= c2.y()) || (c1.y() <= target_c2.y() && target_c2.y() <= c2.y()) ||
            (target_c1.y() <= c1.y() && c1.y() <= target_c2.y()) );
}

bool Zone::is_share_axis(Zone zone)
{
    if(is_share_x_axis(p1, p2, zone.p4, zone.p3)) return true;
    if(is_share_x_axis(p4, p3, zone.p1, zone.p2)) return true;
    
    if(is_share_y_axis(p2, p3, zone.p1, zone.p4)) return true;
    if(is_share_y_axis(p1, p4, zone.p2, zone.p3)) return true;
    
    return false;
}
