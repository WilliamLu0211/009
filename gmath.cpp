#include <iostream>
#include <cmath>

#include "gmath.h"

using namespace std;

// lighting functions
pixel get_lighting( double *normal, pixel alight, light plight, double *areflect, double *dreflect, double *sreflect);

pixel calculate_ambient(pixel alight, double *areflect ){
  alight.r *= areflect[0];
  alight.g *= areflect[1];
  alight.b *= areflect[2];
  return alight;
}

pixel calculate_diffuse(light plight, double *dreflect, double *normal ){
  double c = dot(normal, plight.v) / magnitude(noral) / magnitude(plight.v);
  plight.r *= c;
  plight.g *= c;
  plight.b *= c;
  return plight;
}

pixel calculate_specular(light plight, double *sreflect, double *normal ){
  doubl *p = project(plight.v, normal);
  
}

void limit_color( pixel * c );

double magnitude(double *v){
  return sqrt(dot(v, v));
}

double *project(double *u, double *v){
  double *r = new double[3];
  double c = dot(u, v) / dot(v, v);
  r[0] = a[0] * c;
  r[1] = a[1] * c;
  r[2] = a[2] * c;
  return r;
}

void normalize(double *v){
  double mag = magnitude(v);
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
