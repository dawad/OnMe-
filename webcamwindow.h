#ifndef WEBCAMWINDOW_H
#define WEBCAMWINDOW_H

#include <QtWidgets>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include "mediafiles.h"


class WebCamWindow: public QWidget
{
    Q_OBJECT


public:
    WebCamWindow(QWidget *parent = 0);
     ~WebCamWindow();
     void displayImage();
     void track();

 private slots:
     void aquire();
     void startWebCam();


 private:
     int frameWidth_;
     int frameHeight_;
     int templateWidth_;
     int templateHeight_;
     QPushButton *OpenCameraButton_;
     QCheckBox *trackCheckBox_;
     QTimer *timer_;
     QLabel *label_;
     cv::Mat image_;
     cv::Mat imgRoi_;
     cv::VideoCapture *webcam_;
     std::vector<cv::KeyPoint> keypt_reference;
     cv::Mat desc_ref;
     MediaFiles medfile= MediaFiles();
};

#endif // WEBCAMWINDOW_H
