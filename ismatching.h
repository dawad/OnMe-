#ifndef ISMATCHING_H
#define ISMATCHING_H
/*

Usage: ./IsMatching   QueryImage ReferenceImage


Code Explanation:

Use Combination of techniques to determine the MATCH or NON-MATCh between two images

Steps:
1) Find the keyPoints, Descriptors and then Match between them.
2) Use findHomography Function to get Ransac inlier-outlier information
3) Find the percentage of matched points below threshold value
4) Take only the ransac output matches and find the distribution of the matched points in corresponding Images.
5) Calculate the Intensity difference values around a mathed Points.


*/
#include<QString>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include<opencv\cv.h>
#include <opencv\cxcore.h>
#include<opencv\highgui.h>

#include <opencv2\features2d\features2d.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\xfeatures2d\nonfree.hpp>

class IsMatching
{
public:
    IsMatching();
    ~IsMatching();
    /*Takes an vector of Points and calculate the Euledian distance between all of its pairs and returns its total sum*/
    double PointDistanceDistribution(std::vector<cv::Point2f>&);
    /*gives the maximum no of pixels possibe to use in a patch in among either sides, and the number is greater than max_size then just return max_size*/
    int  largest_patch_possible(cv::Mat &, cv::Point2f);
      /*Give the Average pixel value in the point around the patch*/
    double normalized_pix_value(cv::Mat &, cv::Point2f, int);
    /*display keypoints in an image with imshow */
    void Show_keypoints_image(cv::Mat&, std::vector<cv::KeyPoint> &);
     /* choosse a detector determined by a string*/
    cv::Ptr<cv::FeatureDetector>  choisir_detector(QString type);
      /* choose a descriptor determined by a string */
    cv::Ptr<cv::DescriptorExtractor> choisir_descripteur(QString type);
      // compute distance between two vector of points
    void compute_distance(std::vector<cv::Point2f> &, std::vector<cv::Point2f> &, std::vector<double> &);
    //after detecting keypoints in a part of image Whose its borders obtained in max_point, this functions return  the xy coordinates of these keypoints in the original image
    void convert_to_trueXY( std::vector<std::vector<cv::KeyPoint> > & pt_ref, std::vector < cv::KeyPoint > & keypt,std::vector<cv::Point> & max_point)
    {

          for (unsigned int i=0 ; i< pt_ref.size() ; i++)

              for (unsigned int j=0; j < pt_ref.at(i).size(); j++)
              {
                  cv::KeyPoint point= pt_ref[i][j];
                  keypt.push_back(cv::KeyPoint(point.pt.x+max_point[i].x,point.pt.y-max_point[i].y, point.size,-1,point.response, point.octave, point.class_id));

              }



    }
    //return the true the xy coordinates in an image for keypoints detected in part of image of size smallSize
    void convert_to_trueXY( std::vector<std::vector<cv::KeyPoint> > & pt_ref, std::vector < cv::KeyPoint > & keypt, cv::Size & smallSize)
    {
          int x_img =0;
          for (unsigned int i=0 ; i< pt_ref.size() ; i++)
          {
              for (unsigned int j=0; j < pt_ref.at(i).size(); j++)
              {
                  cv::KeyPoint point= pt_ref[i][j];
                  keypt.push_back(cv::KeyPoint(point.pt.x+x_img,point.pt.y, point.size,-1,point.response, point.octave, point.class_id));

              }
              x_img+= smallSize.width;
          }

    }
    //return the true the xy coordinates in an image of size orig_size for keypoints detected in part of image of size smallSize
    void convert_to_trueXY( std::vector<std::vector<cv::KeyPoint> > & pt_ref, std::vector < cv::KeyPoint > & keypt, cv::Size & smallSize, cv::Size & orig_size)
    {
    size_t i=0;

      while(i< pt_ref.size())
        {
         switch(i)
         {
         case 0:
         {
             for (unsigned int j=0; j < pt_ref.at(i).size(); j++)
             {
                 keypt.push_back(pt_ref[i][j]);
             }
          i++;
          break;
         }
         case 1:
         {
             for (unsigned int j=0; j < pt_ref.at(i).size(); j++)
             {
                 cv::KeyPoint point= pt_ref[i][j];
                 keypt.push_back(cv::KeyPoint(point.pt.x,point.pt.y+smallSize.height,point.size,-1,point.response, point.octave, point.class_id));

             }
              i++;
              break;

         }
         case 2:
         {
             for (unsigned int j=0; j < pt_ref.at(i).size(); j++)
             {
                 cv::KeyPoint point= pt_ref[i][j];
                 keypt.push_back(cv::KeyPoint(point.pt.x+smallSize.width,point.pt.y,point.size,-1,point.response, point.octave, point.class_id));

             }
              i++;
              break;
         }
         case 3:
         {
             for (unsigned int j=0; j < pt_ref.at(i).size(); j++)
             {
                 cv::KeyPoint point= pt_ref[i][j];
                 keypt.push_back(cv::KeyPoint(point.pt.x+smallSize.width,point.pt.y+smallSize.height,point.size,-1,point.response, point.octave, point.class_id));

             }
              i++;
              break;
         }


         }
        }
    }
   // convert vector of keypoints to vector of points2f
  void convert_Pointef(std::vector<cv::KeyPoint> & keypt , std::vector<cv::Point2f> & converted)
  {
     for (int i =0; i<keypt.size(); i++)
     {
       converted.push_back(keypt.at(i).pt);
     }
  }
};

#endif // ISMATCHING_H
