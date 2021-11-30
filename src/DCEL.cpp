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
void Vertex::finish_build( Hedge* _incident_edge ) {
    incident_edge = _incident_edge;
}

float Vertex::norm() {
    return sqrt( x * x + y * y );
}

Vertex Vertex::operator+( const Vertex& _ot ) const {
    Vertex res( x + _ot.x, y + _ot.y );
    return res;
}
Vertex Vertex::operator-( const Vertex& _ot ) const {
    Vertex res( x - _ot.x, y - _ot.y );
    return res;
}

// bool Vertex::operator<( const Vertex& _ot ) const {
//     // FIXME
// }
// bool Vertex::operator>( const Vertex& _ot ) const {
//     // FIXME
// }
bool Vertex::operator==( const Vertex& _ot ) const {
    // o tal vez aquí podemos poner que si ||self-ot|| < tol entonces son iguales
    double tol = 0.000000001;
    if ( ( *this - _ot ).norm() < tol )
        return true;
    else
        return false;
}

/////////////////////////
// DirLine definitions //
/////////////////////////
DirLine::DirLine( Vertex _origin, Vertex _dest ) {
    origin = _origin;
    dest   = _dest;

    calc_equation();
}

void DirLine::calc_equation() {
    // FIXME
    // Asigna los valores correspondientes de A,B,C
    A = dest.y - origin.y;
    B = origin.x - dest.x;
    C = A * ( origin.x ) + B * ( origin.y );
}

bool DirLine::IsLeft( const Vertex& v ) const {
    // Dice si el vertice v esta a la izquierda del segmento dirigido
    if ( determinant( dest - origin, v - origin ) > 0 )
        return true;
    else
        return false;
}

bool DirLine::IsRight( const Vertex& v ) const {
    // Dice si el vertice v esta a la izquierda del segmento dirigido
    if ( determinant( dest - origin, v - origin ) < 0 )
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

void Hedge::finish_build( Hedge* _twin, Face* _incident_face, Hedge* _next, Hedge* _prev ) {
    twin          = _twin;
    incident_face = _incident_face;
    next          = _next;
    prev          = _prev;
}

///////////////////////
// Face definitions //
///////////////////////

Face::Face( Vertex _center ) {
    center          = _center;
    outer_component = nullptr;
}

void Face::finish_build( Hedge* _outer_component ) {
    outer_component = _outer_component;
}

void Face::push( Hedge* next_hedge ) {
    if ( outer_component == nullptr ) {
        outer_component = next_hedge;
        return;
    }

    outer_component->next = next_hedge;
    next_hedge->prev      = outer_component;
    outer_component       = next_hedge;
}
void Face::pop() {
    if ( outer_component == nullptr )
        return;
    outer_component = outer_component->prev;
    if ( outer_component != nullptr )
        outer_component->next = nullptr;
}

void Face::close( bool add_hedge = false ) {
    Hedge* last  = outer_component;
    Hedge* first = outer_component;

    while ( first->prev != nullptr )
        first = first->prev;

    if ( add_hedge ) {
        Hedge* n_hedge = new Hedge( last->dest, first->origin );
        last->next     = n_hedge;
        n_hedge->prev  = last;
        last           = n_hedge;
    }
    last->next  = first;
    first->prev = last;
}

Vertex Face::get_center() {
    return center;
}
Hedge* Face::get_outer_component() const {
    return outer_component;
}

//////////////////////
// DCEL definitions //
//////////////////////

void DCEL::update_txt( std::ofstream& file ) {
    // Solo necesitas pasar vertices y aristas.
    // El centro de las caras es un punto de voronoi,
    // hay que hacer distinción de este punto para ponerlo de
    // otro color en el plot.

    // Nota: Esta función sólo imprime la información de la DCEL. En cada iteración se debe
    // imprimir primero el número de iteración i seguido de los primeros i puntos del INPUT,
    // luego se llama esta función para imprimir la DCEL

    int num_vertices = vertices.size();
    file << num_vertices << "\n";
    for ( int j = 0; j < num_vertices; j++ ) {
        Vertex v = vertices[j];
        file << v.x << " " << v.y << "\n";
    }

    int num_hedges = hedges.size();
    file << num_hedges << "\n";
    for ( int j = 0; j < num_hedges; j++ ) {
        Hedge arista = hedges[j];

        Vertex v1 = arista.origin;
        Vertex v2 = arista.dest;
        file << v1.x << " " << v1.y << " " << v2.x << " " << v2.y << "\n";
    }
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

    // Para que esto jale bien v1 != v2
    Vertex p1( ( v1.x + v2.x ) / 2, ( v1.y + v2.y ) / 2 );
    Vertex r = v2 - v1;
    Vertex ortogonal( -r.y, r.x );
    Vertex p2 = p1 + ortogonal;
    return DirLine( p1, p2 );
}

float determinant( Vertex a, Vertex b ) {
    return ( a.x ) * ( b.y ) - ( a.y ) * ( b.x );
}

bool is_intersection( Hedge arista, DirLine bisec ) {
    // FIXME
    // Regresa true/false si hay intersección.
    // La arista tiene origin y dest, pero solo nos interesa
    // si hay intersección en (origin, dest] con bisec (la recta completa).

    Vertex v1 = arista.origin;
    Vertex v2 = arista.dest;

    // Checar el caso en el que origin esta sobre la recta
    if ( bisec.IsLeft( v1 ) == false && bisec.IsRight( v1 ) == false ) {
        // caso en el que el arista esta completamente sobre la recta
        if ( bisec.IsLeft( v2 ) == false && bisec.IsRight( v2 ) == false )
            return true;
        else
            return false;
    }

    if ( bisec.IsLeft( v1 ) != bisec.IsLeft( v2 ) )
        return true;
    else
        return false;
}

Vertex line_intersection( const Hedge* arista, DirLine bisec ) {
    // FIXME
    // Supón que si hay un único punto intersección.

    Vertex v1 = arista->origin;
    Vertex v2 = arista->dest;

    DirLine line( v1, v2 );

    float det = bisec.A * line.B - line.A * bisec.B;

    float x = ( line.B * bisec.C - bisec.B * line.C ) / det;
    float y = ( bisec.A * line.C - line.A * bisec.C ) / det;
    return Vertex( x, y );
}

float dist( Vertex v1, Vertex v2 ) {
    Vertex v = v1 - v2;
    return v.norm();
}