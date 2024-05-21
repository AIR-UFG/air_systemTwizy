#include "sd_control/sd_teleop_joy.hpp"

#include <sd_msgs/msg/sd_control.hpp>

namespace sd_control
{

  SdTeleopJoy::SdTeleopJoy(const std::string & name, const std::string & namespace_)
  : rclcpp::Node(name, namespace_)
  {
    joy_sub_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "joy", 
      1, 
      std::bind(&SdTeleopJoy::joyCallback, this, std::placeholders::_1));

    control_pub_ = this->create_publisher<sd_msgs::msg::SDControl>("/sd_control", 1);

    enable_button_ = this->declare_parameter<int>("enable_button", 0);
    throttle_axis_ = this->declare_parameter<int>("throttle_axis", 1);
    steer_axis_ = this->declare_parameter<int>("steer_axis", 0);
  }

  void SdTeleopJoy::joyCallback(sensor_msgs::msg::Joy::SharedPtr joy)
  {
    if (joy->buttons[enable_button_] == 1) {
      auto control_msg = std::make_shared<sd_msgs::msg::SDControl>();
      control_msg->torque = joy->axes[throttle_axis_] * 100;
      control_msg->steer = joy->axes[steer_axis_] * 100;
      control_pub_->publish(control_msg);
    }
  }
}