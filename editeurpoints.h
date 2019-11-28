#ifndef EDITEURPOINTS_H
#define EDITEURPOINTS_H

#include <QMainWindow>
#include <vector>
#include "imageview.h"

using namespace std;

class EditeurPoints : public QMainWindow
{
    Q_OBJECT

public:
    EditeurPoints(QWidget *parent = 0);
    ~EditeurPoints();

private slots:
    void on_ouvrirImage();
     void on_savePoints();

private:
    ImageView * imageView_;
    vector<QPoint> tabPoints_;
};

#endif // EDITEURPOINTS_H
