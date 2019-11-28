#ifndef QTGUI2OPENCV_H
#define QTGUI2OPENCV_H
#include <opencv\cv.hpp>
#include <opencv\cxcore.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <QtDebug>
#include <QDir>
#include <QImage>
#include <QPixmap>

class QtGUI2Opencv
{
public:
    QtGUI2Opencv();
    // convert an iplimage to Qimage
    QImage IlpImage2QImage(const IplImage* img);
     // convert mat to qimage
    QImage Mat2QImage(cv::Mat &);
     // convert qimage to mat
    cv::Mat QImage2CvMat(const QImage & inImage, bool inCloneImageData=true);
    // convert pixmap to mat
    inline cv::Mat QPixmap2CvMat(const QPixmap &inPixmap, bool inCloneImageData=true)
    {
        return QImage2CvMat((inPixmap.toImage()), inCloneImageData);
    }
    // convert qimage to pixmap
    inline QPixmap QImage2Pixmap(const QImage image)
    {
      QPixmap monPixmap;
      monPixmap=QPixmap::fromImage(image);
      return monPixmap;
    }
};

#endif // QTGUI2OPENCV_H
