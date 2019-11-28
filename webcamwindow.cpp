#include <QtWidgets>
#include <ctime>
#include <algorithm>
#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include <opencv2\core\core.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2/line_descriptor.hpp>
#include <QDir>
#include <QMessageBox>
#include <QPoint>
#include "webcamwindow.h"

using namespace cv;

WebCamWindow::WebCamWindow(QWidget *parent)
  : QWidget(parent)
{

    frameWidth_=320;
    frameHeight_=240;
    templateWidth_=50;
    templateHeight_=50;

    OpenCameraButton_=new QPushButton(tr("Demarrer aquisition"));
    label_ = new QLabel(tr("Image"));
    trackCheckBox_=new QCheckBox(tr(" Tracking"));

    connect(OpenCameraButton_,SIGNAL(clicked(bool)), this, SLOT(startWebCam()));
    QHBoxLayout * hl=new QHBoxLayout;
    hl->addWidget(OpenCameraButton_);
    hl->addWidget(trackCheckBox_);

    QVBoxLayout * layout=new QVBoxLayout;
    layout->addWidget(label_);
    layout->addLayout(hl);

    setLayout(layout);
    setWindowTitle(tr("Track Webcam"));
    medfile.read_csv_file("C:/Users/douni/Desktop/dounia/captures/captures/04-listes-soldats-morts/Test2/test3-selectionDesPoints/vote(sz10,seuil20).csv",keypt_reference);
    medfile.search_descriptor("C:/Users/douni/Desktop/dounia/captures/captures/04-listes-soldats-morts/Test2/refKeypoints.yml",keypt_reference, desc_ref);

    timer_=new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(aquire()));
}
WebCamWindow::~WebCamWindow()
{
    delete webcam_;
}
void WebCamWindow::displayImage()
{
    if (image_.data!=NULL)
    {
        cv::flip(image_,image_,1); // process the image_
        // change color channel ordering
        cv::cvtColor(image_,image_,COLOR_BGR2RGB);
        // Qt image_
        QImage img= QImage((const unsigned char*)(image_.data),
                           image_.cols,image_.rows,QImage::Format_RGB888);
        // display on label_
        label_->setPixmap(QPixmap::fromImage(img));
        // resize the label_ to fit the image_
        label_->resize(label_->pixmap()->size());
    }
    else QMessageBox(QMessageBox::Critical,tr("Error"),tr("Image is void")).exec();
}

void WebCamWindow::aquire()
{
    if(webcam_->isOpened())
    {
         if(webcam_->read(image_))
         {
            if(trackCheckBox_->isChecked()) track();
             displayImage();

         }
    }

}

void WebCamWindow::startWebCam()
{
    if(!timer_->isActive())
    {
        webcam_=new cv::VideoCapture(0);
        if(!webcam_->isOpened())
        {
           qDebug()<<"cannot open the video cam";
        }
        webcam_->set(CV_CAP_PROP_FRAME_WIDTH,frameWidth_);
        webcam_->set(CV_CAP_PROP_FRAME_HEIGHT,frameHeight_);
        qDebug()<<"width: "<<webcam_->get(CV_CAP_PROP_FRAME_WIDTH);
        qDebug()<<"height: "<<webcam_->get(CV_CAP_PROP_FRAME_HEIGHT);

        timer_->start(20);
        OpenCameraButton_->setText(tr("Arreter aquisition"));
    }
    else
    {
        timer_->stop();
        delete webcam_;
        webcam_=0;
        OpenCameraButton_->setText(tr("Demarrer aquisition"));
    }
}

void WebCamWindow::track()
{

    std::vector<cv::KeyPoint> keypt_query;
    std::vector<cv::Point2f> converted_scenePts_Test;
    std::vector< cv::DMatch > matches, temp_Ransac_matches;
    cv::Mat desc_query;
    cv::Mat Hmask,Hmask_int;
    int descriptor_type_test=5; // MLDB
    int descriptor_SIZE_test=0; //full size
    int descriptor_channels_test=3; // number of channels in the descriptor
    float threshold_test=0.0005f; // detector response to accept pt
    int nOctaves_test = 2; // max octave evolution of the imag
    int nOctaveLayers_test = 2;
    float soffset_test =1.3f;
    cv::Ptr<cv::AKAZE> detector_test= cv::AKAZE::create(descriptor_type_test,descriptor_SIZE_test, descriptor_channels_test,threshold_test,nOctaves_test, nOctaveLayers_test,soffset_test, cv::KAZE::DIFF_PM_G1);


    /* * Brute Force **/
     cv::Ptr<cv::DescriptorMatcher> matcher=cv::DescriptorMatcher::create("BruteForce-Hamming");

    detector_test->detectAndCompute(image_,cv::Mat(), keypt_query, desc_query);

#pragma omp parallel
    matcher->match(desc_query, desc_ref, matches);

    std::vector<cv::Point2f> obj;
    std::vector<cv::Point2f> scene;
    //Inserting the matched keyPoints of query and reference into obj and scene respectively

     for (size_t j = 0; j < matches.size(); j++)
     {
        obj.push_back(keypt_query[matches[j].queryIdx].pt);
        scene.push_back(keypt_reference[matches[j].trainIdx].pt);
     }
     /*findHomography fun returns a 3-by-3 transformation matrix
       Hmask returns the ransac outliers and inliers information as 0 and 1 value*/

    cv::Mat H = findHomography(obj, scene, CV_RANSAC, 20, Hmask);
     //converting Hmask into int type
     Hmask.convertTo(Hmask_int, CV_32SC1);
     /*Stores the matches of only ransac inliers*/

     for (int j = 0; j<Hmask_int.rows; j++) {
        if (Hmask_int.at<int>(j, 0) == 1) {
          temp_Ransac_matches.push_back(matches[j]);

        }
      }
      std::vector<cv::Point2f> scenePts (4);
      scenePts[0]=cv::Point2f(1068,2308); scenePts[3]= cv::Point2f(1437, 2308);
      scenePts[1]= cv::Point2f(1068, 2352); scenePts[2]= cv::Point2f(1437, 2352);

      cv::perspectiveTransform(scenePts, converted_scenePts_Test, H.inv());

      cv::line(image_, converted_scenePts_Test[0], converted_scenePts_Test[1],cv::Scalar(0,255,0),4);
      cv::line(image_, converted_scenePts_Test[1], converted_scenePts_Test[2],cv::Scalar(0,255,0),4);
      cv::line(image_, converted_scenePts_Test[2], converted_scenePts_Test[3],cv::Scalar(0,255,0),4);
      cv::line(image_, converted_scenePts_Test[3], converted_scenePts_Test[0],cv::Scalar(0,255,0),4);
}
