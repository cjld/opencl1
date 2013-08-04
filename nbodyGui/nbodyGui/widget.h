#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include "nbody.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void initNBody(int);
    void paintEvent(QPaintEvent *);
    double rdFloat();

public slots:
    void updataImage();
    
private:
    Ui::Widget *ui;
    QTimer *f5;
    Body *body;
    Point *v,*f;
    int size;
};

#endif // WIDGET_H
