#if defined(Q_OS_WIN32)
    #include "ShellScalingApi.h"
#endif
#include "editeurpoints.h"
#include <QApplication>
#include "mediafiles.h"
#include "ismatching.h"
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
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /**Parametres**/

    MediaFiles medfile= MediaFiles();
    IsMatching match= IsMatching();
    cv::Mat reference;
    cv::Mat frame;
    cv::Mat desc_ref, desc_query;// desc_frame;
    cv::Mat Hmask;
    cv::Mat Hmask_int;
    std::vector<cv::KeyPoint> keypt_reference;
    std::vector<cv::KeyPoint> keypt_query;
    std::vector<cv::Point2f> obj;
    std::vector<cv::Point2f> scene;
    std::vector<int> vote ;
    std::vector< cv::DMatch > temp_Ransac_matches, matches;
    QStringList parameter;
    std::ofstream out;
    std::string nomfichier="C:/Users/douni/Desktop/dounia/vote3.csv";
    out.open(nomfichier.c_str(),std::ofstream::out | std::ofstream::app);
    if(!out)
        std::cout<< "\n Error: file could not be opened !";
   // out <<"SZ=150"<<";"<<"S=20"<<"\n";
 //   out<<"image"<<";"<<"resolution"<<";"<<"number keypoint "<<";"<<"query run time detection"<<";"<<"query run time ransac"<<";"<<"nombres des pts matches"<<";"<<"min distance"<<";"<<"max distance"<<";"<<"distance"<<"\n";
   out<<"point_x"<<";"<<"point_y"<<";"<<"class_di"<<";"<<"vote"<<"\n";
    /*detect keypoints pour query image*/
     /*
     * AKAZE*/
  int descriptor_type=5; // MLDB
    int descriptor_SIZE=0; //full size
    int descriptor_channels=3; // number of channels in the descriptor
    float threshold=0.0003f; // detector response to accept pt
    int nOctaves = 5; // max octave evolution of the image
    int nOctaveLayers =5;
    float soffset=1.8f;
    cv::Ptr<cv::AKAZE> detector= cv::AKAZE::create(descriptor_type,descriptor_SIZE, descriptor_channels,threshold,nOctaves, nOctaveLayers, soffset);
    const clock_t begin_t = clock();
 /*  medfile.read_csv_file("C:/Users/douni/Desktop/dounia/vote(sz10,seuil20).csv",keypt_reference);
   medfile.search_descriptor("C:/Users/douni/Desktop/dounia/ref_keypoints2.yml",keypt_reference, desc_ref);
   size_t total_pts=keypt_reference.size();*/

    int descriptor_type_test=5; // MLDB
    int descriptor_SIZE_test=0; //full size
    int descriptor_channels_test=3; // number of channels in the descriptor
    float threshold_test=0.0005f; // detector response to accept pt
    int nOctaves_test = 2; // max octave evolution of the imag
    int nOctaveLayers_test = 2;
    float soffset_test =1.3f;
    cv::Ptr<cv::AKAZE> detector_test= cv::AKAZE::create(descriptor_type_test,descriptor_SIZE_test, descriptor_channels_test,threshold_test,nOctaves_test, nOctaveLayers_test,soffset_test, cv::KAZE::DIFF_PM_G1);


   /**FlannBasedMatcher to match descriptors, it uses the nearest neighbor search to find best match*
         */

    cv::Ptr <cv::DescriptorMatcher> flann_matcher= cv::DescriptorMatcher::create("FlannBased");

   /* * Brute Force */
    cv::Ptr<cv::DescriptorMatcher> matcher=cv::DescriptorMatcher::create("BruteForce-Hamming");

     // read the image reference
   QFileInfo reffileInfo=QString("C:/Users/douni/Desktop/dounia/imgs/WIN_20160616_12_22_57_Pro.jpg");
    reference = medfile.OpenImage(reffileInfo.filePath().toLocal8Bit().constData());
    parameter.append(reffileInfo.fileName()); // image name
    parameter.append(QString("%1x%2").arg(reference.rows).arg(reference.cols)); //resolution

  detector->detectAndCompute(reference, cv::Mat(), keypt_reference, desc_ref);
    cv::FileStorage fs("C:/Users/douni/Desktop/dounia/ref_keypoints2.yml", cv::FileStorage::WRITE);
     cv::write( fs , "keypoint_1", keypt_reference );
     cv::write( fs ,"descriptor_1", desc_ref );
    fs.release();

   vote.resize(keypt_reference.size());



     //descriptor->compute(reference,keypt_reference,desc_ref);

    parameter.append(QString::number(keypt_reference.size())); // nb des keypoints
    parameter.append(QString::number(float(clock() - begin_t) / CLOCKS_PER_SEC)); // query-run time for descriptor
    parameter.append("0");
    parameter.append("0");
    parameter.append("0");
    parameter.append("0");

 //  medfile.append_CSVfile(out,parameter);

    /*if(total_pts==0)
    {
      std::cout <<" 0 keypoints was detected!"<<std::endl;
    }*/

    std::cout << " number of keypoints " << keypt_reference.size()<<std::endl;

    // query images
    QDir dirQuery("C:/Users/douni/Desktop/dounia//imgs/TEST");
    QFileInfoList fileListQuery;
    medfile.get_list_images(dirQuery,fileListQuery );
    for(int i=0; i< fileListQuery.size(); i++)
     {
        parameter.clear();
        QFileInfo fileInfoquery=fileListQuery.at(i);
        std::cout <<" frame " <<fileInfoquery.fileName().toLocal8Bit().constData()<<std::endl;

        frame = medfile.OpenImage(fileListQuery.at(i).filePath().toLocal8Bit().constData());

        parameter.append(fileInfoquery.fileName()); // image name
        parameter.append(QString("%1x%2").arg(frame.rows).arg(frame.cols)); //resolution

        size_t total_pts_frame=0;
        cv::resize(frame,frame,cv::Size(),0.5,0.5);
        const clock_t begin = clock();
        detector_test->detectAndCompute(frame ,cv::Mat(),keypt_query, desc_query);
        total_pts_frame=keypt_query.size();
        cv::Mat draw_frame;
        cv::drawKeypoints(frame,keypt_query,draw_frame);
        cv::imwrite(QString("C:/Users/douni/Desktop/%1_keypt.jpg").arg(fileInfoquery.fileName()).toStdString(),draw_frame);
        parameter.append(QString("%1").arg(total_pts_frame,10)); // number of keypoints
        parameter.append(QString::number(float(clock() - begin) / CLOCKS_PER_SEC)); // query run time for descriptor

        const clock_t begin_match= clock();

          if(keypt_query.empty())
          {
              std::cout << " 0 zero keypoints was detected" << std::endl;
              continue;
          }
            std::cout << " number of keypoint "<<keypt_query.size() <<std::endl;
          if(desc_query.type()==CV_8U)
           {
             matcher->match(desc_query, desc_ref, matches);
           }
          else
           {
            // match the reference avec the query
             flann_matcher->match(desc_query, desc_ref, matches);
           }


          //Inserting the matched keyPoints of query and reference into obj and scene respectively
           for (size_t j = 0; j < matches.size(); j++)
           {
              obj.push_back(keypt_query[matches[j].queryIdx].pt);
              scene.push_back(keypt_reference[matches[j].trainIdx].pt);
           }
           /*findHomography fun returns a 3-by-3 transformation matrix
             Hmask returns the ransac outliers and inliers information as 0 and 1 value*/

           cv::Mat H = findHomography(obj, scene, CV_RANSAC, 20, Hmask);
           parameter.append(QString::number(float(clock() - begin_match) / CLOCKS_PER_SEC)); // query run time for RANSAC
           //converting Hmask into int type
           Hmask.convertTo(Hmask_int, CV_32SC1);
           for (int i=0; i< H.rows; i++)
           {
               std::cout << "\n";
                 for (int j=0; j< H.cols; j++)
                 {
                     std::cout << H.at<double>(i,j) << " ";
                 }
           }
            std::vector<cv::Point2f> scene_pts;
           /*Stores the matches of only ransac inliers*/
           for (int j = 0; j<Hmask_int.rows; j++) {
              if (Hmask_int.at<int>(j, 0) == 1) {
                temp_Ransac_matches.push_back(matches[j]);
                scene_pts.push_back(scene[j]);
              }
            }
           if(temp_Ransac_matches.empty())
           {
               std::cout << " zero match!!!!!" << std::endl;
               continue;
           }
           parameter.append(QString::number(temp_Ransac_matches.size())); // number of matched keypoint
          cv::Mat img_matches3;
          drawMatches(frame, keypt_query,reference, keypt_reference,
                     temp_Ransac_matches, img_matches3, cv::Scalar::all(-1), cv::Scalar::all(-1),
                     std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
          QString qpath_match=QString("C:/Users/douni/Desktop/dounia/%1_(%2).jpg").arg(fileInfoquery.fileName().toLocal8Bit().constData()).arg(temp_Ransac_matches.size());
          std::cout<< qpath_match.toLocal8Bit().constData()<<std::endl;
          cv::imwrite(qpath_match.toLocal8Bit().constData(),img_matches3);
          // apply the transformation on test
         /*  std::vector<cv::Point2f> pt_reference;
           std::vector<cv::Point2f>pt_query;
           QStringList frame_file=fileInfoquery.fileName().split(".");
           QFileInfo txtfile_ref=QString("C:/Users/douni/Desktop/dounia/images/ref_%1.txt").arg(frame_file[0]);
           std::cout << " file name : "<<txtfile_ref.fileName().toLocal8Bit().constData() <<std::endl;
           medfile.read_file(txtfile_ref.filePath(),pt_reference);
           QFileInfo txtfile_query=QString("C:/Users/douni/Desktop/dounia/images/TEst/%1.txt").arg(frame_file[0]);
           medfile.read_file(txtfile_query.filePath(),pt_query);
          /*findHomography fun returns a 3-by-3 transformation matrix
            Hmask returns the ransac outliers and inliers information as 0 and 1 value*/
           /*std::vector<cv::Point2f> converted_scenePts;
           cv::perspectiveTransform(pt_query, converted_scenePts, H);
           std::vector<double> distance;
           match.compute_distance(pt_reference,converted_scenePts, distance);
           std::cout << " min distance " << *std::min_element(distance.begin(), distance.end())<<std::endl;
           std::cout << " max distance " << *std::max_element(distance.begin(), distance.end())<<std::endl;
           parameter.append(QString::number( *std::min_element(distance.begin(), distance.end()) ));  // min distance
           parameter.append(QString::number( *std::max_element(distance.begin(), distance.end() )));  // max distance

           for(size_t i=0; i< distance.size();  i++)
                    parameter.append(QString::number(distance[i])); // distance
           cv::Mat output;
           cv::warpPerspective(frame,output, H, output.size());
           QString qpath=QString("C:/Users/douni/Desktop/dounia/%1_(%2).jpg").arg(fileInfoquery.fileName().toLocal8Bit().constData()).arg(reffileInfo.fileName().toLocal8Bit().constData());
           std::cout<< qpath.toLocal8Bit().constData()<<std::endl;
           cv::imwrite(qpath.toLocal8Bit().constData(),output);
           cv::Mat refer_draw;
           reference.copyTo(refer_draw);
           for(int j=0; j<pt_reference.size(); j++)
           {
             cv::circle(refer_draw,pt_reference[j],2,cv::Scalar(255,0,0),8);
           }
           for(int i=0; i<converted_scenePts.size();i++)
           {
           cv::circle(refer_draw, converted_scenePts[i],2,cv::Scalar(0,255,0),8);
           }

           QString qpath_res=QString("C:/Users/douni/Desktop/dounia/res_%1_(%2).jpg").arg(fileInfoquery.fileName().toLocal8Bit().constData()).arg(reffileInfo.fileName().toLocal8Bit().constData());
           std::cout<< qpath_res.toLocal8Bit().constData()<<std::endl;
           cv::imwrite(qpath_res.toLocal8Bit().constData(),refer_draw);
           medfile.append_CSVfile(out,parameter);*/

           medfile.search_keypoint_csv_file(keypt_reference,scene_pts,vote);

          frame= cv::Mat();
          keypt_query.clear();
          desc_query=cv::Mat();
          matches.clear();
          obj.clear();
          scene.clear();
          Hmask=cv::Mat();
          Hmask_int=cv::Mat();
          temp_Ransac_matches.erase(temp_Ransac_matches.begin(), temp_Ransac_matches.end());



     }
    std::vector<cv::KeyPoint> temp_ref;

         for(size_t i=0; i<keypt_reference.size(); i++)
          {
              if(vote.at(i)!=0)
              {
                  temp_ref.push_back(keypt_reference.at(i));
              }
         }
          medfile.append_CSVfile(out,keypt_reference,vote);
     out.close();
     std::cout <<" l'application est fini";




    return a.exec();
}
