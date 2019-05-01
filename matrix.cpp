#include <iostream>
#include <cmath>
#include <ctime>

#include "matrix.h"

using namespace std;


Matrix::Matrix(int r, int c){
  rows = r;
  maxCols = c;
  cols = 0;
  m = initialize(rows, maxCols);
}

Matrix::~Matrix(){
  destroy();
}

double **Matrix::initialize(int r, int c){
  double **newM = new double*[r];
  for (int i = 0; i < r; i ++)
    newM[i] = new double[c];
  return newM;
}

void Matrix::destroy(){
  for (int i = 0; i < rows; i ++)
    delete[] m[i];
  delete[] m;
}

void Matrix::copy(Matrix &b){
  b.destroy();
  b.m = initialize(rows, maxCols);
  for (int i = 0; i < rows; i ++)
    for (int j = 0; j < cols; j ++)
      // cout << m[i][j] << endl;
      b.m[i][j] = m[i][j];
  b.cols = cols;
  // b.print();
}

void Matrix::print(){
  for (int i = 0; i < rows; i ++){
    cout << "|";
    for (int j = 0; j < cols; j ++)
      cout << m[i][j] << " ";
    cout << "|\n";
  }
}

int Matrix::getRows(){ return rows; }

int Matrix::getCols(){ return cols; }

void Matrix::ident(){
  cols = rows;
  for (int i = 0; i < rows; i ++)
    for (int j = 0; j < cols; j ++)
      if (i == j)
        m[i][j] = 1;
      else
        m[i][j] = 0;
}

void Matrix::mult(Matrix &b){
  if (cols != b.rows){
    cout << "invalid dimensions\n";
    return;
  }
  double **newM = initialize(rows, b.maxCols);
  double temp;
  for (int i = 0; i < rows; i ++){
    for (int j = 0; j < b.cols; j ++){
      temp = 0;
      for (int k = 0; k < cols; k ++)
        temp += m[i][k] * b.m[k][j];
      newM[i][j] = temp;
    }
  }
  b.destroy();
  b.m = newM;
  b.rows = rows;
}

// Matrix Matrix::operator *(Matrix b){
//   // cout << "*";
//   if (cols != b.rows){
//     cout << "invalid dimensions\n";
//     exit(1);
//   }
//   Matrix newM(rows, b.cols);
//   newM.cols = b.cols;
//   double temp;
//   for (int i = 0; i < rows; i ++){
//     for (int j = 0; j < b.cols; j ++){
//       temp = 0;
//       for (int k = 0; k < cols; k ++)
//         temp += m[i][k] * b.m[k][j];
//       newM.m[i][j] = temp;
//     }
//   }
//   // newM.print();
//   return newM;
// }
//
// void Matrix::operator =(Matrix b){
//   destroy();
//   m = initialize(b.rows, b.maxCols);
//   for (int i = 0; i < b.rows; i ++)
//     for (int j = 0; j < b.cols; j ++)
//       m[i][j] = b.m[i][j];
//   cols = b.cols;
//   // cout << rows << cols << maxCols << endl;
// }

// void Matrix::operator *=(Matrix b){
//   if (b.cols != rows){
//     cout << "invalid dimensions\n";
//     exit(1);
//   }
//   double **newM = initialize(b.rows, cols);
//   double temp;
//   for (int i = 0; i < b.rows; i ++){
//     for (int j = 0; j < cols; j ++){
//       temp = 0;
//       for (int k = 0; k < rows; k ++)
//         temp += b.m[i][k] * m[k][j];
//       newM[i][j] = temp;
//     }
//   }
//   destroy();
//   m = newM;
// }

//-------------------------------------------------------------

Points::Points():Matrix(4, 10){
}

void Points::clear(){
  cols = 0;
  destroy();
  // Points();
  m = initialize(4, 10);
  cols = 0;
  maxCols = 10;
}

void Points::grow(){
  maxCols *= 2;
  double **newM = initialize(rows, maxCols);// = initialize(rows, maxCols);
  for (int i = 0; i < rows; i ++){
    for (int j = 0; j < cols; j ++)
      newM[i][j] = m[i][j];
    delete[] m[i];
  }
  delete[] m;
  // destroy();
  m = newM;
}

void Points::add(double x, double y, double z){
  if (cols >= maxCols)
    grow();
  // for (int i = 0; i < rows; i ++)
  //   m[i][cols] = newCol[i];
  m[0][cols] = x;
  m[1][cols] = y;
  m[2][cols] = z;
  m[3][cols] = 1;
  cols ++;
}

double Points::getX(int col){
  if (cols < 0 || col >= cols){
    cout << "index out of bound\n";
    return -1;
  }
  return m[0][col];
}

double Points::getY(int col){
  if (cols < 0 || col >= cols){
    cout << "index out of bound\n";
    return -1;
  }
  return m[1][col];
}

double Points::getZ(int col){
  if (cols < 0 || col >= cols){
    cout << "index out of bound\n";
    return -1;
  }
  return m[2][col];
}

//-------------------------------------------------------------

Edges::Edges():Points(){
}

void Edges::addEdge(double x0, double y0, double z0, double x1, double y1, double z1){
  add(x0, y0, z0);
  add(x1, y1, z1);
}


void Edges::addCircle(double cx, double cy, double cz,
                      double r, double step){
  double x0 = cx + r, y0 = cy;
  double x1, y1;
  for (int t = 1; t <= step; t ++){
    x1 = cx + r * cos(2 * M_PI * t / step);
    y1 = cy + r * sin(2 * M_PI * t / step);
    addEdge(x0, y0, cz, x1, y1, cz);
    x0 = x1;
    y0 = y1;
  }
}

void Edges::addCurve(double x0, double y0,
                     double x1, double y1,
                     double x2, double y2,
                     double x3, double y3,
                     int type, double step){
  Curve x(x0, x1, x2, x3, type);
  Curve y(y0, y1, y2, y3, type);
  double ax = x.getA(), bx = x.getB(), cx = x.getC(), dx = x.getD();
  double ay = y.getA(), by = y.getB(), cy = y.getC(), dy = y.getD();
  double u;
  for (int t = 1; t <= step; t ++){
    u = t / step;
    x1 = ax * pow(u, 3) + bx * pow(u, 2) + cx * u + dx;
    y1 = ay * pow(u, 3) + by * pow(u, 2) + cy * u + dy;
    addEdge(x0, y0, 0, x1, y1, 0);
    x0 = x1;
    y0 = y1;
  }
}

Polygons::Polygons():Points(){
}

void Polygons::addPolygon(double x0, double y0, double z0,
                          double x1, double y1, double z1,
                          double x2, double y2, double z2){
  add(x0, y0, z0);
  add(x1, y1, z1);
  add(x2, y2, z2);
}

void Polygons::addBox(double x, double y, double z,
                      double w, double h, double d){
  // addEdge(x, y, z, x+w, y, z);
  // addEdge(x, y, z, x, y-h, z);
  // addEdge(x, y, z, x, y, z-d);
  // addEdge(x+w, y-h, z, x, y-h, z);
  // addEdge(x+w, y-h, z, x+w, y, z);
  // addEdge(x+w, y-h, z, x+w, y-h, z-d);
  // addEdge(x+w, y, z-d, x, y, z-d);
  // addEdge(x+w, y, z-d, x+w, y, z);
  // addEdge(x+w, y, z-d, x+w, y-h, z-d);
  // addEdge(x, y-h, z-d, x, y, z-d);
  // addEdge(x, y-h, z-d, x, y-h, z);
  // addEdge(x, y-h, z-d, x+w, y-h, z-d);
  addPolygon(x, y, z, x, y-h, z, x+w, y, z);
  addPolygon(x, y-h, z, x+w, y-h, z, x+w, y, z);
  addPolygon(x, y, z-d, x+w, y, z-d, x, y-h, z-d);
  addPolygon(x, y-h, z-d, x+w, y, z-d, x+w, y-h, z-d);

  addPolygon(x+w, y, z, x+w, y-h, z, x+w, y, z-d);
  addPolygon(x+w, y-h, z, x+w, y-h, z-d, x+w, y, z-d);
  addPolygon(x, y, z, x, y, z-d, x, y-h, z);
  addPolygon(x, y-h, z, x, y, z-d, x, y-h, z-d);

  addPolygon(x, y, z-d, x, y, z, x+w, y, z-d);
  addPolygon(x, y, z, x+w, y, z, x+w, y, z-d);
  addPolygon(x, y-h, z-d, x+w, y-h, z-d, x, y-h, z);
  addPolygon(x, y-h, z, x+w, y-h, z-d, x+w, y-h, z);
}

void Polygons::addSphere(double cx, double cy, double cz,
                         double r, int step){
  Sphere s(cx, cy, cz, r, step);

  int p0 = 0, p1 = 1, p2 = step + 1, temp;
  for (int i = 0; i < step; i ++){

    p2 %= step * step;

    for (int j = 0; j < step - 2; j ++){
      addPolygon(s.getX(p0), s.getY(p0), s.getZ(p0),
                 s.getX(p1), s.getY(p1), s.getZ(p1),
                 s.getX(p2), s.getY(p2), s.getZ(p2));

      addPolygon(s.getX(p1), s.getY(p1), s.getZ(p1),
                 s.getX(p2+1), s.getY(p2+1), s.getZ(p2+1),
                 s.getX(p2), s.getY(p2), s.getZ(p2));

      p0 ++;
      p1 ++;
      p2 ++;
    }

    p0 += 2;
    p1 += 2;
    p2 += 2;
  }
}

void Polygons::addTorus(double cx, double cy, double cz,
                        double r1, double r2, int step){
  Torus t(cx, cy, cz, r1, r2, step);

  int p0, p1, p2;
  for (int i = 0; i < step; i ++){
    for (int j = 0; j < step; j ++){
      p0 = i * step + j;
      p1 = i * step + (j + 1) % step;
      p2 = (p1 + step) % (step * step);
      addPolygon(t.getX(p0), t.getY(p0), t.getZ(p0),
                 t.getX(p2), t.getY(p2), t.getZ(p2),
                 t.getX(p1), t.getY(p1), t.getZ(p1));
      p1 = p2;
      p2 = (p0 + step) % (step * step);
      addPolygon(t.getX(p0), t.getY(p0), t.getZ(p0),
                 t.getX(p2), t.getY(p2), t.getZ(p2),
                 t.getX(p1), t.getY(p1), t.getZ(p1));

    }

  }
}

//-------------------------------------------------------------

Sphere::Sphere(double cx, double cy, double cz,
               double r, int step): Points(){
  double angleP, angleT;
  for (int p = 0; p < step; p ++){
    angleP = 2 * M_PI * p / step;
    for (int t = 0; t < step; t ++){
      angleT = M_PI * t / (step-1);
      add(cx + r * cos(angleT),
          cy + r * sin(angleT) * cos(angleP),
          cz + r * sin(angleT) * sin(angleP));
    }
  }
}

Torus::Torus(double cx, double cy, double cz,
             double r1, double r2, int step):Points(){
  double angleP, angleT;
  for (int p = 0; p < step; p ++){
    angleP = 2 * M_PI * p / step;
    for (int t = 0; t < step; t ++){
      angleT = 2 * M_PI * t / step;
      add(cx + (r1 * cos(angleT) + r2) * cos(angleP),
          cy + r1 * sin(angleT),
          cz - (r1 * cos(angleT) + r2) * sin(angleP) );
    }
  }
}

//-------------------------------------------------------------

Transform::Transform():Matrix(){
  ident();
}

Translate::Translate(double x, double y, double z):Transform(){
  m[0][3] = x;
  m[1][3] = y;
  m[2][3] = z;
}

Scale::Scale(double x, double y, double z):Transform(){
  m[0][0] = x;
  m[1][1] = y;
  m[2][2] = z;
}

RotX::RotX(double t):Transform(){
  m[1][1] = cos(t);
  m[1][2] = -sin(t);
  m[2][1] = sin(t);
  m[2][2] = cos(t);
}

RotY::RotY(double t):Transform(){
  m[0][0] = cos(t);
  m[0][2] = sin(t);
  m[2][0] = -sin(t);
  m[2][2] = cos(t);
}

RotZ::RotZ(double t):Transform(){
  m[0][0] = cos(t);
  m[0][1] = -sin(t);
  m[1][0] = sin(t);
  m[1][1] = cos(t);
}

//-------------------------------------------------------------

Bezier::Bezier():Matrix(){
  cols = 4;

  m[0][0] = -1;
  m[0][1] = 3;
  m[0][2] = -3;
  m[0][3] = 1;

  m[1][0] = 3;
  m[1][1] = -6;
  m[1][2] = 3;
  m[1][3] = 0;

  m[2][0] = -3;
  m[2][1] = 3;
  m[2][2] = 0;
  m[2][3] = 0;

  m[3][0] = 1;
  m[3][1] = 0;
  m[3][2] = 0;
  m[3][3] = 0;
}

Hermite::Hermite():Matrix(){
  cols = 4;

  m[0][0] = 2;
  m[0][1] = -2;
  m[0][2] = 1;
  m[0][3] = 1;

  m[1][0] = -3;
  m[1][1] = 3;
  m[1][2] = -2;
  m[1][3] = -1;

  m[2][0] = 0;
  m[2][1] = 0;
  m[2][2] = 1;
  m[2][3] = 0;

  m[3][0] = 1;
  m[3][1] = 0;
  m[3][2] = 0;
  m[3][3] = 0;
}

//-------------------------------------------------------------

Curve::Curve(double p0, double p1, double p2, double p3, int type):Matrix(4, 1){
  cols = 1;
  Matrix *inv;
  if (type == HERMITE) {
    m[0][0] = p0;
    m[1][0] = p1;
    m[2][0] = p2;
    m[3][0] = p3;
    inv = new Hermite();
  }
  else {
    this->m[0][0] = p0;
    this->m[1][0] = p1;
    this->m[2][0] = p2;
    this->m[3][0] = p3;
    inv = new Bezier();
  }
  inv->mult(*this);
  delete inv;
}

double Curve::getA(){ return m[0][0]; }

double Curve::getB(){ return m[1][0]; }

double Curve::getC(){ return m[2][0]; }

double Curve::getD(){ return m[3][0]; }

//-------------------------------------------------------------

// int main(){
//
//   Matrix a;
//   srand(time(NULL));
//
//   double *c = new double[4];
//   c[3] = 1;
//
//   for (int i = 0; i < 8; i ++){
//     c[0] = rand() % 100;
//     c[1] = rand() % 100;
//     c[2] = rand() % 100;
//     a.add(c);
//   }
//
//   a.print();
//
//   Translate b(1, 2, 3);
//
//   b.print();
//   Matrix r = b * a;
//   r.print();
//
//   // b = new Scale(2, 1, 3);
//   // b->print();
//   // b->mult(*a);
//   // a->print();
//   // Matrix *d = *b * a;
//
//   return 0;
// }
