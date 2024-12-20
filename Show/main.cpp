#include <pcl/io/ply_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/point_types.h>

int main()
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

    if (pcl::io::loadPLYFile<pcl::PointXYZ>("/home/nvorkov/CLionProjects/3DVision/cmake-build-debug/RecordPointCloud/DepthPoints.ply", *cloud) == -1)
    {
        PCL_ERROR("Couldn't read file\n");
        return -1;
    }

    pcl::visualization::CloudViewer viewer("Cloud Viewer");
    viewer.showCloud(cloud);

    while (!viewer.wasStopped())
    {
    }

    return 0;
}
