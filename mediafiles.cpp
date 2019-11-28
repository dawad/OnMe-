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
#include <fstream>
#include <math.h>
MediaFiles::MediaFiles()
{

}
MediaFiles::~MediaFiles()
{
}
//http://answers.opencv.org/question/9692/mp4-reading/
//http://rsrcv.blogspot.fr/2015/03/opencv-tutorial-how-to-play-video-file.html
//http://stackoverflow.com/questions/12445955/c-c-opencv-video-processing
/*read an image and return in format Mat */
cv::Mat MediaFiles::OpenImage(std::string & filename)
{

    cv::Mat imag = cv::imread(filename, 1);
    if (!imag.data)
    {
        std::cout << "Could not open or find the image" << std::endl;
    }
    return imag;
}
cv::Mat MediaFiles::OpenImage(const char* filename)
{

    cv::Mat imag = cv::imread(filename, 1);
    if (!imag.data)
    {
        std::cout << "Could not open or find the image" << std::endl;
    }
    return imag;
}
/*read a video file ".avi", takes the file name as input and  CvCapture* as output */
void MediaFiles::readAVIVideo(std::string & filename, CvCapture* capture)
{
    capture = cvCaptureFromAVI(filename.c_str());
    IplImage* frame = cvQueryFrame(capture);

    if (!capture)
    {
        fprintf(stderr, "Cannot open AVI!\n");
    }
    cvShowImage("frame", frame);
}
/*read an MP4 video file, takes the file name as input, and VideoCapture , number of frames in the video as output*/
void MediaFiles::readMP4Video(std::string & filename, cv::VideoCapture & video, double  & totalframe, double & fps)
{
    /*Opens capture stream for video file */
    video.open(filename);
    if (!video.isOpened())
    {
        fprintf(stderr, "Cannot open MP4!\n");
    }
    /*count the total number of frame in video */
    totalframe = video.get(CV_CAP_PROP_FRAME_COUNT);
    std::cout << "Total number of frames " << totalframe << std::endl;

    /*find the frame rate of a video */
    fps = video.get(CV_CAP_PROP_FPS);
    std::cout << " Frames par second " << fps << std::endl;
    std::cout << " codec " << video.get(CV_CAP_PROP_FOURCC);

}
/* read the video file name and return the frames in folder defined by path */
void MediaFiles::ReadVideoANDExtractFrames(std::string & filename, const wchar_t * path)
{
    cv::VideoCapture videofile= cv::VideoCapture();
    double totalframes=0;
    double fps = 0;
    readMP4Video(filename, videofile, totalframes, fps);
    ExtractFrames(path, videofile, totalframes, fps);

}
CvCapture * MediaFiles::readvideo(std::string & videofile,  double & totalframe, double & fps)
{

   CvCapture * capture=cvCaptureFromFile(videofile.c_str());
    if(capture== NULL)
    {
        fprintf(stderr, "Error: Can't open video. \n");
    }
    totalframe= cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
    std::cout << "total nombre de frames "<<totalframe<<std::endl;
    fps=cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    std::cout << "frames per second "<<fps<<std::endl;
    std::cout<<" Type of Codecs "<<cvGetCaptureProperty(capture, CV_CAP_PROP_FOURCC);

   return capture;
}

void MediaFiles::ReadVideo_ExtractFrames(std::string &filename, const wchar_t *pathOfFrames)
{
    double totalframes=0;
    double fps = 0;
    IplImage* frame2= NULL;

    double  i=0.0;
    CvCapture* capture=readvideo(filename,totalframes,fps);
    IplImage* frame=cvQueryFrame(capture); // to read the properties of the frame
    double frame_echelle=totalframes/fps;
    double frame_no=0;
    while(i<totalframes)
    {
    if(frame2)
       cvCopy(frame,frame2); // copy image to allow grabbing next frame
    frame2=cvQueryFrame(capture); // read next frame
    if(!frame2) break; // if frame cannot be read, EOF so break from Loop
    if((frame_no==i)&&(frame_no < totalframes))
    {
        save_frame(frame,pathOfFrames,frame_no); //save the frame
        frame_no+=round(frame_echelle);
    }

    i++;

    }
    cvReleaseCapture(&capture);
  //  cvReleaseImage(&frame);
    cvReleaseImage(&frame2);

}


/*Extract a frame from video file, it takes the path of folder of extracted frame, the video ,  and of the frame in the video as input and frame  as output */
void MediaFiles::ExtractFrame(const wchar_t * path, cv::VideoCapture& videofile, cv::Mat frame, double & nbOfFrame)
{
    try
      {
        videofile.read(frame);
      }
    catch(cv::Exception)
    {
        std::cout << "can not open video file" << std::endl;
    }
    int totalframe = videofile.get(CV_CAP_PROP_FRAME_COUNT);
    std::cout << "Total number of frames " << totalframe << std::endl;
    cv::Mat convertedframe = cv::Mat(frame.rows, frame.cols, CV_8UC2);
    if (!frame.data)
    {
        std::cout << " frame didn't extracted from video!" << std::endl;
    }
    //cv::cvtColor(frame, convertedframe, CV_BGR2XYZ);

    cv::imshow("frame", convertedframe);
    save_frame(convertedframe, path, nbOfFrame);
}

/*Extract all the frames from video, it takes the path where the frames will be extracted, the video and the number of the frames in this video*/
void MediaFiles::ExtractFrames(const wchar_t * path,cv::VideoCapture& videofile, double & nbOfFrames, double & fps)
{
    cv::Mat frame;
    //CreateFolder(path);
    int index = nbOfFrames / fps;
    for (double f = 1; f < index; f+=round(fps))
    {
        ExtractFrame(path,videofile, frame, f);
        frame = cv::Mat();
    }
}
/*create a folder in path defined in paramter, return error if the directory already exists*/
void MediaFiles::CreateFolder(const wchar_t * path)
{
    if (CreateDirectory(path, NULL))
    {
        std::cout << path << " is created !" << std::endl;
    }
    else if(ERROR_ALREADY_EXISTS==GetLastError())
    {
        std::cout<< path<< " is already exists!" << std::endl;
    }
    else
    {
        std::cout<< " cannot create the folder! " << std::endl;
    }
}
// resize  the image, it take the size of the new resized image
void MediaFiles::resize_image(cv::Size dsize, cv::Mat img, cv::Mat resized_img)
{
    int interpolation = cv::INTER_LINEAR;
    cv::resize(img, resized_img, dsize, 0, 0, interpolation);
}

void MediaFiles::save_frame(cv::Mat & frame, const wchar_t * path, double & nbOfFrame)
{
    std::vector<int > compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);

    std::wstring ws(path);
    std::string str(ws.begin(), ws.end());

    std::string temp = str + "/frame_" + static_cast<std::ostringstream*>(&(std::ostringstream() << (nbOfFrame)))->str() + ".jpg";
    cv::imwrite(temp.c_str(), frame, compression_params);
}
void MediaFiles::save_frame(IplImage *frame, const wchar_t * path, double & nbOfFrame)
{

    std::wstring ws(path);
    std::string str(ws.begin(), ws.end());
    std::string temp = str + "/frame_" + static_cast<std::ostringstream*>(&(std::ostringstream() << (nbOfFrame)))->str() + ".jpg";
    cvSaveImage(temp.c_str(),frame);
    std::cout<<" the frame has been saved at "<<temp<<std::endl;
}
void MediaFiles::get_list_images(QDir &directory, QFileInfoList &fileList)
{
   QStringList filters;
   filters<<"*.png"<<"*.jpg"<<"*.bmp";
   directory.setNameFilters(filters);
   directory.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
   fileList=directory.entryInfoList();
}
void MediaFiles::read_file(QString nomfichier,  std::vector <cv::KeyPoint> & pts)
{
    QFile file1 (nomfichier);
    if(file1.open(QIODevice::ReadOnly| QIODevice::Text))
    {
     QTextStream in(&file1);
     while(!in.atEnd())
     {
         QStringList str1=in.readLine().split(";");
         pts.push_back(cv::KeyPoint (cv::Point2f(QString(str1.at(0)).toInt(), QString(str1.at(1)).toInt()), 1.f));
     }
    }
    else
    {
        std::cout<<" file  could not be opened !"<<std::endl;
    }
    file1.close();
}
void MediaFiles::read_file(QString nomfichier,  std::vector <cv::Point2f> & pts)
{
    QFile file1 (nomfichier);
    if(file1.open(QIODevice::ReadOnly| QIODevice::Text))
    {
     QTextStream in(&file1);
     while(!in.atEnd())
     {
         QStringList str1=in.readLine().split(";");
         pts.push_back(cv::Point2f(QString(str1.at(0)).toInt(), QString(str1.at(1)).toInt()));
     }
    }
    else
    {
        std::cout<<" file  could not be opened !"<<std::endl;
    }
    file1.close();
}
void MediaFiles::write_CSVfile(const std::string & nomfichier, std::vector<QStringList> parameters)
{
 std::ofstream out(nomfichier.c_str());
 if(!out)
     std::cout<< "\n Error: file could not be opened !";
 out<< "image"<<";"<<"resolution"<<";"<<"nombre des keypoints"
    <<";"<<"tps de calcul pr desc"<<";"<<"tps de calcul pr RANSAC"
   <<";"<<"nombre des  matches"<<";"<<"distance"<<"\n";
for(unsigned int i=0; i<parameters.size(); i++)
{
   QStringList temp=parameters.at(i);
   foreach(QString s, temp)
       out<<s.toLocal8Bit().constData()<<";";
   out <<"\n";
}
std::cout<<"the file" << nomfichier << "created!"<<std::endl;
out.close();
}
void MediaFiles::write_CSVfile(const std::string & nomfichier, QStringList parameters)
{
 std::ofstream out;
 out.open(nomfichier.c_str(),std::ofstream::out | std::ofstream::app);
 if(!out)
     std::cout<< "\n Error: file could not be opened !";
 out<< "image"<<";"<<"resolution"<<";"<<"nombre des keypoints"
    <<";"<<"tps de calcul pr desc"<<";"<<"tps de calcul pr RANSAC"
   <<";"<<"nombre des  matches"<<";"<<"distance"<<"\n";

   foreach(QString s, parameters)
       out<<s.toLocal8Bit().constData()<<";";
   out <<"\n";

std::cout<<"the file" << nomfichier << "created!"<<std::endl;
out.close();
}
void MediaFiles::append_CSVfile(std::ofstream & out, QStringList paramters)
{
    if(!out)
        std::cout<< "\n Error: file could not be opened !";
    foreach(QString s, paramters)
        out<<s.toLocal8Bit().constData()<<";";
    out<<std::endl;
}
void MediaFiles::append_CSVfile(std::ofstream & out, std::vector<cv::KeyPoint> points)
{
    if(!out)
        std::cout<< "\n Error: file could not be opened !";
    for (size_t i=0; i<points.size(); i++)
    {
        cv::KeyPoint pt= points.at(i);
        out<<pt.pt.x<<";"<<pt.pt.y<<";"<<pt.class_id<<std::endl;

    }
    std::cout <<" the keypoints is stored !!!";


}
void MediaFiles::append_CSVfile(std::ofstream & out, std::vector < cv::KeyPoint > points, std::vector<int> & vote)
{
    if(!out)
        std::cout<< "\n Error: file could not be opened !";

    for (size_t i=0; i<points.size(); i++)
    {

       out<< points.at(i).pt.x<<";"<< points.at(i).pt.y<<";"<< points.at(i).class_id<<";"<<vote.at(i)<<std::endl;

    }    std::cout <<" the keypoints is stored !!!";


}
void MediaFiles::append_CSVfile(std::ofstream & out, std::vector<cv::KeyPoint> & points, std::vector<int> & vote, int nOctaveLayers,int nOctaves)
{    if(!out)
        std::cout<< "\n Error: file could not be opened !";

    for (size_t i=0; i<points.size(); i++)
    {
       float echelle=std::pow(2.f,(points.at(i).octave*((points.at(i).class_id/(nOctaves-1))/(nOctaveLayers))));
       out<< points.at(i).pt.x<<";"<< points.at(i).pt.y<<";"<< points.at(i).class_id<<";"<<echelle<<";"<<points.at(i).octave<<";"<<vote.at(i)<<std::endl;

    }    std::cout <<" the keypoints is stored !!!";


}


void MediaFiles::search_keypoint_csv_file(const char *nom_fichier,std::vector<cv::Point2f> & matched_pts,std::ofstream & out)
{

    QFile file1 (nom_fichier);
    int vote=0;
    if(file1.open(QIODevice::ReadWrite| QIODevice::Text))
    {
     QTextStream in(&file1);
     while(!in.atEnd())
     {
         QString line=in.readLine();
         QStringList str1=line.split(";");
         int ancien_vote=QString(str1.at(3)).toInt();
         if(ancien_vote==NULL)
         {
             ancien_vote=0;
         }
         vote =ancien_vote+search_keypoint(matched_pts,cv::Point2f(QString(str1.at(0)).toInt(), QString(str1.at(1)).toInt()));
         QString outline= line.replace(QString("%1; %2; %3").arg(str1.at(0)).arg(str1.at(1).arg(str1.at(2))), QString("%1 ; %2 ; %3; %4").arg(str1.at(0)).arg(str1.at(1)).arg(str1.at(2)).arg(vote));
         std::cout<< outline.toLocal8Bit().constData() <<"\n";
         //out<<outline;
        // out<<"\n";
     }
    }
    else
    {
        std::cout<<" file  could not be opened !"<<std::endl;
    }
    file1.close();

}
void MediaFiles::read_csv_file(QString nomfichier, std::vector<cv::KeyPoint> &pts)
{

    QFile file1(nomfichier);
    if(file1.open(QIODevice::ReadWrite| QIODevice::Text))
    {
     QTextStream in(&file1);
     while(!in.atEnd())
     {
         QString line=in.readLine();
         QStringList str1=line.split(";");
         int vote= QString(str1.at(2)).toInt();
         if(vote!=0)
         {
           cv::KeyPoint pttts=cv::KeyPoint(cv::Point2f(QString(str1.at(0)).toFloat(), QString(str1.at(1)).toFloat()),1.f);
            pts.push_back(pttts);
         }
     }
    }
    else
    {
       std::cout<<" file  could not be opened !"<<std::endl;
    }
    file1.close();
}
void MediaFiles::search_descriptor(QString nomfichier, std::vector<cv::KeyPoint> &key_point, cv::Mat& desc)
{

    cv::FileStorage fs;
    std::vector<cv::KeyPoint> ref_pts;
    cv::Mat ref_desc;
    fs.open(nomfichier.toStdString(),cv::FileStorage::READ);
    if(fs.isOpened())
    {
        cv::read(fs["keypoint_1"],ref_pts);
        cv::read(fs["descriptor_1"],ref_desc);
    }
    else
    {
        std::cout<<"File Not found!"<<std::endl;
    }
    fs.release();
    desc.create(key_point.size(),ref_desc.cols, ref_desc.type());
    for (size_t i=0; i< key_point.size(); i++)
    {
        for(size_t j=0; j< ref_pts.size(); j++)
        {
            if((round(key_point.at(i).pt.x) == round(ref_pts.at(j).pt.x)) && (round(key_point.at(i).pt.y)==round(ref_pts.at(j).pt.y)))
            {
                ref_desc.row(j).copyTo(desc.row(i));
            }
        }
    }


}


int MediaFiles::search_keypoint(std::vector<cv::Point2f> &matched_pts, cv::Point2f keypt)
{
        int vote=0;
        for (size_t i=0; i< matched_pts.size(); i++)
        {
            if((matched_pts.at(i).x==keypt.x) && (matched_pts.at(i).y==keypt.y))
            {
                vote=1;
            }
            else
            {
                vote=0;
            }

        }
        return vote;

}

void MediaFiles::search_keypoint_csv_file(std::vector<cv::KeyPoint> ref_pt,std::vector<cv::Point2f> & matched_pts,  std::vector<int>& vote )
{
   for(size_t j=0; j<ref_pt.size();j++)
   {
       for (size_t i=0; i< matched_pts.size(); i++)
       {
           if((matched_pts.at(i).x==ref_pt.at(j).pt.x) && (matched_pts.at(i).y==ref_pt.at(j).pt.y))
           {
               vote.at(j) =vote.at(j)+1;
           }
           else
           {
                vote.at(j) =vote.at(j)+0;
           }

       }

    }
   std::cout<<" research finished";

}

