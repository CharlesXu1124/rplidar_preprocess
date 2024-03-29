#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

#include <memory>
#include <opencv2/opencv.hpp>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;
using namespace cv;

class RPLidarPreprocessNode : public rclcpp::Node
{
public:
  RPLidarPreprocessNode() : Node("scan_subscriber")
  {
    scan_subscriber = this->create_subscription<sensor_msgs::msg::LaserScan>(
        "/scan", 10, std::bind(&RPLidarPreprocessNode::scanReceived, this, _1));

    scan_publisher = this->create_publisher<sensor_msgs::msg::LaserScan>(
        "/scan_processed", 10);
  }

private:
  void scanReceived(sensor_msgs::msg::LaserScan::SharedPtr scan)
  {


    scan_publisher->publish(*scan);

    // pause for 20 milliseconds
    waitKey(20);
  }

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_subscriber;
  rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_publisher;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RPLidarPreprocessNode>());
  rclcpp::shutdown();
  return 0;
}
