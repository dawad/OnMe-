#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include "editeurpoints.h"

EditeurPoints::EditeurPoints(QWidget *parent)
    : QMainWindow(parent)
{

    imageView_=new ImageView(this);
    imageView_->setTabPoints(&tabPoints_);
    setCentralWidget(imageView_);
    setWindowTitle("Editeur de points");

    QAction *ouvrirImageAction = new QAction(tr("&Ouvrir image..."), this);
    connect(ouvrirImageAction, SIGNAL(triggered()), this, SLOT(on_ouvrirImage()));
    QAction * enregistrerFichierAction=new QAction(tr("&Enregistrer un fichier..."), this);
    connect(enregistrerFichierAction,SIGNAL(triggered(bool)), this,SLOT(on_savePoints()));

    QMenu *fichierMenu = menuBar()->addMenu(tr("&Fichier"));
    fichierMenu->addAction(ouvrirImageAction);
     fichierMenu->addAction(enregistrerFichierAction);

}

EditeurPoints::~EditeurPoints()
{

}

void EditeurPoints::on_ouvrirImage()
{
    QString nomFich = QFileDialog::getOpenFileName(this,
         tr("Ouvrir image"), "", tr("Images (*.png *.jpg *.bmp)"));
    imageView_->ouvrirImage(nomFich);
}
void EditeurPoints::on_savePoints()
{
QString filename= QFileDialog::getSaveFileName(this,tr("Enregistrer un  fichier"), QString()," Text files (*.txt )");
imageView_->writefile(filename);
}
