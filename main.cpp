#include <iostream>
#include <cmath>
#include <stack>
// #include <cstdlib>
// #include <ctime>

#include "image.h"

using namespace std;

int main() {

  pixel ambient;
  ambient.r = 255;
  ambient.g = 255;
  ambient.b = 255;

  light point;
  point.v = new double[3];
  point.v[0] = 0.5;
  point.v[1] = 0.75;
  point.v[2] = 1;

  point.r = 255;
  point.g = 255;
  point.b = 255;

  double areflect[3];
  areflect[0] = 0.1;
  areflect[1] = 0.1;
  areflect[2] = 0.1;
  double dreflect[3];
  dreflect[0] = 0.5;
  dreflect[1] = 0.5;
  dreflect[2] = 0.5;
  double sreflect[3];
  sreflect[0] = 0.4;
  sreflect[1] = 0.4;
  sreflect[2] = 0.4;

  Image img;
  img.parse(ambient, point, areflect, dreflect, sreflect);


  return 0;
}
