#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <limits>

#include "image.h"

using namespace std;

void Image::swap(int &x0, int &y0, double &z0,
                 int &x1, int &y1, double &z1){
  int tempI = x0;
  x0 = x1;
  x1 = tempI;
  tempI = y0;
  y0 = y1;
  y1 = tempI;
  double tempD = z0;
  z0 = z1;
  z1 = tempD;
}

void Image::order(int &x0, int &y0, double &z0,
                  int &x1, int &y1, double &z1,
                  int &x2, int &y2, double &z2){
  if (y0 > y1){
    swap(x0, y0, z0, x1, y1, z1);
  }
  if (y1 > y2){
    swap(x1, y1, z1, x2, y2, z2);
    if (y0 > y1)
    swap(x0, y0, z0, x1, y1, z1);
  }
}

Image::Image(int w, int h){
  width = w;
  height = h;
  initialize(width, height);
}

Image::~Image(){
  for (int i = 0; i < width; i ++){
    delete[] pixels[i];
    delete[] zbuf[i];
  }
  delete[] pixels;
  delete[] zbuf;
}

void Image::initialize(int w, int h){
  double init = numeric_limits<double>::lowest();
  pixels = new pixel*[w];
  zbuf = new double*[w];
  for (int i = 0; i < w; i ++){
    pixels[i] = new pixel[h];
    zbuf[i] = new double[h];
    for (int j = 0; j < h; j ++)
      zbuf[i][j] = init;
  }
}

int Image::getWidth(){
  return width;
}

int Image::getHeight(){
  return height;
}

void Image::plot(int x, int y, double z, pixel p){
  if (x >= 0 && x < width &&
      y >= 0 && y < height &&
      z >= zbuf[x][y]){
    pixels[x][y] = p;
    zbuf[x][y] = z;
  }
}

void Image::plotLine(int x0, int y0, double z0,
                     int x1, int y1, double z1,
                     pixel p){
  if (x0 > x1)
    swap(x0, y0, z0, x1, y1, z1);

  // cout << x0 << x1;
  int x = x0, y = y0;
  double z = z0, dz = z1 - z0;
  int a = y1 - y0, b = x0 - x1;
  int d;
  if (a >= 0){

    if (a <= -b){
      dz /= (x1 - x0);
      d = 2 * a + b;
      while (x < x1){
        plot(x, y, z, p);
        if (d > 0){
          y ++;
          d += 2 * b;
        }
        x ++;
        d += 2 * a;
        z += dz;
      }
    }
    else {
      dz /= (y1 - y0);
      d = a + 2 * b;
      while (y < y1){
        plot(x, y, z, p);
        if (d < 0){
          x ++;
          d += 2 * a;
        }
        y ++;
        d += 2 * b;
        z += dz;
      }
    }
  }
  else {

    if (a >= b){
      // cout << "#";
      dz /= (x1 - x0);
      d = 2 * a + b;
      while (x < x1){
        plot(x, y, z, p);
        if (d < 0){
          y --;
          d -= 2 * b;
        }
        x ++;
        d += 2 * a;
        z += dz;
      }
    }
    else {
      // cout << "&";
      dz /= (y0 - y1);
      d = a + 2 * b;
      while (y > y1){
        plot(x, y, z, p);
        if (d > 0){
          x ++;
          d += 2 * a;
        }
        y --;
        d -= 2 * b;
        z += dz;
      }
    }
  }
  plot(x1, y1, z1, p);
}

void Image::plotLines(Edges &m, pixel p){
  // m.print();
  for (int i = 0; i < m.getCols(); i += 2)
    plotLine(m.getX(i), m.getY(i), m.getZ(i),
             m.getX(i+1), m.getY(i+1), m.getZ(i+1),
             p);
}

void Image::scanline(int bx, int by, double bz,
                     int mx, int my, double mz,
                     int tx, int ty, double tz,
                     pixel p){
  p.r = rand() % 256;
  p.g = rand() % 256;
  p.b = rand() % 256;
  order(bx, by, bz, mx, my, mz, tx, ty, tz);
  double x0 = bx, x1 = bx;
  int y = by;
  double z0 = bz, z1 = bz;
  while (y < my){
    plotLine(x0, y, z0, x1, y, z1, p);
    x0 += (double)(tx - bx) / (ty - by);
    x1 += (double)(mx - bx) / (my - by);
    z0 += (tz - bz) / (ty - by);
    z1 += (mz - bz) / (my - by);
    y ++;
  }
  x1 = mx;
  z1 = mz;
  while (y < ty){
    plotLine(x0, y, z0, x1, y, z1, p);
    x0 += (double)(tx - bx) / (ty - by);
    x1 += (double)(tx - mx) / (ty - my);
    z0 += (tz - bz) / (ty - by);
    z1 += (tz - mz) / (ty - my);
    y ++;
  }
  plot(tx, ty, tz, p);
}

void Image::plotPolygons(Polygons &m, pixel p){
  double vx0, vy0, vx1, vy1;
  for (int i = 0; i < m.getCols(); i += 3){
    vx0 = m.getX(i+1) - m.getX(i);
    vy0 = m.getY(i+1) - m.getY(i);
    vx1 = m.getX(i+2) - m.getX(i);
    vy1 = m.getY(i+2) - m.getY(i);
    if (vx0 * vy1 > vy0 * vx1)
      scanline(m.getX(i), m.getY(i), m.getZ(i),
               m.getX(i+1), m.getY(i+1), m.getZ(i+1),
               m.getX(i+2), m.getY(i+2), m.getZ(i+2),
               p);
  }
}

void Image::clear(){
  pixel p{0, 0, 0};
  double init = numeric_limits<double>::lowest();
  cout << init << endl;
  for (int i = 0; i < width; i ++)
    for (int j = 0; j < height; j ++){
      pixels[i][j] = p;
      zbuf[i][j] = init;
    }
}

void Image::display(){
  FILE *f = popen("display", "w");
  fprintf(f, "P3 %d %d %d\n", width, height, MAX);
  for (int j = height - 1; j >= 0; j --)
    for (int i = 0; i < width; i ++)
      fprintf(f, "%d %d %d ", pixels[i][j].r, pixels[i][j].g, pixels[i][j].b);
  pclose(f);
}

void Image::draw(const char *fileName){
  string s = "convert - ";
  s += fileName;
  FILE *f = popen(s.c_str(), "w");
  fprintf(f, "P3 %d %d %d\n", width, height, MAX);
  for (int j = height - 1; j >= 0; j --)
    for (int i = 0; i < width; i ++)
      fprintf(f, "%d %d %d ", pixels[i][j].r, pixels[i][j].g, pixels[i][j].b);
  pclose(f);
}

void Image::parse(){
  stack<Transform> s;
  // Edges &e = *(new Edges());
  Edges e;
  // Polygons &p = *(new Polygons());
  Polygons p;
  string command;
  char file[16];
  char axis;
  // Transform &ident = *(new Transform());
  // ident.print();
  s.push(*(new Transform));
  // s.top().print();
  // Matrix temp;
  double x, y, z, th, r1, r2, w, h, d;
  double x0, y0, z0, x1, y1, z1, x2, y2, x3, y3;
  while (getline(cin, command)){
    cout << command << endl;
    if (command == "line"){
      cin >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
      e.addEdge(x0, y0, z0, x1, y1, z1);
      s.top().mult(e);
      plotLines(e, pixel{255, 0, 0});
      e.clear();
    }
    else if (command == "circle"){
      cin >> x >> y >> z >> r1;
      e.addCircle(x, y, z, r1);
      s.top().mult(e);
      plotLines(e, pixel{0, 255, 0});
      e.clear();
    }
    else if (command == "box"){
      cin >> x >> y >> z >> w >> h >> d;
      p.addBox(x, y, z, w, h, d);
      s.top().mult(p);
      plotPolygons(p, pixel{0, 0, 255});
      p.clear();
    }
    else if (command == "sphere"){
      cin >> x >> y >> z >> r1;
      p.addSphere(x, y, z, r1);
      s.top().mult(p);
      plotPolygons(p, pixel{128, 128, 0});
      p.clear();
    }
    else if (command == "torus"){
      cin >> x >> y >> z >> r1 >> r2;
      p.addTorus(x, y, z, r1, r2);
      s.top().mult(p);
      plotPolygons(p, pixel{128, 0, 128});
      p.clear();
    }
    else if (command == "hermite"){
      cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
      e.addCurve(x0, y0, x1, y1, x2, y2, x3, y3, HERMITE);
      s.top().mult(e);
      plotLines(e, pixel{0, 128, 128});
      e.clear();
    }
    else if (command == "bezier"){
      cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
      e.addCurve(x0, y0, x1, y1, x2, y2, x3, y3, BEZIER);
      s.top().mult(e);
      plotLines(e, pixel{128, 128, 128});
      e.clear();
    }
    else if (command == "scale"){
      cin >> x >> y >> z;
      Transform &t = *(new Scale(x, y, z));
      s.top().mult(t);
      s.pop();
      s.push(t);
      // temp->mult(s.top());
      // delete temp;
    }
    else if (command == "move"){
      cin >> x >> y >> z;
      Transform &t = *(new Translate(x, y, z));
      s.top().mult(t);
      s.pop();
      s.push(t);
    }
    else if (command == "rotate"){
      cin >> axis >> th;
      th *= M_PI / 180;
      Transform *temp;
      if (axis == 'x'){
        temp = new RotX(th);
      }
      else if (axis == 'y'){
        temp = new RotY(th);
      }
      else if (axis == 'z'){
        temp = new RotZ(th);
      }
      // temp->mult(s.top());
      // temp->print();
      // s.top().print();
      // delete temp;
      Transform &t = *temp;
      s.top().mult(t);
      s.pop();
      s.push(t);
    }
    else if (command == "push"){
      // temp = new Transform();
      // s.top().copy(*temp);

      // top.print();
      // Transform t = s.top();
      // t.print();
      Transform &t = *(new Transform());
      s.top().copy(t);
      s.push(t);
      // s.top().print();
    }
    else if (command == "pop"){
      // s.top().print();
      s.pop();
    }
    else if (command == "clear"){
      clear();
    }
    else if (command == "display"){
      // clear();
      // plotLines(e, pixel{255, 0, 0});
      // plotPolygons(p, pixel{0, 0, 255});
      display();
    }
    else if (command == "save"){
      // clear();
      cin >> file;
      // plotLines(e, pixel{255, 0, 0});
      // plotPolygons(p, pixel{0, 0, 255});
      draw(file);
    }
    // else
    //   cout << command << endl;
  }
}
