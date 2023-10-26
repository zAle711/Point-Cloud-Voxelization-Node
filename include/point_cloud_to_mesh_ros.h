#ifndef POINTCLOUDTOMESHROS_H
#define POINTCLOUDTOMESHROS_H

#include <point_cloud_to_mesh.h>
#include <mesh_to_msg.h>
//ROS
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <shape_msgs/Mesh.h>
//PCL 
#include <pcl/point_cloud.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>


//template <typename PointT>
class PointCloudToMeshRos
{
public:
	PointCloudToMeshRos() 
	{
		ros::NodeHandle nh;
		
		cloud_sub = nh.subscribe("camera/rgb/points", 1, &PointCloudToMeshRos::cloudCallBack, this);
		
		shape_pub = nh.advertise<shape_msgs::Mesh>("mesh_shape", 1, true);
		f_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("filtered_point_cloud",1, true);
		
	}
		
	void cloudCallBack(const sensor_msgs::PointCloud2::ConstPtr& cloud_in)
	{
		boost::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> point_cloud (new pcl::PointCloud<pcl::PointXYZ>());
		pcl::fromROSMsg(*cloud_in, *point_cloud);
		
		cloud_to_mesh.set_input(point_cloud);
		
		if (cloud_to_mesh.compute_mesh())
		{
		
			if (shape_pub.getNumSubscribers() > 0)
			{
				shape_msgs::Mesh mesh_msg;
				
				meshToShapeMsg(cloud_to_mesh.getMesh(), mesh_msg);
				shape_pub.publish(mesh_msg);
			
			
			
			}
			
			if (f_cloud_pub.getNumSubscribers() > 0)
			{
			
				sensor_msgs::PointCloud2 point_cloud_msg;
				pcl::toROSMsg(cloud_to_mesh.getFilteredPointCloud(), point_cloud_msg);
				f_cloud_pub.publish(point_cloud_msg);
			
			
			}
		
		}
		
		ROS_INFO("Point Cloud ricevuta!");
	}
	

private:
	ros::Subscriber cloud_sub;
	ros::Publisher shape_pub;
	ros::Publisher f_cloud_pub;
	
	PointCloudToMesh cloud_to_mesh;
	
};

#endif

