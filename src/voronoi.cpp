#include <limits>

#include "voronoi.h"

voronoi::voronoi( std::vector< std::pair< float, float > > _pts, float _margin = 1.0 ) {
    // FIXME
    // No es escencial este constructor.
    // Solo debe pasar los _pts a Vertex.
    margin = _margin;

    incremental_voronoi();
}

voronoi::voronoi( std::string file_name, float _margin = 1.0 ) {
    // FIXME
    // Debe leer el número de puntos N
    // Después N lineas con 2 float cada uno
    // hay que ponerlos en el vector<Vertex> pts
    margin = _margin;

    int           N;
    float         x, y;
    std::ifstream file( file_name );

    file >> N;

    for ( int j = 0; j < N; j++ ) {
        file >> x >> y;
        Vertex v( x, y );
        pts.push_back( v );
    }
    file.close();

    incremental_voronoi();
}

Face* voronoi::find_face( Vertex v ) {
    Face* ret      = nullptr;
    float min_dist = std::numeric_limits< float >::max();

    for ( auto n_face: faces ) {
        float n_dist = dist( v, n_face->get_center() );
        if ( n_dist < min_dist ) {
            min_dist = n_dist;
            ret      = n_face;
        }
    }
    return ret;
}

void voronoi::add_voronoi( Vertex v ) {
    // Buscar en que cara esta v.
    Face* original_face = find_face( v );
}

void voronoi::incremental_voronoi() {
    insert_first_point();

    for ( int i = 1; i < pts.size(); i++ ) {
        add_voronoi( pts[i] );
    }
}