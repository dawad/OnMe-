#ifndef TABPOINTDIALOG_H
#define TABPOINTDIALOG_H
#include <QString>
#include <QDialog>
#include "ui_tabpointdialog.h"


using namespace std;

namespace Ui {
class TabPointDialog;
}

class TabPointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TabPointDialog(QWidget *parent = 0);
    explicit TabPointDialog(QWidget *parent = 0,vector<QPoint> * tab=0);
    ~TabPointDialog();
    void ajouterPoint(const QPoint &p);

signals:
    void supprimerPoint(int pos);

private slots:
    void on_supprimerButton__clicked();

private:
    Ui::tabpointDialog *ui;
    vector<QPoint> * tabPoints_;
};

#endif // TABPOINTDIALOG_H
