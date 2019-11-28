#include<QGraphicsScene>
#include<QGraphicsPixmapItem>
#include<QMouseEvent>
#include<QPen>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "imageview.h"

ImageView::ImageView(QWidget *parent) :
    QGraphicsView(parent)
{
    //set default zoom factors
    zoomFactor=DEFAULT_ZOOM_FACTOR;
    zoomCtrlFactor=DEFAULT_ZOOM_CTRL_FACTOR;
    tabPoints_=0;
    tabPointDialog_=0;
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    scene_= new QGraphicsScene;
    // Allow mouse tracking even if no button is pressed
    this->setMouseTracking(true);
    // Add the scene to the QGraphicsView
    setScene(scene_);
    // Update all the view port when needed, otherwise, the drawInViewPort may experience trouble
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
   // When zooming, the view stay centered over the mouse
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    ouvrirImage(":/images/Saturne.bmp");
}

void ImageView::setTabPoints(vector<QPoint> *tab)
{
    tabPoints_=tab;
    tabPointDialog_=new TabPointDialog(this,tabPoints_);
    connect(tabPointDialog_,SIGNAL(supprimerPoint(int)),this,SLOT(supprimerPoint(int)));
}

ImageView::~ImageView()
{
    delete pixmap_;
    delete tabPointDialog_;
}

void ImageView::ouvrirImage(QString nom)
{
    pixmap_=new QPixmap(nom);
    scene_->addPixmap(*pixmap_);
}
void ImageView::writefile(QString nom)
{
  QFile file(nom);
  file.open(QIODevice::WriteOnly|QIODevice::Text);
  QTextStream out (&file);
  if(!(tabPoints_->empty()))
  {
   for(size_t i=0; i< tabPoints_->size(); i++)
   {
     QPoint pt= tabPoints_->at(i);
     out<<pt.x()<<";" <<pt.y()<<endl;
   }

  }
 file.close();
}
void ImageView::readfile(QString nom)
{

    QFile file1 (nom);
    file1.open(QIODevice::ReadOnly| QIODevice::Text);
    QTextStream in(&file1);
    QString str1=in.readLine();
    file1.close();
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint p=event->pos();
        if (p.x()<pixmap_->width() && p.y()<pixmap_->height()) {
            tracePoint(mapToScene(p));
            tabPoints_->push_back(QPoint(p.x()/factor, p.y()/factor));
            tabPointDialog_->ajouterPoint(QPoint(p.x()/factor, p.y()/factor));
            tabPointDialog_->show();
        }
    }
}

void ImageView::fitImage()
{
    // Get current scroll bar policy
     Qt::ScrollBarPolicy	currentHorizontalPolicy = horizontalScrollBarPolicy();
     Qt::ScrollBarPolicy	currentverticalPolicy = verticalScrollBarPolicy();

     // Disable scroll bar to avoid a margin around the image
     setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

     // Fit the scene in the QGraphicsView
     this->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);

     // Restaure scroll bar policy
     setHorizontalScrollBarPolicy(currentHorizontalPolicy);
     setVerticalScrollBarPolicy(currentverticalPolicy);
}

void ImageView::tracePoint(const QPointF &p)
{
    QPen pen(Qt::green, 2);
    scene_->addLine(p.x()-2,p.y()-2,p.x()+2,p.y()+2,pen);
    scene_->addLine(p.x()-2,p.y()+2,p.x()+2,p.y()-2,pen);

}

void ImageView::supprimerPoint(int pos)
{
    QPointF p(tabPoints_->at(pos).x(),tabPoints_->at(pos).y());
    QTransform trans;
    QGraphicsItem * item=scene_->itemAt(p,trans);
    scene_->removeItem(item);
    item=scene_->itemAt(p,trans);
    scene_->removeItem(item);
    tabPoints_->erase(tabPoints_->begin()+pos);
}
#ifndef QT_NO_WHEELEVENT

// Call when there is a scroll event (zoom in or zoom out)
void ImageView::wheelEvent(QWheelEvent *event)
{
    // When zooming, the view stay centered over the mouse
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    double ratio = (event->modifiers() & Qt::ControlModifier) ? zoomCtrlFactor : zoomFactor;
    if(event->delta() > 0)
    {
        factor=ratio;
        // Zoom in
        scale(ratio, ratio);
    }
    else
    {
        factor=1.0 / ratio;
        // Zooming out
        scale(1.0 / ratio, 1.0 / ratio);
    }
    // The event is processed
    event->accept();
}
// Overload the mouse MoveEvent to display the tool tip
void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    // Get the coordinates of the mouse in the scene
    QPointF imagePoint = mapToScene(QPoint(event->x(), event->y() ));
    // Call the function that create the tool tip
    setToolTip(setToolTipText(QPoint((int)imagePoint.x(),(int)imagePoint.y())));
    // Call the parent's function (for dragging)
    QGraphicsView::mouseMoveEvent(event);
}
#endif
// Define the virtual function to avoid the "unused parameter" warning
QString ImageView::setToolTipText(QPoint imageCoordinates)
{
    (void)imageCoordinates;
    return QString("");
}
void ImageView::showContextMenu(const QPoint & pos)
{
    // Get the mouse position in the scene
    QPoint globalPos = mapToGlobal(pos);
    // Create the menu and add action
    QMenu contextMenu;
    contextMenu.addAction("Reset view", this, SLOT(fitImage()));
    // Display the menu
    contextMenu.exec(globalPos);
}
