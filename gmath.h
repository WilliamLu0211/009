#include "matrix.h"

typedef struct {
  int r, g, b;
} pixel;

typedef struct {
  int x, y, z;
  int r, g, b;
} light;

// lighting functions
pixel get_lighting( double *normal, pixel alight, light plight, double *areflect, double *dreflect, double *sreflect);

pixel calculate_ambient(pixel alight, double *areflect );

pixel calculate_diffuse(light plight, double *dreflect, double *normal );

pixel calculate_specular(light plight, double *sreflect, double *normal );

void limit_color( pixel * c );

// vector functions
void normalize(double *v);

double dot(double *a, double *b);

double *normals(Polygons &p, int i);
