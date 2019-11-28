#ifndef MEDIAFILES_H
#define MEDIAFILES_H
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
class MediaFiles
{
public:
    MediaFiles();
    ~MediaFiles();
    cv::Mat OpenImage(std::string & filename);
    cv::Mat OpenImage(const char* filename);
    void readAVIVideo(std::string & filename, CvCapture* video);
    void readMP4Video(std::string & filename, cv::VideoCapture & , double &, double& );
    CvCapture* readvideo(std::string &,double &, double &);
    void ReadVideo_ExtractFrames(std::string & filename, const wchar_t * pathOfFrames);
    void ReadVideoANDExtractFrames(std::string & filename, const wchar_t * pathOfFrames);
    void ExtractFrame(const wchar_t *,cv::VideoCapture &, cv::Mat, double &);
    void ExtractFrames(const wchar_t *,cv::VideoCapture &, double &, double & );
    void CreateFolder(const wchar_t *);
    void resize_image(cv::Size, cv::Mat img, cv::Mat resized_img);
    void save_frame(cv::Mat &, const wchar_t *, double &);
    void save_frame(IplImage *, const wchar_t *, double &);
    void get_list_images(QDir &, QFileInfoList &);
    void read_file(QString nomfichier, std::vector <cv::KeyPoint> & pts);
    void read_file(QString nomfichier, std::vector <cv::Point2f> & pts);
    void write_CSVfile(const std::string & nomfichier, std::vector<QStringList> parameters);
    void write_CSVfile(const std::string & nomfichier,QStringList paramters);
    void append_CSVfile(std::ofstream &, QStringList  paramters);
    void append_CSVfile(std::ofstream &, std::vector<cv::KeyPoint> points);
    void append_CSVfile(std::ofstream & out, std::vector < cv::KeyPoint > points, std::vector <int> & vote);
    void append_CSVfile(std::ofstream & out, std::vector < cv::KeyPoint > & points, std::vector<int> & vote, int ,int );
    void search_keypoint_csv_file(const char* nom_fichier, std::vector<cv::Point2f> & matched_pts,std::ofstream &);
    void search_keypoint_csv_file(std::vector<cv::KeyPoint> ref_pt,std::vector<cv::Point2f> & matched_pts,std::vector<int>& vote );
    int search_keypoint(std::vector<cv::Point2f> & matched_pts, cv::Point2f keypt);
    void read_csv_file(QString nomfichier,std::vector<cv::KeyPoint> & pts);
    void search_descriptor(QString nomfichier, std::vector<cv::KeyPoint> & keypoint, cv::Mat &desc);

};

#endif // MEDIAFILES_H
