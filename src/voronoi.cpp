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

Hedge* cut_face( Face* div_face, Hedge* inter1, Hedge* inter2, DirLine bisec ) {
    Vertex v_inter1 = line_intersection( inter1, bisec );
    Vertex v_inter2 = line_intersection( inter2, bisec );

    Hedge* incident_div = new Hedge( v_inter1, v_inter2 );
    Hedge* twin         = new Hedge( v_inter2, v_inter1 );

    Hedge* prev = new Hedge( inter1->origin, v_inter1 );
    Hedge* next = new Hedge( v_inter2, inter2->dest );

    // set prev
    prev->prev         = inter1->prev;
    prev->prev->next   = prev;
    prev->next         = incident_div;
    incident_div->prev = prev;

    // set next
    next->prev         = incident_div;
    incident_div->next = next;
    next->next         = inter2->next;
    next->next->prev   = next;

    incident_div->twin = twin;
    twin->twin         = incident_div;

    inter2->next = nullptr;

    return twin;
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
            Hedge* v_hedge = cut_face( div_face, aristas.first, aristas.second, bisec );
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

void voronoi::insert_first_point() {

    float x_min, x_max, y_min, y_max;
    x_min = y_min = -std::numeric_limits< float >::max();
    x_max = y_max = std::numeric_limits< float >::max();

    for ( auto v: pts ) {
        x_min = std::min( x_min, v.x );
        x_max = std::max( x_max, v.x );
        y_min = std::min( y_min, v.y );
        y_max = std::max( y_max, v.y );
    }
    x_min -= margin;
    y_min -= margin;

    x_max += margin;
    y_max += margin;

    // Construir la cara con aristas de rectangulo
    Vertex v1( x_min, y_min );
    Vertex v2( x_max, y_min );
    Vertex v3( x_max, y_max );
    Vertex v4( x_min, y_max );

    Hedge* h1 = new Hedge( v1, v2 );
    Hedge* h2 = new Hedge( v2, v3 );
    Hedge* h3 = new Hedge( v3, v4 );
    Hedge* h4 = new Hedge( v4, v1 );

    h1->prev = h4;
    h1->next = h2;

    h2->prev = h1;
    h2->next = h3;

    h3->prev = h2;
    h3->next = h4;

    h4->prev = h3;
    h4->next = h1;

    Face* v_face = new Face( pts[0] );
    v_face->finish_build( h1 );
}

void voronoi::incremental_voronoi() {
    insert_first_point();
    // update_txt( std::ofstream& file );

    for ( int i = 1; i < pts.size(); i++ ) {
        add_voronoi( pts[i] );
        // update_txt( std::ofstream& file );
    }
}