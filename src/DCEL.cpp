#include "DCEL.h"

////////////////////////
// Vertex definitions //
////////////////////////

Vertex::Vertex( float _x, float _y ) {
    x             = _x;
    y             = _y;
    incident_edge = nullptr;
}
void Vertex::finish_build( Hedge _incident_edge ) {
    incident_edge = &_incident_edge;
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
}

///////////////////////
// Extra definitions //
///////////////////////
