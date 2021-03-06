/**
    Copyright (C) 2015 Jennifer Buehler

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**/
#include <ros/ros.h>
#include <urdf2inventor/Urdf2Inventor.h>
#include <urdf2inventor/InventorViewer.h>
#include <string>

using urdf2inventor::viewer::InventorViewer;
using urdf2inventor::Urdf2Inventor;

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "urdf_viewer", ros::init_options::AnonymousName);
    ros::NodeHandle priv("~");

    if (argc < 2)
    {
        ROS_INFO_STREAM("Usage: " << argv[0] << " <input-file> [--iv] [<from-link>]" );
        ROS_INFO_STREAM(" If --iv is specified, the file is assumed to be an inventor file, otherwise a URDF file." );
        ROS_INFO_STREAM(" if <from-link> is specified (only supported if not using --iv), the URDF is converted from this link down." );
        return 0;
    }

    bool isURDF = true;

    std::string inputFile = argv[1];
    std::string fromLink;
 
    if (argc > 2)
    {
        std::string arg(argv[2]);
        if (arg=="--iv") isURDF=false;
        else if (arg!="root") fromLink=arg;
    } 


    // join fixed links before displaying    
    bool joinFixedLinks = false;
    priv.param<bool>("join_fixed_links", joinFixedLinks, joinFixedLinks);

    // rotate all axes to z    
    bool rotateAxesZ = false;
    priv.param<bool>("rotate_axes_z", rotateAxesZ, rotateAxesZ);
   
    // true to display axes of local joint coordinate systems 
    bool displayAxes = true;
    priv.param<bool>("display_axes", displayAxes, displayAxes);

    float axRad=0.001;
    priv.param<float>("axes_radius", axRad, axRad);
    
    float axLen=0.015;
    priv.param<float>("axes_length", axLen, axLen);

    /**
     * TODO: For some reason I haven't yet further investigated, view.init() has to be called after
     * loadAndGetAsInventor(), or it won't work. Find out why, and fix it.
     * It has probably to do with the calls of SoDB::init involved by ivcon as well.
     */

    bool success = true;
    Urdf2Inventor converter(1, displayAxes, axRad, axLen);
    InventorViewer view;
    if (isURDF)
    {
        ROS_INFO_STREAM("Converting model from file "<<inputFile<<"...");
        if (!fromLink.empty()) ROS_INFO_STREAM("Staring from link "<<fromLink);

        if (!converter.loadModelFromFile(inputFile))
        {
            ROS_ERROR_STREAM("Could not load file "<<inputFile);
            return 0;
        }
        converter.printJointNames(fromLink);

        if (joinFixedLinks && !converter.joinFixedLinks(fromLink))
        {
            ROS_ERROR_STREAM("Could not join fixed links");
            return 0;
        }
        
        Eigen::Vector3d axis(0,0,1);
        if (rotateAxesZ && !converter.allRotationsToAxis(fromLink, axis))
        {
            ROS_ERROR_STREAM("Could not rotate axes");
            return 0;
        }
    
        SoNode * node = converter.getAsInventor(fromLink, false, displayAxes, axRad, axLen);
        if (!node)
        {
            ROS_INFO_STREAM("ERROR: Could not get inventor node");
            success = false;
        }else{
            ROS_INFO_STREAM("Model converted, now loading into viewer...");
            view.init("WindowName");
            view.loadModel(node);
        }
    }
    else
    {
        view.init("WindowName");
        view.loadModel(inputFile);
    }
    if (success)  view.runViewer();

    bool deleteOutputRedirect = true;
    converter.cleanup(deleteOutputRedirect);
    return 0;
}


