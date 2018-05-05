#ifndef IV_PCL_DATA_TYPES_H
#define IV_PCL_DATA_TYPES_H
#ifndef Q_MOC_RUN
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

namespace iv {

typedef pcl::PointCloud<pcl::PointXYZRGBA> Cloud;
typedef Cloud::Ptr CloudPtr;
typedef Cloud::ConstPtr CloudConstPtr;
}
#endif
#endif // IV_PCL_DATA_TYPES_H
