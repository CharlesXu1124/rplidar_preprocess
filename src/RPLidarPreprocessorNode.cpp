#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <image_transport/image_transport.hpp>

#include <memory>
#include "cv_bridge/cv_bridge.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

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

    scan_publisher = this->create_publisher<sensor_msgs::msg::CompressedImage>(
        "/scan_processed", 10);
  }

private:
  void scanReceived(sensor_msgs::msg::LaserScan::SharedPtr scan)
  {
    uint32_t counter = 0;
    std::vector<float> ranges = scan->ranges;
    for (int i = 0; i < ranges.size(); ++i)
    {
      if (ranges[i] > 100)
      {
        counter++;
      }
    }

    RCLCPP_INFO(get_logger(), "there are %d empty points", counter);

  }

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_subscriber;
  rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr scan_publisher;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RPLidarPreprocessNode>());
  rclcpp::shutdown();
  return 0;
}
