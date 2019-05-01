#ifndef MATRIX_H
#define MATRIX_H

#define HERMITE 0
#define BEZIER 1

class Matrix {

protected:
  double **m;
  int rows, cols;
  int maxCols;

  double **initialize(int r, int c);

  void destroy();

public:

  // Matrix();

  Matrix(int r = 4, int c = 4);

  ~Matrix();

   void copy(Matrix &b);

   int getRows();

   int getCols();
   //----------------------------------

   void ident();

   void print();

   void mult(Matrix &b);

   // Matrix operator *(Matrix b);
   //
   // void operator =(Matrix b);
   //
   // void operator *=(Matrix b);

};

class Points: public Matrix {

  void grow();

public:
  Points();

  void clear();

  void add(double x, double y, double z);

  double getX(int col);

  double getY(int col);

  double getZ(int col);
};

class Edges: public Points {

  public:
    Edges();

  void addEdge(double x0, double y0, double z0,
               double x1, double y1, double z1);

  void addCircle(double cx, double cy, double cz,
                 double r, double step = 50);

  void addCurve(double x0, double y0,
                double x1, double y1,
                double x2, double y2,
                double x3, double y3,
                int type, double step = 50);

};

class Polygons: public Points {

public:
  Polygons();

  void addPolygon(double x0, double y0, double z0,
                  double x1, double y1, double z1,
                  double x2, double y2, double z2);

  void addBox(double x, double y, double z,
              double w, double h, double d);

  void addSphere(double cx, double cy, double cz,
                 double r, int step = 20);

  void addTorus(double cx, double cy, double cz,
                double r1, double r2, int step = 20);
};

class Sphere: public Points {
public:
  Sphere(double cx, double cy, double cz,
         double r, int step = 20);
};

class Torus: public Points {
public:
  Torus(double cx, double cy, double cz,
        double r1, double r2, int step = 20);
};

class Transform: public Matrix {
public:
  Transform();
};

class Translate: public Transform {
public:
  Translate(double x = 0, double y = 0, double z = 0);
};

class Scale: public Transform {
public:
  Scale(double x = 1, double y = 1, double z = 1);
};

class RotX: public Transform {
public:
  RotX(double t = 0);
};

class RotY: public Transform {
public:
  RotY(double t = 0);
};

class RotZ: public Transform {
public:
  RotZ(double t = 0);
};

class Bezier: public Matrix {
public:
  Bezier();
};

class Hermite: public Matrix {
public:
  Hermite();
};

class Curve: public Matrix {
public:

  Curve(double p0, double p1, double p2, double p3, int type);

  double getA();

  double getB();

  double getC();

  double getD();
};

#endif
