#include <stack>

#include "matrix.h"
#include "gmath.h"

class Image {

  pixel **pixels;
  double **zbuf;
  int width;
  int height;
  const int MAX = 255;

  void swap(int &x0, int &y0, double &z0,
            int &x1, int &y1, double &z1);

  void order(int &x0, int &y0, double &z0,
             int &x1, int &y1, double &z1,
             int &x2, int &y2, double &z2);

  void initialize(int w, int h);

public:

  Image(int w = 500, int h = 500);

  ~Image();

  int getWidth();

  int getHeight();

  void plot(int x, int y, double z, pixel p);

  void plotLine(int x0, int y0, double z0,
                int x1, int y1, double z1,
                pixel p);

  void plotLines(Edges &m, pixel p);

  void scanline(int x0, int y0, double z0,
                int x1, int y1, double z1,
                int x2, int y2, double z2,
                pixel p);

  void plotPolygons(Polygons &m, pixel p);

  void clear();

  void display();

  void draw(const char *fileName);

  void parse();

};
