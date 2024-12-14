#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/CompressedImage.h>
#include <sensor_msgs/CameraInfo.h>

class CompressedImageDownsampler
{
private:
    /*node handle*/
    ros::NodeHandle nh_;
    ros::NodeHandle nh_private_;
    /*subscriber*/
    message_filters::Subscriber<sensor_msgs::CompressedImage> image_sub_;
    message_filters::Subscriber<sensor_msgs::CameraInfo> info_sub_;
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::CompressedImage, sensor_msgs::CameraInfo> MySyncPolicy;
    typedef message_filters::Synchronizer<MySyncPolicy> MySynchronizer;
    boost::shared_ptr<MySynchronizer> sync_ptr_;
    /*publisher*/
    ros::Publisher image_pub_;
    ros::Publisher info_pub_;
    /*buffer*/
    bool got_first_msg_;
    ros::Time last_time_;
    /*parameter*/
    float publish_hz_;

public:
    CompressedImageDownsampler();
    void callback(const sensor_msgs::CompressedImageConstPtr &image_msg, const sensor_msgs::CameraInfoConstPtr &info_msg);
};

CompressedImageDownsampler::CompressedImageDownsampler()
    : nh_private_("~"), got_first_msg_(false)
{
    std::cout << "----- compressed_image_downsampler -----" << std::endl;
    /*parameter*/
    nh_private_.param("publish_hz", publish_hz_, 1.0f);
    std::cout << "publish_hz_ = " << publish_hz_ << std::endl;
    /*subscriber*/
    image_sub_.subscribe(nh_, "/image_raw/compressed", 1);
    info_sub_.subscribe(nh_, "/camera_info", 1);
    sync_ptr_.reset(new MySynchronizer(MySyncPolicy(10), image_sub_, info_sub_));
    sync_ptr_->registerCallback(boost::bind(&CompressedImageDownsampler::callback, this, _1, _2));
    /*publisher*/
    image_pub_ = nh_.advertise<sensor_msgs::CompressedImage>("/image_raw/downsampled/compressed", 1);
    info_pub_ = nh_.advertise<sensor_msgs::CameraInfo>("/camera_info/downsampled", 1);
}

void CompressedImageDownsampler::callback(const sensor_msgs::CompressedImageConstPtr &image_msg, const sensor_msgs::CameraInfoConstPtr &info_msg)
{
    const ros::Time &curr_time = image_msg->header.stamp;
    const float interval_sec = (curr_time - last_time_).toSec();
    if (!got_first_msg_ || interval_sec > 1 / publish_hz_)
    {
        image_pub_.publish(image_msg);
        info_pub_.publish(info_msg);
        last_time_ = curr_time;
    }
    got_first_msg_ = true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "compressed_image_downsampler");
    CompressedImageDownsampler compressed_image_downsampler;
    ros::spin();
}