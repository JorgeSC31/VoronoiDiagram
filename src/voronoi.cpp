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
    Face* div_face      = original_face;

    Face* v_face = new Face( v );
    faces.push_back( v_face );

    bool add_hedge = false;

    do {
        /*
            Cada cara donde v tenga región va a cortar en
            1 arista: En este caso saltamos a la siguiente cara.
            2 aristas: Esto crea una nueva arista y corta la cara.
        */
        DirLine                     bisec   = bisector( v, div_face->get_center() );
        std::pair< Hedge*, Hedge* > aristas = get_face_intersection( div_face, bisec );
        /*
            get_face_intersection tiene orientación.
              ___<<<______________arista.first
              |       ^          |
              |       |          |
              |   v---|---center |
              |       |          |
              |_______|______>>>_|arista.second

        */
        if ( aristas.second != nullptr ) {
            // Regresa el twin de la arista creada, ie. el Hedge nuevo para la región de v.
            // Además corta el poligono y pone arista.second->next = nullptr.
            // No elimina la cadena de aristas, ya que se va a ocupar después.
            // Las Hedges de caras adyacentes quedan inmutables.
            Hedge* v_hedge         = cut_face( div_face, aristas.first, aristas.second );
            v_hedge->incident_face = v_face;
            v_face->push( v_hedge );
        }

        // Hay que movernos a la siguiente cara.
        // Si estamos en la frontera la siguiente cara es nullptr, esto es un caso especial.
        if ( aristas.first->twin != nullptr ) {
            // Existe la cara adyacente.
            div_face = aristas.first->twin->incident_face;
            continue;
        }
        // Si no existe, nos vamos a mover sobre la antigua frontera, que es la cadena de
        // aristas que inicia con aristas.first.
        // Las aristas nuevas que se creen durante este proceso tienen twin=nullptr.
        Hedge* edge_frontier = aristas.first;
        Vertex prev_inter    = line_intersection( edge_frontier, bisec );

        while ( edge_frontier->twin == nullptr && edge_frontier != aristas.second ) {
            Hedge* v_hedge = new Hedge( prev_inter, edge_frontier->dest );
            v_face->push( v_hedge );
            edge_frontier = edge_frontier->next;
        }

        if ( edge_frontier == aristas.second ) {
            add_hedge = true;
            break;
        }
        div_face = edge_frontier->twin->incident_face;

    } while ( div_face != original_face );
    v_face->close( add_hedge );
}

void voronoi::incremental_voronoi() {
    insert_first_point();

    for ( int i = 1; i < pts.size(); i++ ) {
        add_voronoi( pts[i] );
    }
}