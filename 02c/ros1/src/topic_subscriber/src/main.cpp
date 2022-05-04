//ROS头文件
#include <ros/ros.h>
//包含自定义msg产生的头文件
#include "topic_publisher/person.h"
//ROS标准msg头文件
#include <std_msgs/Float32.h>

void Callback(const topic_publisher::person::ConstPtr &msg)
{  
     ROS_INFO("Talker-->person: name:%s age:%d sex:%d ", msg->name.c_str(), msg->age, msg->sex);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  //创建一个subscriber,订阅名为/person_info的topic,注册回调函数Callback
  ros::Subscriber sub = n.subscribe("/person_info", 1, Callback);
  //ros::spin()用于调用所有可触发的回调函数。将进入循环，不会返回，类似于在循环里反复调用ros::spinOnce()。
  ros::spin(); 
  return 0;
}
