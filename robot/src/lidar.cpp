#include <iostream>
#include <ros/ros.h>
#include <math.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_datatypes.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/LaserScan.h>

using namespace std;
ros::Publisher pub;

class OA
{
public:
    double d_lat, d_lon;

    double Angle(double lat1, double lon1, double lat2, double lon2)
    {
        lat1 = lat1 * M_PI / 180;
        lon1 = lon1 * M_PI / 180;
        lat2 = lat2 * M_PI / 180;
        lon2 = lon2 * M_PI / 180;

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
};

class GPS
{
public:
    double c_lat;
    double c_lon;

    void gpscallback(const sensor_msgs::NavSatFix::ConstPtr &msg)
    {
        c_lat = msg->latitude;
        c_lon = msg->longitude;
        // ROS_INFO("LAT AND LONG = %lf : %lf", c_lat, c_lon);
    }
};

class IMU
{
public:
    double roll, pitch, current_yaw, yaw;
    void imucallback(const sensor_msgs::Imu::ConstPtr &msg)
    {
        tf::Quaternion quat(msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);

        tf::Matrix3x3 ori(quat);
        roll, pitch, current_yaw;
        ori.getRPY(roll, pitch, current_yaw);
        yaw = current_yaw;
        // ROS_INFO("YAW = %lf", yaw);
    }
};

class LIDAR
{
public:
    int length;
    float r_min;
    float c_min;
    float l_min;
    float cen_right;
    float cen_left;

    void lasercallback(const sensor_msgs::LaserScan::ConstPtr &msg)
    {
        length = msg->ranges.size();

        r_min = min(*min_element(msg->ranges.begin(), msg->ranges.begin() + 54), 10.0f);
        cen_right = min(*min_element(msg->ranges.begin() + 54, msg->ranges.begin() + 108), 10.0f);
        c_min = min(*min_element(msg->ranges.begin() + 108, msg->ranges.begin() + 162), 10.0f);
        cen_left = min(*min_element(msg->ranges.begin() + 162, msg->ranges.begin() + 216), 10.0f);
        l_min = min(*min_element(msg->ranges.begin() + 216, msg->ranges.begin() + 270), 10.0f);
    }
};

OA oa;
GPS gps;
IMU imu;
LIDAR l;

// INCLUDE THIS IN CLASS

float linear_x;
float angular_z;
bool check = false;

geometry_msgs::Twist ATT()
{

    geometry_msgs::Twist msg;
    double a = oa.Angle(gps.c_lat, gps.c_lon, oa.d_lat, oa.d_lon);
    double angle_diff = a - imu.yaw;
    double target_dist = std::abs(oa.Distance(gps.c_lat, gps.c_lon, oa.d_lat, oa.d_lon));
    // msg.linear.x = linear_x;
    // msg.angular.z = angular_z;

    // std::cout << "NO OBSTACLE!";
    // linear_x = 0.6;
    // angular_z = 0;
    // if (angle_diff >= 0.01)
    ROS_INFO("remaining dist = %lf", target_dist);
    ROS_INFO("Angle = %lf", angle_diff);

    if (target_dist < 1 and angle_diff < abs(0.01))
    {
        check = true;
    }
    else
    {
        if (target_dist < 1)
        {
            msg.linear.x = 0;
        }
        else if (abs(angle_diff) <= 0.01)
        {
            msg.linear.x = 0.5;
            msg.angular.z = 0;
        }
        else
        {
            msg.angular.z = 3 * angle_diff;
        }
    }

    return msg;
}

geometry_msgs::Twist turn()
{
    geometry_msgs::Twist msg;
    double a = oa.Angle(gps.c_lat, gps.c_lon, oa.d_lat, oa.d_lon);
    double angle_diff = a - imu.yaw;
    double target_dist = std::abs(oa.Distance(gps.c_lat, gps.c_lon, oa.d_lat, oa.d_lon));
    // msg.linear.x = linear_x;
    // msg.angular.z = angular_z;

    if (target_dist > 1)
    {
        msg.angular.z = 1;
        return msg;
    }
}

geometry_msgs::Twist wall()
{
    geometry_msgs::Twist msg;
    double a = oa.Angle(gps.c_lat, gps.c_lon, oa.d_lat, oa.d_lon);
    double angle_diff = a - imu.yaw;
    double target_dist = std::abs(oa.Distance(gps.c_lat, gps.c_lon, oa.d_lat, oa.d_lon));

    if (target_dist > 1)
    {
        msg.linear.x = 0.5;
        return msg;
    }
}

int c;
int find()
{
    int x = 1;
    if (l.r_min > x && l.c_min > x && l.l_min > x && l.cen_right > x && l.cen_left > x)
    {
        cout << "No Obstacle !";
        c = 1;
    }

    else if (l.cen_right > x && l.c_min < x && l.cen_left > x)
    {
        cout << "Obstacle Center!";
        c = 2;
    }

    else if (l.cen_right < x && l.c_min > x && l.cen_left > x)
    {
        cout << "Obstacle Front Right!";
        c = 3;
    }
    else if (l.cen_right > x && l.c_min > x && l.cen_left < x)
    {
        cout << "Obstacle Front Left!";
        c = 1;
    }

    else if (l.cen_right < x && l.c_min < x && l.cen_left > x)
    {
        cout << "Obstacle Centre and Front Right!";
        c = 2;
    }

    else if (l.cen_right > x && l.c_min < x && l.cen_left < x)
    {
        cout << "Obstacle Centre and Front Left!";
        c = 2;
    }

    else if (l.cen_right < x && l.c_min > x && l.cen_left < x)
    {
        cout << "Obstacle Front Right and Front Left!";
        c = 3;
    }

    else if (l.cen_right < x && l.c_min < x && l.cen_left < x)
    {
        cout << "Obstacle Ahead !";
        c = 2;
    }

    return c;
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "gps_node");
    ros::NodeHandle nh;

    // GPS gps;
    // IMU imu;
    // OA oa;

    ros::Subscriber sub_imu = nh.subscribe<sensor_msgs::Imu>("/imu", 1, &IMU::imucallback, &imu);

    ros::Subscriber gps_sub = nh.subscribe<sensor_msgs::NavSatFix>("/gps/fix", 1, &GPS::gpscallback, &gps);

    ros::Subscriber sub = nh.subscribe("/laserscan", 1, &LIDAR::lasercallback, &l);

    pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    ros::Rate loop_rate(1);
    ros::spinOnce();

    cout << "ENTER CO-ORDINATES:";
    cin >> oa.d_lat >> oa.d_lon;

    while (ros::ok() && ! check)
    {
        geometry_msgs::Twist message;
        // double a = oa.Angle(gps.c_lat, gps.c_lon, oa.d_lat, oa.d_lon);
        // double angle_diff = a - imu.yaw;
        // double target_dist = std::abs(oa.Distance(gps.c_lat, gps.c_lon, oa.d_lat, oa.d_lon));
        // if (check)
        //     break;

    

        find();
        if (c == 1)
        {
            message = ATT();
        }
        else if (c == 2)
        {
            message = turn();
        }
        else if (c == 3)
        {
            message = wall();
        }
        

        // message.linear.x = linear_x;
        // message.angular.z = angular_z;
        // loop_rate.sleep();

        pub.publish(message);

        ros::spinOnce();
    }

    return 0;
}

//
