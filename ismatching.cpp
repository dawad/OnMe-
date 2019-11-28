#include "ismatching.h"
#include<opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <QStringList>
IsMatching::IsMatching()
{

}
IsMatching::~IsMatching()
{
}
/*Takes an vector of Points and calculate the Euledian distance between all of its pairs and returns its total sum*/
double IsMatching::PointDistanceDistribution(std::vector<cv::Point2f>& xy_vector)
{
    double sum= 0.0;
    for (unsigned int i = 0; i  < xy_vector.size() - 1; i++) {
        for (unsigned int k = i + 1; k < xy_vector.size(); k++) {
            sum = sum + norm(xy_vector[i] - xy_vector[k]);
        }
    }
    return sum;
}
/*gives the maximum no of pixels possibe to use in a patch in among either sides, and the number is greater than max_size then just return max_size*/
int IsMatching::largest_patch_possible(cv::Mat & img, cv::Point2f pt)
{
    int x = round(pt.x);
    int y = round(pt.y);

    /*is the */
    int max_size = 5;
    int image_rows = img.rows;
    int image_cols = img.cols;

    int lx = x;		//cols left of x;
    int ty = y;		//rows top of y;
    int rx = image_cols - x - 1;
    int by = image_rows - y - 1;

    int size = (std::min(std::min(lx, ty), std::min(rx, by)));
    if (size <= max_size) {
        return size;
    }
    else {
        return max_size;
    }
}
/*Give the Average pixel value in the point around the patch*/
double IsMatching::normalized_pix_value(cv::Mat & img, cv::Point2f pt, int patch_size)
{
    /*Patch size will always be a odd number*/
    double sum = 0.0;
    double no_of_pixels = patch_size*patch_size;

    cv::Point2f Corner(pt.x - (patch_size / 2), pt.y - (patch_size / 2));

    for (int i = Corner.x; i<Corner.x + patch_size; i++) {
        for (int k = Corner.y; k<Corner.y + patch_size; k++) {
            sum = sum + img.at<uchar>(cv::Point2f(i, k));
        }
    }
    return (double)sum / no_of_pixels;
}
/*display keypoints in an image with imshow */
void IsMatching::Show_keypoints_image(cv::Mat & image, std::vector<cv::KeyPoint>& Interest_pt)
{
    for (unsigned int i = 0; i < Interest_pt.size(); i++)
    {
        const cv::KeyPoint& kp = Interest_pt[i];
        cv::circle(image, cv::Point(kp.pt.x, kp.pt.y), 10, cv::Scalar(255, 0, 0, 255));
    }
    cv::namedWindow("detected keypoints ", CV_WINDOW_NORMAL);
    cv::imshow("detected keypoints ", image);
}
 /* choosse a detector determined by a string*/
cv::Ptr<cv::FeatureDetector>  IsMatching::choisir_detector(QString type)
{
 QStringList detect_type;
 detect_type<<
   "STAR"<<
    "ORB"<<
   "GFTT"<<
   "SURF"<<
   "BRISK";
  switch(detect_type.indexOf(type))
  {
   case 0:
  {
     cv::Ptr<cv::FeatureDetector> detector = cv::xfeatures2d::StarDetector::create();
    return detector;
  }
  case 1:
  {
      int nfeatures = 500;
          float scaleFactor = 1.2f;
          int nlevels = 8;
          int edgeThreshold = 15; // Changed default (31);
          int firstLevel = 0;
          int WTA_K = 2;
          int scoreType = cv::ORB::HARRIS_SCORE;
          int patchSize = 31;
          int fastThreshold = 20;
          cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create(
              nfeatures,
              scaleFactor,
              nlevels,
              edgeThreshold,
              firstLevel,
              WTA_K,
              scoreType,
              patchSize,
              fastThreshold);
          return detector;
  }
  case 2:
  {
     cv::Ptr<cv::FeatureDetector> detector=  cv::GFTTDetector::create();
     return detector;
  }
  case 3:
  {
      double minHessian = 1000.0;
      int nOctaves = 4;
      int nOctaveLayers = 2;
      static cv::Ptr<cv::xfeatures2d::SURF> detector;
      detector= cv::xfeatures2d::SURF::create(minHessian, nOctaves, nOctaveLayers);
      return detector;
  }
    case 4:
  {
      int Threshold = 60;
      int Octaves = 4;
      float PatternScales = 1.0;
      cv::Ptr<cv::FeatureDetector> detector= cv::BRISK::create(Threshold,Octaves,PatternScales);
      return detector;
  }

    default:
      cv::Ptr<cv::FeatureDetector> detector = cv::FastFeatureDetector::create(50);
      return detector;
  }
}
 /* choose a descriptor determined by a string */
 cv::Ptr<cv::DescriptorExtractor> IsMatching::choisir_descripteur(QString type)
 {
     QStringList desc_type;
     desc_type<<
     "SURF" <<
     "BRISK"<<
     "FREAK"<<
      "ORB";
     switch(desc_type.indexOf(type))
     {
     case 0:
     {
         cv::Ptr<cv::DescriptorExtractor> descriptor = cv::xfeatures2d::SurfDescriptorExtractor::create(1000);
        return descriptor;
     }
     case 1:
     {
         int Threshold = 60;
         int Octaves = 4;
         float PatternScales = 1.0;
         cv::Ptr<cv::DescriptorExtractor> descriptor= cv::BRISK::create(Threshold,Octaves,PatternScales);
         return descriptor;
     }
     case 2:
     {
        cv::Ptr<cv::DescriptorExtractor> descriptor=cv::xfeatures2d::FREAK::create();
        return descriptor;
     }
     case 3:
     {
          cv::Ptr<cv::DescriptorExtractor> descriptor=  cv::ORB::create();
          return descriptor;
     }
     default:
     {
        cv::Ptr<cv::DescriptorExtractor> descriptor = cv::xfeatures2d::BriefDescriptorExtractor::create();
        return descriptor;
     }
     }
 }
   // compute distance between two vector of points
void  IsMatching::compute_distance(std::vector<cv::Point2f> & ref_pts, std::vector<cv::Point2f> & transformed_pts, std::vector<double> & distance )
 {

     if(!(ref_pts.size()==transformed_pts.size()))
         std::cout << " the two vectors are not equal!"<<std::endl;
     for( size_t i=0 ; i< ref_pts.size() &&i<transformed_pts.size() ; i++)
     {

          cv::Point2f pt_ref=ref_pts.at(i);
          cv::Point2f transf_pt= transformed_pts.at(i);
          double diff=std::sqrt( std::pow((pt_ref.x-transf_pt.x),2)+std::pow((pt_ref.y-transf_pt.y),2) );
          distance.push_back(diff);

     }

 }
