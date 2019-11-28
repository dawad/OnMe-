#include "dtgui2opencv.h"
#include "mediafiles.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>
#include <Windows.h>
#include <sstream>
#include <QImage>
#include <QPixmap>
QtGUI2Opencv::QtGUI2Opencv()
{

}
 // convert an iplimage to Qimage
QImage QtGUI2Opencv::IlpImage2QImage(const IplImage *img)
{

QImage qtemp;
if(img && cvGetSize(img).width>0)
{
    int x;
    int y;
    char* data=img->imageData;
    qtemp=QImage(img->width, img->height, QImage::Format_RGB32);
    for(y=0; y< img->height;y++, data+=img->widthStep)
        for(x=0; x<img->width; x++)
        {
            uint *p=(uint *)qtemp.scanLine(y)+x;
            *p=qRgb(data[x * img->nChannels+2], data[x * img->nChannels+1], data[x * img->nChannels]);
        }

}
return qtemp;
}
// convert mat to qimage
QImage QtGUI2Opencv::Mat2QImage(cv::Mat & img)
{
    // 8-bits unsigned No. OF Channels=1
 switch(img.type())
 {

  //8-bit, 4 channels
 case CV_8UC4:
 {
     QImage image(img.data, img.cols, img.rows,img.step, QImage::Format_RGB32);
     return image;
 }
 // 8-bit 3 channels
 case CV_8UC3:
 {
     QImage image(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
     return image;
 }
 // 8-bit, 1channels
 case CV_8UC1:
 {
     //Set the color table (used to translate colour indexes to qRgb values)
     QVector<QRgb> colorTable;
     for(int i=0; i<256; i++)
         colorTable.push_back(qRgb(i,i,i));
     //Copy input Mat
     const uchar* qImageBuffer=(const uchar*)img.data;
     //Create QImage with the same dimension as input
     QImage image(qImageBuffer, img.cols, img.rows, img.step, QImage::Format_Indexed8);
     image.setColorTable(colorTable);
     return image;
 }
 default:
     qWarning() << " Mat2QImage - Mat type not handled in switch "<< img.type();
     break;
 }
 return QImage();
}
   // convert qimage to mat
cv::Mat QtGUI2Opencv::QImage2CvMat(const QImage &inImage, bool inCloneImageData)
{
 switch(inImage.format())
 {
    //8-bit, 4 channel
    case QImage::Format_RGB32:
    {
     cv::Mat mat(inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine());
     return (inCloneImageData ? mat.clone():mat);

    }

    //8-bit, 3 channel
    case QImage::Format_RGB888:
    {
     if(!inCloneImageData)
         qWarning() <<"QImage2CvMat()-Conversion requires cloning since we use a temporary QImage";
     QImage swapped=inImage.rgbSwapped();
     return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
    }

    //8-bit, 1 channel
    case QImage::Format_Indexed8:
    {
     cv::Mat mat(inImage.height(), inImage.width(), CV_8UC1, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine());

     return (inCloneImageData ? mat.clone() : mat);
    }

    default:
     qWarning() << "QImage2CvMat() - QImage format not handled in switch: " << inImage.format();
     break;
 }
 return cv::Mat();
}
