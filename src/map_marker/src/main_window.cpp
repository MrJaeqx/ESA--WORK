#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include <algorithm>
#include "../include/map_marker/main_window.hpp"

#include <QDebug>


#include "geometry_msgs/Pose.h"

//extern "C" {
//#include "yaml.h"
//}

using namespace Qt;

namespace map_marker {

	MainWindow::MainWindow(int argc, char** argv, QWidget *parent) : QMainWindow(parent), qnode(argc,argv) {
		ui.setupUi(this);
		qnode.init();

		/* HOEFT NIET
		QObject::connect(ui.btnLoadYaml, SIGNAL(clicked(bool)), this, SLOT(on_btnLoadYaml_clicked()));
		QObject::connect(ui.btnLoadMap, SIGNAL(clicked(bool)), this, SLOT(on_btnLoadMap_clicked()));
		QObject::connect(ui.btnWriteYaml, SIGNAL(clicked(bool)), this, SLOT(on_btnWriteYaml_clicked()));
		QObject::connect(ui.btnClearYaml, SIGNAL(clicked(bool)), this, SLOT(on_btnClearYaml_clicked()));
		QObject::connect(ui.btnAddCurrentPose, SIGNAL(clicked(bool)), this, SLOT(on_btnAddCurrentPose_clicked()));
		QObject::connect(ui.btnAddCustomPose, SIGNAL(clicked(bool)), this, SLOT(on_btnAddCustomPose_clicked()));
		*/


		QString url = "/home/viki/git/ESA-PROJ/maps/legomap3-cropped.pgm";
	    QPixmap mapImg(url);

	    // Ttable editing
	    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	    ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	    
	    // Create map image
	    lblMapImage = new ClickableLabel(this);
	    lblMapImage->setAlignment(Qt::AlignBottom | Qt::AlignRight);
	    lblMapImage->setGeometry(QRect(0,0,992,992));
	    lblMapImage->setPixmap(mapImg);

	    QObject::connect(lblMapImage, SIGNAL(clicked(QPoint)), this, SLOT(lblMapImage_clicked(QPoint)));

	    // Set validator for input fields
	    ui.inpCustomX->setValidator(new QDoubleValidator(-100, 100, 5, ui.inpCustomX));
	    ui.inpCustomY->setValidator(new QDoubleValidator(-100, 100, 5, ui.inpCustomY));
	    ui.inpCustomAngle->setValidator(new QDoubleValidator(0, 360, 5, ui.inpCustomAngle));

	    Marker m(1.0, 2.0, 40.0, Navigation);
		AddMarker(m);

	    UpdateTable();
	}

	MainWindow::~MainWindow() {

	}

	void MainWindow::lblMapImage_clicked(QPoint a) {

		QString x = QString::number(a.x());
		QString y = QString::number(a.y());
		ui.inpCustomX->setText(x);
		ui.inpCustomY->setText(y);
	}

	void MainWindow::on_btnLoadYaml_clicked() {

	}

	void MainWindow::on_btnLoadMap_clicked() {

	}

	void MainWindow::on_btnWriteYaml_clicked() {

	}

	void MainWindow::on_btnClearYaml_clicked() {

	}

	void MainWindow::on_btnAddCurrentPose_clicked() {
		geometry_msgs::Pose pos = qnode.GetRobotPosition();

		MarkerType type;
		if(ui.radioNav->isChecked()) {
			type = Navigation;
		} else if (ui.radioWorkspace->isChecked()) {
			type = Workspace;
		}

	    AddMarker(Marker(pos, type));
		UpdateTable();
	}

	void MainWindow::on_btnAddCustomPose_clicked() {
		double x = ui.inpCustomX->text().toDouble();
		double y = ui.inpCustomY->text().toDouble();
		double angle = ui.inpCustomAngle->text().toDouble();
		
		MarkerType type;
		if(ui.radioNav->isChecked()) {
			type = Navigation;
		} else if (ui.radioWorkspace->isChecked()) {
			type = Workspace;
		}
		
	    AddMarker(Marker(x, y, angle, type));
		UpdateTable();
	}

	void MainWindow::on_btnMoveRobot_clicked() {
		int index = GetSelectedMarker();
		if(index == -1) return; // Nothing selected
		Marker * m = &markers[index];
		qnode.MoveRobotToPose(m->GetPose());
	}

	void MainWindow::on_btnRemoveMarker_clicked() {
		int index = GetSelectedMarker();
		if(index == -1) return; // Nothing selected
		markers.erase(markers.begin()+index);
		UpdateTable();
	}

	int MainWindow::GetSelectedMarker() {
		int j = -1;
	    QModelIndexList indexes = ui.tableWidget->selectionModel()->selectedRows();

	 	for (int i = 0; i < indexes.count(); ++i)
	 	{    
		 	j = indexes.at(i).row();
		}

		return j;
	}

	void MainWindow::AddMarker(Marker marker) {
		markers.push_back(marker);
	}

	void MainWindow::UpdateTable() {
		ui.tableWidget->setRowCount(0);
		for(int i=0; i < markers.size(); i++) {
			ui.tableWidget->insertRow ( ui.tableWidget->rowCount() );
			ui.tableWidget->setItem(ui.tableWidget->rowCount()-1, 0, new QTableWidgetItem(QString::fromStdString(markers[i].GetTypeStr())));
			ui.tableWidget->setItem(ui.tableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(markers[i].GetX())));
			ui.tableWidget->setItem(ui.tableWidget->rowCount()-1, 2, new QTableWidgetItem(QString::number(markers[i].GetY())));
			ui.tableWidget->setItem(ui.tableWidget->rowCount()-1, 3, new QTableWidgetItem(QString::number(markers[i].GetAngle())));
		}
	}


//void MapMarker::on_btnLoadYaml_clicked()
//{
//    FILE *fh = fopen("../maps/legomap-cropped.yaml", "r");
//    yaml_parser_t parser;
//yaml_token_t  token;   /* new variable */
//    // Initialize parser
//    if(!yaml_parser_initialize(&parser))
//    {
//    fputs("Failed to initialize parser!\n", stderr);
//    }
//    if(fh == NULL)
//    {
//    fputs("Failed to open file!\n", stderr);
//    }

//    // Set input file
//    yaml_parser_set_input_file(&parser, fh);

//    // CODE HERE
//    do {
//        yaml_parser_scan(&parser, &token);
//        switch(token.type)
//        {
//        /* Stream start/end */
//        case YAML_STREAM_START_TOKEN: puts("STREAM START"); break;
//        case YAML_STREAM_END_TOKEN:   puts("STREAM END");   break;
//        /* Token types (read before actual token) */
//        case YAML_KEY_TOKEN:   printf("(Key token)   "); break;
//        case YAML_VALUE_TOKEN: printf("(Value token) "); break;
//        /* Block delimeters */
//        case YAML_BLOCK_SEQUENCE_START_TOKEN: puts("<b>Start Block (Sequence)</b>"); break;
//        case YAML_BLOCK_ENTRY_TOKEN:          puts("<b>Start Block (Entry)</b>");    break;
//        case YAML_BLOCK_END_TOKEN:            puts("<b>End block</b>");              break;
//        /* Data */
//        case YAML_BLOCK_MAPPING_START_TOKEN:  puts("[Block mapping]");            break;
//        case YAML_SCALAR_TOKEN:  printf("scalar %s \n", token.data.scalar.value); break;
//        /* Others */
//        default:
//          printf("Got token of type %d\n", token.type);
//        }
//        if(token.type != YAML_STREAM_END_TOKEN)
//          yaml_token_delete(&token);
//      } while(token.type != YAML_STREAM_END_TOKEN);
//      yaml_token_delete(&token);
//    // Cleanup
//    yaml_parser_delete(&parser);
//    fclose(fh);
//}

}