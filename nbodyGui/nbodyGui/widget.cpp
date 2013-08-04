#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include "nbody.h"
#include <QDebug>
#include <ctime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    f5=new QTimer(this);
    initNBody(20000);
    f5->start(3);
    connect(f5,SIGNAL(timeout()),this,SLOT(updataImage()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updataImage() {
    static int aa=0;
    qDebug()<<++aa;
    double dt=0.002;
    long long tt=clock();
    FOR(i,0,size-1)
            body[i].x=body[i].x + v[i]*dt + f[i]*(dt*dt*.5/body[i].m);
    BVHTree tree(body,size);
    qDebug()<<clock()-tt;
    FOR(i,0,size-1) {
        Point nf=tree.cal(body[i]);
        v[i]=v[i] + (f[i]+nf)*(dt*.5/body[i].m);
        f[i]=nf;
    }
    qDebug()<<clock()-tt;
    update();
}

double Widget::rdFloat() {
    return ((rand()*1ll<<15)+(rand()*1ll<<30)+(rand()*1ll<<45)+rand()*1ll)%(1ll<<60)*1./(1ll<<60);
}

void Widget::initNBody(int n) {
    size=n;
    body=new Body[n];
    v=new Point[n];
    f=new Point[n];
    //FOR(i,0,n-1) body[i]=Body(rdFloat()*200+100,rdFloat()*200+100,1);
    FOR(i,0,n-1) {
        body[i]=Body(rdFloat()*200-100,rdFloat()*200-100,0.5+rdFloat()*rdFloat()*100);
        while (body[i].x.mo()>100*100)
            body[i]=Body(rdFloat()*200-100,rdFloat()*200-100,0.5+rdFloat()*rdFloat()*100);
        v[i].x[0]=body[i].x[1];
        v[i].x[1]=-body[i].x[0];
        v[i]=v[i]*10;
        if (i==0) {
            v[i]=Point();
            body[i].x=Point();
            body[i].m=1e5;
        }
        body[i].x[0]+=500;
        body[i].x[1]+=300;
    }
    BVHTree tree(body,size);
    FOR(i,0,n-1) f[i]=tree.cal(body[i]);
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter pt(this);
    pt.setRenderHint( QPainter::Antialiasing, true );
    pt.setBrush(QBrush(Qt::red));
    pt.setPen(Qt::NoPen);
    FOR(i,0,size-1) pt.drawEllipse(QRectF(body[i].x[0],body[i].x[1],.8,.8));
}
