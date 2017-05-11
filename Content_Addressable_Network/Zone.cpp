
/**********************************
 * FILE NAME: Zone.cpp
 *
 * Created by Balakrishna Lellapalli on 5/4/17
 * DESCRIPTION: Definition of all Zone related class
 *
 * Copyright © 2017 Balakrishna. All rights reserved.
 **********************************/

#include "Zone.hpp"

Zone::Zone()
{
    boost::geometry::assign_values(p1, 0, 0);
    boost::geometry::assign_values(p2, 100, 0);
    boost::geometry::assign_values(p3, 100, 100);
    boost::geometry::assign_values(p4, 0, 100);
}

Zone Zone::operator=(const Zone& obj)
{
    setZone(obj.p1, obj.p2, obj.p3, obj.p4);
    return *this;
}

void Zone::setZone(boost_geometry::point_xy<short> c1, boost_geometry::point_xy<short> c2,
                   boost_geometry::point_xy<short> c3, boost_geometry::point_xy<short> c4)
{
    boost::geometry::assign_values(p1, c1.x(), c1.y());
    boost::geometry::assign_values(p2, c2.x(), c2.y());
    boost::geometry::assign_values(p3, c3.x(), c3.y());
    boost::geometry::assign_values(p4, c4.x(), c4.y());
}

bool Zone::isCoordinateInZone(boost_geometry::point_xy<short> point)
{
    return (p1.x() <= point.x() && point.x() <= p2.x() &&
            p1.y() <= point.y() && point.y() <= p4.y());
}

Zone Zone::splitZone()
{
    Zone zone_new;
    short d1 = boost::geometry::distance(p1, p2);
    short d2 = boost::geometry::distance(p1, p4);
    
    boost_geometry::point_xy<short> c1(p1.x(), p1.y());
    boost_geometry::point_xy<short> c2(p2.x(), p2.y());
    boost_geometry::point_xy<short> c3(p3.x(), p3.y());
    boost_geometry::point_xy<short> c4(p4.x(), p4.y());
    
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
    zone_new.setZone(c1, c2, c3, c4);
    return zone_new;
}

bool Zone::is_share_x_axis(boost_geometry::point_xy<short> c1, boost_geometry::point_xy<short> c2,
                           boost_geometry::point_xy<short> target_c1, boost_geometry::point_xy<short> target_c2)
{
    if(c1.y() == target_c1.y())
    {
        if (( c1.x() <= target_c1.x()) && (target_c1.x() <= c2.x())) { return true; }
        if ((c1.x() <= target_c2.x() && target_c2.x() <= c2.x())) { return true; }
        if ((target_c1.x() <= c1.x() && c1.x() <= target_c2.x())) { return true; }
    }
    return false;
}

bool Zone::is_share_y_axis(boost_geometry::point_xy<short> c1, boost_geometry::point_xy<short> c2,
                           boost_geometry::point_xy<short> target_c1, boost_geometry::point_xy<short> target_c2)
{
    if(c1.x() == target_c1.x())
    {
        if  (( c1.y() <= target_c1.y() && target_c1.y() <= c2.y())) { return true; }
        if ((c1.y() <= target_c2.y() && target_c2.y() <= c2.y())) { return true; }
        if ((target_c1.y() <= c1.y() && c1.y() <= target_c2.y())) { return true; }
    }
    return false;
}

bool Zone::is_share_axis(Zone zone)
{
    if(is_share_x_axis(p1, p2, zone.p4, zone.p3)) { return true; }
    if(is_share_x_axis(p4, p3, zone.p1, zone.p2)) { return true; }
    
    if(is_share_y_axis(p2, p3, zone.p1, zone.p4)) { return true; }
    if(is_share_y_axis(p1, p4, zone.p2, zone.p3)) { return true; }
    
    return false;
}

short Zone::minDistance(boost::geometry::model::d2::point_xy<short> pt)
{
    short d1 = boost::geometry::distance(p1, pt);
    short d2 = boost::geometry::distance(p2, pt);
    short d3 = boost::geometry::distance(p3, pt);
    short d4 = boost::geometry::distance(p4, pt);
    return std::min(d1, std::min(d2, std::min(d3, d4)));
}

bool Zone::canMergeZone(Zone zone)
{
    if (boost::geometry::equals(p1, zone.p4) && boost::geometry::equals(p2, zone.p3)) { return true; }
    if (boost::geometry::equals(p2, zone.p1) && boost::geometry::equals(p3, zone.p4)) { return true; }
    if (boost::geometry::equals(p3, zone.p2) && boost::geometry::equals(p4, zone.p1)) { return true; }
    if (boost::geometry::equals(p4, zone.p3) && boost::geometry::equals(p1, zone.p2)) { return true; }
    return false;
}

void Zone::mergeZone(Zone& zone)
{
    if (boost::geometry::equals(p1, zone.p4) && boost::geometry::equals(p2, zone.p3))
    {
        boost::geometry::assign_values(p1, zone.p1.x(), zone.p1.y());
        boost::geometry::assign_values(p2, zone.p2.x(), zone.p2.y());
    }
    else if (boost::geometry::equals(p2, zone.p1) && boost::geometry::equals(p3, zone.p4))
    {
        boost::geometry::assign_values(p2, zone.p2.x(), zone.p2.y());
        boost::geometry::assign_values(p3, zone.p3.x(), zone.p3.y());
    }
    else if (boost::geometry::equals(p3, zone.p2) && boost::geometry::equals(p4, zone.p1))
    {
        boost::geometry::assign_values(p3, zone.p3.x(), zone.p3.y());
        boost::geometry::assign_values(p4, zone.p4.x(), zone.p4.y());
    }
    else if (boost::geometry::equals(p4, zone.p3) && boost::geometry::equals(p1, zone.p2))
    {
        boost::geometry::assign_values(p4, zone.p4.x(), zone.p4.y());
        boost::geometry::assign_values(p1, zone.p1.x(), zone.p1.y());
    }
}

std::string Zone::to_String()
{
    std::ostringstream output;
    output << "P1: (" << p1.x() << ", "  << p1.y() << ")"
    << " P2: (" << p2.x() << ", " << p2.y() << ")"
    << " P3: (" << p3.x() << ", " << p3.y() << ")"
    << " P4: (" << p4.x() << ", " << p4.y() <<  ")";
    return output.str();
}
