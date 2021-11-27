#pragma once

#include "vertex.h"
#include "face.h"

class DirLine {
  public:
    DirLine( Vertex _v1, Vertex _v2 );

  private:
    Vertex v1, v2;
    // Línea que pasa por v1 y v2 en forma Ax + By + C = 0
    float A, B, C;

    void calculate_equation();
};

class Hedge {
  public:
    Hedge();

  private:
    Vertex origin;
    Hedge* twin;
    Face*  incident_face;
    Hedge *next, *prev;
};