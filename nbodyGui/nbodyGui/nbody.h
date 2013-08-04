#ifndef NBODY_H
#define NBODY_H

#include <algorithm>
#include <iostream>
#include <cstring>
#include <cmath>

#define FOR(i,l,r) for (int i=(l);i<=(r);i++)
#define ROF(i,r,l) for (int i=(r);i<=(l);i--)

using namespace std;

#define DIM 2
#define THETA 1.9

//#define ONLYFOR2

struct Point {
  float x[DIM];
  Point() {memset(x,0,sizeof x);}
  Point(float xx, float yy) {x[0]=xx,x[1]=yy;}
  float& operator [](const int &i) {return x[i];}
  const float& operator [](const int &i) const {return x[i];}

  Point operator +(const Point &b) const {
#ifndef ONLYFOR2
    Point c;
    FOR(i,0,DIM-1) c[i]=x[i]+b[i];
    return c;
#else
    return Point(x[0]+b.x[0],x[1]+b.x[1]);
#endif
  }

  Point operator -(const Point &b) const {
#ifndef ONLYFOR2
    Point c;
    FOR(i,0,DIM-1) c[i]=x[i]-b[i];
    return c;
#else
    return Point(x[0]-b.x[0],x[1]-b.x[1]);
#endif
  }

  Point operator *(const float &b) const {
#ifndef ONLYFOR2
    Point c;
    FOR(i,0,DIM-1) c[i]=x[i]*b;
    return c;
#else
    return Point(x[0]*b,x[1]*b);
#endif
  }

  float mo() const {
#ifndef ONLYFOR2
    float s=0;
    FOR(i,0,DIM-1) s+=x[i]*x[i];
    return s;
#else
    return x[0]*x[0]+x[1]*x[1];
#endif
  }

};

struct Body {
  Point x;
  float m;

  Body(float _x=0, float _y=0, float _m=0) : m(_m) {x[0]=_x, x[1]=_y;}
  Body(const Body &a, const Body &b) : m(a.m+b.m) {
#ifndef ONLYFOR2
    FOR(i,0,DIM-1)
      x[i]=(a.x[i]*a.m+b.x[i]*b.m)/m;
#else
    x[0]=(a.x[0]*a.m+b.x[0]*b.m)/m;
    x[1]=(a.x[1]*a.m+b.x[1]*b.m)/m;
#endif
  }

};

struct BVHTree {

  struct Node {
    Node *l,*r;
    int dim;
    Body a;
    float w;

    Node() : l(0),r(0),dim(0),w(0) {}
    ~Node() {
      if (l!=0) {
        delete l;
        delete r;
      }
    }

  } *root;

  typedef Node* PtrNode;

  ~BVHTree() {delete root;}
  int t;

  void build(PtrNode &o, Body *b, int n) {
    o=new Node();
    if (n==0) return;
    if (n==1) {
      o->a=*b;
      return;
    }
    float l[DIM],r[DIM];
    FOR(i,0,DIM-1) l[i]=r[i]=b->x[i];
    FOR(i,0,n-1) {
      FOR(j,0,DIM-1) {
        l[j]=min(l[j],b[i].x[j]);
        r[j]=max(r[j],b[i].x[j]);
      }
    }
    FOR(i,0,DIM-1) if (r[o->dim]-l[o->dim]<r[i]-l[i])
      o->dim=i;
    FOR(i,0,DIM-1) o->w+=(r[i]-l[i])*(r[i]-l[i]);
    float mid=(r[o->dim]+l[o->dim])*.5;
    int t=0;
    FOR(i,0,n-1) if (b[i].x[o->dim] < mid)
      swap(b[t++],b[i]);
    build(o->l,b,t);
    build(o->r,b+t,n-t);
    o->a=Body(o->l->a,o->r->a);
  }

  BVHTree(Body *a, int n) {
    Body copy_a[n];
    FOR(i,0,n-1) copy_a[i]=a[i];
    build(root,copy_a,n);
  }

  inline Point cal(const Point &f, const float &mf,const float &mxm) {
    return f*(mxm/(mf+0.11));
  }

  Point cal(Node &o, const Body &a) {
    Point f=o.a.x-a.x;
    float dis=f.mo();
    if (dis<1e-5) return Point();
    if (o.w<dis*THETA*THETA)
      return cal(f,dis,a.m*o.a.m);
    else
      return cal(*(o.l),a)+cal(*(o.r),a);
  }

  Point cal(Body a) {
    return cal(*root,a);
  }

};

#endif
