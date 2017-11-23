#ifndef map_marker_MAIN_WINDOW_H
#define map_marker_MAIN_WINDOW_H

#include "ui_MainWindow.h"
#include "Qnode.hpp"
#include <vector>
#include "Marker.hpp"
#include "ClickableLabel.hpp"
#include "YamlParser.hpp"
#include "MapConfig.hpp"
#include "YamlWriter.hpp"

#include "geometry_msgs/Pose.h"

namespace map_marker {
	class MainWindow : public QMainWindow {
		Q_OBJECT

	public Q_SLOTS:
		void lblMapImage_clicked(QPoint);
		void on_btnLoadYaml_clicked();
		void on_btnLoadMap_clicked();
		void on_btnWriteYaml_clicked();
		void on_btnClearAllMarkers_clicked();
		void on_btnAddCurrentPose_clicked();
		void on_btnAddCustomPose_clicked();
		void on_btnMoveRobot_clicked();
		void on_btnRemoveMarker_clicked();
		void on_btnPanic_clicked();
		void on_btnMoveMarkerDown_clicked();
		void on_btnMoveMarkerUp_clicked();
		void on_btnLoadMarkersYaml_clicked();
		void UpdateRobotPose();
		void SelectionIsChanged();
		

	public:
		MainWindow(int argc, char** argv, QWidget *parent = 0);
		~MainWindow();

	private:
		void AddMarker(Marker marker);
		void MoveMarkerUp(int selectedMarker);
		void MoveMarkerDown(int selectedMarker);
		void UpdateTable();
		void FillMarkerList(std::vector<KeyDataPair> data);
		void ToggleInterface(bool b);
		void UpdateWindow();
		QPointF Rotatestuff(QPoint center, double x, double y, double angle);
			
		double Deg2Rad(double deg);
		double Rad2Deg(double rad);

		int GetSelectedMarker();
		int ConvertRobotToPixel(double a);
		double ConvertPixelToRobot(int a);

		Ui::MapMarker ui;
		QNode qnode;
		std::vector<Marker> markers;
		ClickableLabel *lblMapImage;
		YamlParser yaml;
		YamlWriter yamlWriter;
		MapConfig mapConfig;
		QPixmap *map;
		geometry_msgs::Pose robotPose;
		QSize robotSize;

	protected:
    	void paintEvent(QPaintEvent *event);
    	void drawMarkers(QPainter *qp);
	};
}

#endif // map_marker_MAIN_WINDOW_H
