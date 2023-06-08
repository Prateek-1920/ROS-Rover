#include <iostream>
#include <ros/ros.h>
#include <math.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_datatypes.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/NavSatFix.h>

using namespace std;
double c_lat, c_lon, d_lat, d_lon, yaw;

//          lat 2 lon 2 destination coords
double Angle(double lat1, double lon1, double lat2, double lon2)
{
    lat1 = lat1 * M_PI / 180;
    lon1 = lon1 * M_PI / 180;
    lat2 = lat2 * M_PI / 180;
    lon2 = lon2 * M_PI / 180;
    // double del_lon = (lon2 - lon1) * M_PI / 180;
    // double angle = atan2(sin(del_lon) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(del_lon));
    // return angle;
    double angle = atan2(lat2 - lat1, lon2 - lon1);
    return angle;
}

double Distance(double lat1, double lon1, double lat2, double lon2)
{
    const double R = 6371000;
    double d_Lat = (lat2 - lat1) * M_PI / 180;
    double d_Lon = (lon2 - lon1) * M_PI / 180;
    lat1 = lat1 * M_PI / 180;
    lat2 = lat2 * M_PI / 180;
    double a = pow(sin(d_Lat / 2), 2) + pow(sin(d_Lon / 2), 2) * cos(lat1) * cos(lat2);
    double c = 2 * asin(sqrt(a));
    return R * c;
}

void imucallback(const sensor_msgs::Imu::ConstPtr &msg)
{

    tf::Quaternion quat(msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);

    tf::Matrix3x3 ori(quat);
    double roll, pitch, current_yaw;
    ori.getRPY(roll, pitch, current_yaw);
    yaw = current_yaw;
    // double x = msg->orientation.x;
    // double y = msg->orientation.y;
    // double z = msg->orientation.z;
    // double w = msg->orientation.w;
    // double t1 = +2.0 * (w * z + x * y);
    // double t2 = +1.0 - 2.0 * (y * y + z * z);
    // yaw = std::atan2(t1, t2);
    // ROS_INFO("YAW = %lf", yaw);
}

void gpscallback(const sensor_msgs::NavSatFix::ConstPtr &msg)
{
    c_lat = msg->latitude;
    c_lon = msg->longitude;
    // ROS_INFO("LAT: %lf , LON: %lf", c_lat, c_lon);
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "move_node");
    ros::NodeHandle nh;

    ros::Subscriber sub_imu = nh.subscribe<sensor_msgs::Imu>("/imu", 1, imucallback);
    ros::Subscriber sub_gps = nh.subscribe<sensor_msgs::NavSatFix>("/gps/fix", 1, gpscallback);
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    ros::Rate loop_rate(10);

    cout << "ENTER CO-ORDINATES:";
    cin >> d_lat >> d_lon;
    ros::spinOnce();
    bool reached_target_angle = false;
    while (ros::ok() && !reached_target_angle)
    {

        double a = Angle(c_lat, c_lon, d_lat, d_lon);
        double angle_diff = a - yaw;

        double tolerance = 0.01;

        if (angle_diff > M_PI)
        {
            angle_diff -= 2 * M_PI;
        }
        else if (angle_diff < -M_PI)
        {
            angle_diff += 2 * M_PI;
        }

        if (std::abs(angle_diff) > tolerance)
        {
            geometry_msgs::Twist command;

            command.angular.z = 5*angle_diff;
            // command.angular.z = -0.75;
            pub.publish(command);
        }

        else
        {
            reached_target_angle = true;
        }

        ROS_INFO("ANGLE: %lf , ANGLE DIFF: %lf , YAW: %lf", a, angle_diff, yaw);
        ROS_INFO("CURRENT LAT LONG %lf  ,  %lf", c_lat, c_lon);
        ros::spinOnce();
        loop_rate.sleep();
    }

    bool reached_target_dest = false;
    double target_dist = std::abs(Distance(c_lat, c_lon, d_lat, d_lon));

    while (ros::ok() && !reached_target_dest)
    {
        ROS_INFO("LAT: %lf, LON: %lf , target distance: %lf", c_lat, c_lon, target_dist);
        double distance = Distance(c_lat, c_lon, d_lat, d_lon);
        double distance_threshold = 1.0;
        double dist_diff = distance;
        ROS_INFO("dist diff = %lf", dist_diff);

        if (dist_diff > distance_threshold)
        {

            geometry_msgs::Twist vel;
            vel.linear.x = 1.5;
            pub.publish(vel);
        }
        else
        {
            geometry_msgs::Twist vel;
            vel.linear.x = 0.0;
            pub.publish(vel);
            reached_target_dest = true;
        }
        // std::cout << "im here" << std::endl;

        ros::spinOnce();
        //  ros::spin();
        loop_rate.sleep();
    }

    return 0;
}
