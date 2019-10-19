#include <ros/ros.h>

#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>

mavros_msgs::State current_state;

void state_cb(const mavros_msgs::State::ConstPtr& msg);

int main(int argc, char *argv[]) {
	
	ros::init(argc, argv, "second_challenge_node");
	ros::NodeHandle nodeHandle;
	
	ros::Subscriber stat_sub = nodeHandle.subscribe<mavros_msgs::State>("mavros/state", 10, &state_cb);
	ros::Publisher local_pos_pub = nodeHandle.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 10);
	ros::ServiceClient arming_client = nodeHandle.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
	ros::ServiceClient set_mode_client = nodeHandle.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
	
	geometry_msg::PoseStamped pose;
	
	ros::Rate rate(20.0);
	
	while (ros::ok() && !current_state.connected) {
		ros::spinOnce();
		rate.sleep();
	}
	
	mavros_msgs::SetMode second_challenge_set_mode;
	second_challenge_set_mode.request.custom_mode = "OFFBOARD";
	mavros_msgs::CommandBool arm_cmd;
	arm_cmd.request.value = true;
	ros::Time last_request = ros::Time::now();

	while (ros::ok()) {
		if (current_state.mode != "OFFBOARD" && (ros::Time::now() - last_request > ros::Duration(5.0))) {
			if (set_mode_client.call(second_challenge_set_mode) && second_challenge_set_mode.response.mode_sent) {
				ROS_INFO("Offboard enabled");
			}
			last_request = ros::Time::now();
		} else {
			if (!current_state.armed && (ros::Time::now() - last_request > ros::Duration(5.0))) {
				if (arming_client.call(arm_cmd) && arm_cmd.response.success) {
					ROS_INFO("Vehicle armed");
				}
				last_request = ros::Time::now();
		}
	}
	
	local_pos_pub.publish(pose);
	
	ros::spinOnce();
	rate.sleep();
	
	return 0;
}

void sate_cb(const mavros_msgs::State::ConstPtr& msg) {
	current_state = *msg;
}