#pragma once

#include <vector>

// Declarar todas las clases por que se usan mutuamente.
class Vertex;
class DirLine;
class Hedge;
class Face;
class DCEL;

class Vertex {
  public:
    Vertex( float, float );

  protected:
    float  x, y;
    Hedge* incident_edge;
};

class DirLine {
  public:
    DirLine( Vertex, Vertex );

  protected:
    Vertex *origin, *dest;
    // Ecuación de la recta que pasa por origin y dest, en la forma Ax + By + C = 0.
    // Consideramos está recta con orientación de origin a dest.
    // Esto permite expresar todas las rectas incluyendo verticales.
    float A, B, C;
};

class Hedge: DirLine {
  public:
    Hedge( Vertex, Vertex );

  protected:
    Hedge*  twin;
    Face*   incident_face;
    Vertex *next, *prev;
};

class Face {
  public:
    Face( Vertex );

  protected:
    Vertex* center;
    Hedge*  outer_component;
    // inner_component no es necesario.
};

class DCEL {
  public:
    DCEL();

  protected:
    std::vector< Vertex > vertices;
    std::vector< Hedge >  hedges;
    std::vector< Face >   faces;
};