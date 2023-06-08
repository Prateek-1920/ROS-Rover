#include <ros/ros.h>
#include <sensor_msgs/NavSatFix.h>

void gps_callback(const sensor_msgs::NavSatFix::ConstPtr& msg)
{
  double lat = msg->latitude;
  double lon = msg->longitude;
  double alt = msg->altitude;
  ROS_INFO("Latitude: %f, Longitude: %f, Altitude: %f", lat, lon, alt);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "gps_subscriber");
  ros::NodeHandle nh;
  ros::Subscriber gps_sub = nh.subscribe("/gps/fix", 10, gps_callback);
  ros::spin();
  return 0;
}
