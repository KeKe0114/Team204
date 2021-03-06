#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>

ros::Publisher move_pub;
bool swit;

void lidarCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    int nNum = scan->ranges.size();
    
    
    int nLeft = nNum/4;
    float leftDist = scan->ranges[nLeft];

    int nMid = nNum/2;
    float frontDist = scan->ranges[nMid];
    int nLeftMid = (nMid+nLeft)/2;
    float leftMidDist = scan->ranges[nLeftMid];
    
    int nRight = nLeft+nMid;
    float rightDist = scan->ranges[nRight];
    int nRightMid = (nMid+nRight)/2;
    float rightMidDist = scan->ranges[nRightMid];
    //ROS_INFO("Point[%d] = %f,Point[%d] = %f,Point[%d] = %f, Point[%d] = %f,Point[%d] = %f", 
      //  nLeft, leftDist, nLeftMid, leftMidDist, nMid, frontDist, nRightMid, rightMidDist, nRight, rightDist); 

std_msgs::Int32  bar_msg;
   // geometry_msgs::Twist vel_cmd;
    float dis = 1.5;
    bool barrier = false;
    if(frontDist > dis && leftDist > dis && rightDist > dis && leftMidDist > dis && rightMidDist > dis)
    {
        //vel_cmd.linear.x = 0.4;
        barrier = false;
    }
    else
    {
        //vel_cmd.angular.z = 0.3;
        barrier = true;
    }

    //vel_pub.publish(vel_cmd);
    if (swit && barrier) 
    {
        bar_msg.data = 1;
        move_pub.publish(bar_msg);
        ros::spinOnce();
    }
    else
    {
        bar_msg.data = 0;
        // if (swit)
        // {
            move_pub.publish(bar_msg);
            ros::spinOnce();
        // }
    }
}

void barrier_ctrl(const std_msgs::Int32::ConstPtr & msg)
{
    printf("barrier  recieve\n");
   if (msg->data == 0)
   {
      swit = false;
   }
   else
   {
     swit = true;
   }
}

int main(int argc, char** argv)
{
    swit = false;
    ros::init(argc,argv,"barrier_detection");
    
    ROS_INFO("barrier_ start!");

    ros::NodeHandle nh;
    ros::Subscriber lidar_sub = nh.subscribe("/scan", 10, &lidarCallback);
     ros::Subscriber barrier_switch = nh.subscribe("/barrier_switch", 10, barrier_ctrl);
    move_pub = nh.advertise<std_msgs::Int32>("/move_lock", 10);
    //vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",10);
    //vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop",10);

    ros::spin();
}
