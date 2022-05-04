//ROS头文件
#include <ros/ros.h>
//自定义msg产生的头文件
#include "topic_publisher/person.h"

int main(int argc, char **argv)
{
  //用于解析ROS参数，第三个参数为本节点名
  //#include<ros/init.h>
  ros::init(argc, argv, "talker");

  //实例化句柄，初始化node
  //#include<ros/node_handle.h>
  ros::NodeHandle nh;

  //自定义gps msg
  topic_publisher::person msg;
  msg.name ="chao";
  msg.age = 18;
  msg.sex =  topic_publisher::person::male;

  //创建publisher,发布名为gps_info的topic。消息类型为topic_demo::gps,队列长度1
  ros::Publisher pub = nh.advertise<topic_publisher::person>("person_info", 10);

  //定义发布的频率 
  ros::Rate loop_rate(1.0);
  //循环发布msg
  while (ros::ok())
  {
    //以指数增长，每隔1秒更新一次
    msg.age = 1.1*msg.age;
    ROS_INFO("Talker-->person: name:%s age:%d sex:%d ", msg.name.c_str(), msg.age, msg.sex);
    //以1Hz的频率发布msg
    pub.publish(msg);
    //根据前面定义的频率, sleep 1s
    loop_rate.sleep();//根据前面的定义的loop_rate,设置1s的暂停
  }

  return 0;
} 
