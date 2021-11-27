#pragma once

#include <vector>
#include <string>
#include <cmath>

// Declarar todas las clases por que se usan mutuamente.
class Vertex;
class DirLine;
class Hedge;
class Face;
class DCEL;

class Vertex {
  public:
    Vertex( float, float );

    Vertex(){}; //Agregé un constructor vacío 

    void finish_build( Hedge );

    float norm();

    Vertex operator+( const Vertex& );
    Vertex operator-( const Vertex& );

    bool operator<( const Vertex& );
    bool operator>( const Vertex& );
    bool operator==( const Vertex& );

    float  x, y; //puse a las coordenadas publicas para poderlas usar en otros lados

  protected:
    //float  x, y;
    Hedge* incident_edge;
};

class DirLine {
  public:
    DirLine( Vertex, Vertex );

    bool IsLeft(Vertex * );


    // Ecuación de la recta que pasa por origin y dest, en la forma Ax + By = C.
    // Consideramos está recta con orientación de origin a dest.
    // Esto permite expresar todas las rectas incluyendo verticales.
    float A, B, C;

  protected:
    Vertex *origin, *dest;
    

  private:
    void calc_equation();
};

class Hedge: public DirLine {
  public:
    Hedge( Vertex, Vertex );
    void finish_build( Hedge _twin, Face _incident_face, Hedge _next, Hedge _prev );

    Vertex *origin;
    Hedge* twin;
  protected:
    
    Face*  incident_face;
    Hedge *next, *prev;
    
};

class Face {
  public:
    Face( Vertex );

    void finish_build( Hedge _outer_component );

  protected:
    Vertex* center;
    Hedge*  outer_component;
    // inner_component no es necesario.
};

class DCEL {
  public:
    DCEL();

    void export_txt( std::string file_name );

  protected:
    std::vector< Vertex > vertices;
    std::vector< Hedge >  hedges;
    std::vector< Face >   faces;
};