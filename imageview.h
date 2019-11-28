#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <QPoint>
#include <QObject>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QMenu>
#include <vector>
#include "tabpointdialog.h"


//default zoom factors
#define DEFAULT_ZOOM_FACTOR 1.15
#define DEFAULT_ZOOM_CTRL_FACTOR 1.01

using namespace std;

class ImageView : public QGraphicsView
{
    Q_OBJECT
public:

    explicit ImageView(QWidget *parent = 0);
    ~ImageView();
    void setTabPoints(vector<QPoint> * tab);
    void ouvrirImage(QString nom);
    void tracePoint(const QPointF &p);
    void writefile(QString nom);
    void readfile(QString nom);
    //Set the zoom factor when the CTRL key is not pressed  zoom factor (>1)
    void setZoomFactor(const double factor) { zoomFactor=factor; }
    // Set the zoom factor when the CTRL key is pressed  zoom factor (>1)
    void setZoomCtrlFactor(const double factor) {zoomCtrlFactor=factor; }


signals:

public slots:
    void supprimerPoint(int pos);
    // This function fit the image in the view while keeping aspect ratio
    void fitImage();

protected slots:
    /*!
        * \brief setToolTipText        Display the tool tip over the mouse
        * \param imageCoordinates      Coordinates of the mouse in the image's frame
        * \return                      The function returns the QString that is displayed over the image
        */
       virtual QString         setToolTipText(QPoint imageCoordinates);


    /*!
        * \brief wheelEvent            Overload this function to process mouse wheel event
        * \param event                 mouse wheel event
        */
       virtual void            wheelEvent(QWheelEvent *event);
    /*!
        * \brief mouseMoveEvent        Overload this function to process mouse moves
        * \param event                 mouse move event
        */
       virtual void            mouseMoveEvent(QMouseEvent *event);
    /*!
     * \brief showContextMenu       Display the contextual menu (on right click)
     * \param pos                   Position of the mouse in the widget
     */
    virtual void            showContextMenu(const QPoint & pos);

private:
    void mousePressEvent(QMouseEvent *event);
     // Scene where the image is drawn
    QGraphicsScene * scene_;
    // Pixmap item containing the image
    QGraphicsPixmapItem*    pixmapItem;
    // Zoom factor
    double       zoomFactor;
   // Zoom factor when the CTRL key is pressed
    double       zoomCtrlFactor;
    // Current pixmap
    QPixmap * pixmap_;
    double factor=1;
    vector<QPoint> * tabPoints_;
    TabPointDialog * tabPointDialog_;

};

#endif // IMAGEVIEW_H
