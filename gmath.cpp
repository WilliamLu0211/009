#include <iostream>
#include <cmath>

#include "gmath.h"

using namespace std;

void normalize(double *v){
  double mag = dot(v, v);
  for (int i = 0; i < 3; i ++)
    v[i] /= mag;
}

double dot(double *a, double *b){
  double sum = 0;
  for (int i = 0; i < 3; i ++)
    sum += a[i] * b[i];
  return sum;
}

double *normals(Polygons &p, int i){
  double *norm = new double[3];
  double x0 = p.getX(i+1) - p.getX(i);
  double y0 = p.getY(i+1) - p.getY(i);
  double z0 = p.getZ(i+1) - p.getZ(i);
  double x1 = p.getX(i+2) - p.getX(i);
  double y1 = p.getY(i+2) - p.getY(i);
  double z1 = p.getZ(i+2) - p.getZ(i);
  norm[0] = y0 * z1 - z0 * y1;
  norm[1] = z0 * x1 - x0 * z1;
  norm[2] = x0 * y1 - y0 * x1;
  return norm;
}
