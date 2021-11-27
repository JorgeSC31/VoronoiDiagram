#include "DCEL.h"

////////////////////////
// Vertex definitions //
////////////////////////

Vertex::Vertex( float _x, float _y ) {
    // No sé si sea conveniente redondear para evitar problemas de precisión.
    x             = _x;
    y             = _y;
    incident_edge = nullptr;
}
void Vertex::finish_build( Hedge _incident_edge ) {
    incident_edge = &_incident_edge;
}

Vertex Vertex::operator+( const Vertex& _ot ) {
    // FIXME
}
Vertex Vertex::operator-( const Vertex& _ot ) {
    // FIXME
}

bool Vertex::operator<( const Vertex& _ot ) {
    // FIXME
}
bool Vertex::operator>( const Vertex& _ot ) {
    // FIXME
}
bool Vertex::operator==( const Vertex& _ot ) {
    // FIXME
    // o tal vez aquí podemos poner que si ||self-ot|| < tol entonces son iguales
}

/////////////////////////
// DirLine definitions //
/////////////////////////
DirLine::DirLine( Vertex _origin, Vertex _dest ) {
    origin = &_origin;
    dest   = &_dest;

    calc_equation();
}

void DirLine::calc_equation() {
    // FIXME
    // Asigna los valores correspondientes de A,B,C
}

///////////////////////
// Hedge definitions //
///////////////////////

Hedge::Hedge( Vertex _origin, Vertex _dest ): DirLine( _origin, _dest ) {
    twin = next = prev = nullptr;
    incident_face      = nullptr;
}

void Hedge::finish_build( Hedge _twin, Face _incident_face, Hedge _next, Hedge _prev ) {
    // FIXME
}

///////////////////////
// Face definitions //
//////////////////////

Face::Face( Vertex _center ) {
    center          = &_center;
    outer_component = nullptr;
}

void Face::finish_build( Hedge _outer_component ) {
    outer_component = &_outer_component;
}

//////////////////////
// DCEL definitions //
//////////////////////

void DCEL::export_txt( std::string file_name ) {
    // FIXME
    // Solo necesitas pasar vertices y aristas.
    // El centro de las caras es un punto de voronoi,
    // hay que hacer distinción de este punto para ponerlo de
    // otro color en el plot.
}

///////////////////////
// Extra definitions //
///////////////////////

DirLine bisector( Vertex v1, Vertex v2 ) {
    // FIXME
    // Se regresa un DirLine para tener una orientación.
    // Realmente solo importa la recta y no tanto origin y dest.
    // Podrías ponerlo como vector normal al segmento v1-v2
    // y que pase por su punto medio.
}

bool is_intersection( Hedge arista, DirLine bisec ) {
    // FIXME
    // Regresa true/false si hay intersección.
    // La arista tiene origin y dest, pero solo nos interesa
    // si hay intersección en (origin, dest] con bisec (la recta completa).
}

Vertex line_intersection( Hedge arista, DirLine bisec ) {
    // FIXME
    // Supón que si hay un único punto intersección.
}