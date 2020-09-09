//
// Created by ohashi on 19/09/13.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include <Eigen/Core>

#include <ros/ros.h>
#include <ros/console.h>

#include <sensor_msgs/Imu.h>
#include <geometry_msgs/PoseStamped.h>

#define TOPIC_NAME_SUBSCRIBED ""
#define TOPIC_NAME_PUBLISHED ""

class ClassName{

public:

    ClassName(ros::NodeHandle *nh, ros::NodeHandle *private_nh);
    ~ClassName();
    void callback(const sensor_msgs::Imu::ConstPtr &msg);

    void run();
    bool initialization();

private:

    ros::NodeHandle nh;
    ros::NodeHandle private_nh;
    ros::Publisher pub;
    ros::Subscriber sub;
    ros::Time current;

    sensor_msgs::Imu msg_imu;

    bool update_imu=false;

};

ClassName::ClassName(ros::NodeHandle *nh, ros::NodeHandle *private_nh) : nh(*nh), private_nh(*private_nh)
{

    ROS_DEBUG("%s",__FUNCTION__);

    /** Set parameter **/

    /** Set publisher **/
    pub=this->nh.advertise<geometry_msgs::PoseStamped>(TOPIC_NAME_PUBLISHED, 1);

    /** Set subscriber **/
    sub=this->nh.subscribe(TOPIC_NAME_SUBSCRIBED,1,&ClassName::callback,this);

    ros::spin();
}

ClassName::~ClassName() {}

/**
 * callback
 * @param msg
 */
void ClassName::callback(const sensor_msgs::Imu::ConstPtr &msg) {

    auto tmp=*msg;
    ROS_DEBUG_STREAM("[" << std::string(__FUNCTION__) << "]\n" << tmp);
    msg_imu = *msg;
    current = msg_imu.header.stamp;
    update_imu = true;
    run();

}

/**
 * run
 */
void ClassName::run() {

    ROS_DEBUG_STREAM("[" << std::string(__FUNCTION__) << "]\n");

    /** Initialization **/
    static bool init=false;
    if(!init)
        init=initialization();
    if(!(init)){
        ROS_WARN("Not Completed Initialization \ninit: %d",init);
        return;
    }

}

/**
 * initialization
 */
bool ClassName::initialization() {

    ROS_DEBUG_STREAM("[" << std::string(__FUNCTION__) << "]\n");

    if (!update_imu)
        return false;


    return true;

}


/**
 * Main Function
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) {

    // Initialize ROS
    ros::init(argc, argv, "node_name");
    ros::NodeHandle nh, private_nh("~");
    // debug mode
    bool debug;
    private_nh.param<bool>("debug", debug, false);
    if (debug) {
        if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug)) {
            ros::console::notifyLoggerLevelsChanged();
        }
    }
    ClassName node(&nh, &private_nh);
    ros::spin();

    return (0);
}
