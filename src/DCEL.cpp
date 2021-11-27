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

float Vertex::norm(){
    return sqrt(x*x + y*y);
}

Vertex Vertex::operator+( const Vertex& _ot ) {
    // FIXME
    Vertex res(x + _ot.x, y + _ot.y);
    return res;

}
Vertex Vertex::operator-( const Vertex& _ot ) {
    // FIXME
    Vertex res(x - _ot.x, y - _ot.y);
    return res;
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
    double tol = 0.000000001;
    if( (*this - _ot).norm() < tol )
        return true;
    else
        return false;
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
    A = dest->y - origin->y;
    B = origin->x - dest->x;
    C = A*(origin->x) + B*(origin->y);
}

bool DirLine::IsLeft(Vertex *v){
    //Dice si el vertice v esta a la izquierda del segmento dirigido
    if(determinant(*dest - *origin, *v - *origin)  > 0)
        return true;
    else
        return false;
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


    //El único error que habría es si v1 = v2
    Vertex p1((v1.x + v2.x)/2, (v1.y + v2.y)/2);
    Vertex r = v2 - v1;
    Vertex ortogonal(-r.y, r.x);
    Vertex p2 = p1 + ortogonal;
    return DirLine(p1, p2);
    

}

float determinant(Vertex a, Vertex b){
    return (a.x)*(b.y)-(a.y)*(b.x);
}

bool is_intersection( Hedge arista, DirLine bisec ) {
    // FIXME
    // Regresa true/false si hay intersección.
    // La arista tiene origin y dest, pero solo nos interesa
    // si hay intersección en (origin, dest] con bisec (la recta completa).

    Vertex *v1 = arista.origin;
    Vertex *v2 = arista.twin->origin;

    if(bisec.IsLeft(v1) != bisec.IsLeft(v2)) 
        return true;
    else
        return false;
}

Vertex line_intersection( Hedge arista, DirLine bisec ) {
    // FIXME
    // Supón que si hay un único punto intersección.

    Vertex v1 = *(arista.origin);
    Vertex v2 = *(arista.twin->origin);

    DirLine line(v1, v2);

    float det = bisec.A*line.B - line.A*bisec.B;

    float x = (line.B*bisec.C- bisec.B*line.C)/det;
    float y = (bisec.A*line.C - line.A*bisec.C)/det;
    return Vertex(x, y);


}