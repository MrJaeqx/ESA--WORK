#ifndef map_marker_QNODE_HPP_
#define map_marker_QNODE_HPP_

#include <ros/ros.h>
#include <string>
#include <QThread>
#include <QMutex>
#include <QStringListModel>
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Quaternion.h"
#include "nav_msgs/OccupancyGrid.h"

namespace map_marker {

class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool Init();
	bool Init(const std::string &master_url, const std::string &host_url);
	void run();
	geometry_msgs::Pose GetRobotPosition();
	void MoveRobotToPose(geometry_msgs::Pose pos);
	void Panic();
	void DrawLine(const int x1, const int y1, const int x2, const int y2);
	nav_msgs::OccupancyGrid GetKeepOutMap();
	void keepoutMapCallback(const nav_msgs::OccupancyGrid::ConstPtr& grid);

Q_SIGNALS:
    void RosShutdown();
    void RobotPosUpdated();

private:
	int init_argc;
	char** init_argv;
	ros::Publisher pubPose;
	ros::Publisher pubEmergency;
	ros::Publisher pubNoGoLine;
	ros::Subscriber subMap;
    QStringListModel logging_model;
    geometry_msgs::Pose pose;
    nav_msgs::OccupancyGrid keepoutMap;
    
};

}

#endif