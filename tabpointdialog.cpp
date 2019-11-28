#include <QString>
#include "tabpointdialog.h"
#include "ui_tabpointdialog.h"

TabPointDialog::TabPointDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tabpointDialog)
{
    ui->setupUi(this);

}

TabPointDialog::TabPointDialog(QWidget *parent,vector<QPoint> * tab) :
    QDialog(parent),
    ui(new Ui::tabpointDialog)
{
    ui->setupUi(this);
    tabPoints_=tab;
}

TabPointDialog::~TabPointDialog()
{
    delete ui;
}

void TabPointDialog::ajouterPoint(const QPoint &p)
{
    QString s=QString("%1 %2").arg(p.x()).arg(p.y());

  /*  QListWidgetItem *item = new QListWidgetItem(s);
    ui->listPoints_->addItem(item);
    ui->listPoints_->setCurrentItem(item);*/
}

void TabPointDialog::on_supprimerButton__clicked()
{
    // remove in the listWidget
    //int pos=ui->listPoints_->currentRow();
    /*if (pos>=0) {
        delete ui->listPoints_->takeItem(pos);
        emit supprimerPoint(pos);
    }*/
}

