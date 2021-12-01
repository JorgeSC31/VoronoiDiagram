#include <limits>

#include "voronoi.h"

voronoi::voronoi( std::vector< std::pair< float, float > > _pts, float _margin ) {
    // FIXME
    // No es escencial este constructor.
    // Solo debe pasar los _pts a Vertex.
    margin = _margin;

    incremental_voronoi();
}

voronoi::voronoi( std::string file_name, float _margin ):
    margin( _margin ), out_file( std::ofstream( file_name + ".out" ) ) {
    // FIXME
    // Debe leer el número de puntos N
    // Después N lineas con 2 float cada uno
    // hay que ponerlos en el vector<Vertex> pts
    margin = _margin;

    std::string input_file_name = file_name + ".in";

    int           N;
    float         x, y;
    std::ifstream in_file( input_file_name );

    in_file >> N;

    for ( int j = 0; j < N; j++ ) {
        in_file >> x >> y;
        Vertex v( x, y );
        pts.push_back( v );
    }
    in_file.close();

    // abrir file de output
    out_file << N << '\n';
    incremental_voronoi();
    out_file.close();
}

void voronoi::update_txt() {
    for ( auto f: faces ) {
        Vertex v = f->get_center();
        out_file << v.x << ' ' << v.y << '\n';
    }
    for ( auto f: faces ) {
        Hedge* inicio = f->get_outer_component();
        Hedge* edge   = inicio;

        do {
            Vertex v1 = edge->origin;
            Vertex v2 = edge->dest;
            out_file << v1.x << ' ' << v1.y << ' ' << v2.x << ' ' << v2.y << '\n';

            edge = edge->next;
        } while ( edge != inicio );
    }
    out_file << "10000 10000 10000 10000\n";
}

std::pair< Hedge*, Hedge* > voronoi::get_face_intersection( const Face* div_face,
                                                            DirLine     bisec ) {

    Hedge *h1 = nullptr, *h2 = nullptr;
    Hedge* edge       = div_face->get_outer_component();
    Hedge* first_edge = edge;

    do {
        if ( is_intersection( *edge, bisec ) ) {
            if ( bisec.IsRight( edge->origin ) )
                h1 = edge;
            else
                h2 = edge;
        }
        edge = edge->next;
    } while ( edge != first_edge );

    if ( h1 == nullptr )
        h2 = nullptr;
    return std::make_pair( h1, h2 );
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

Hedge* voronoi::cut_face( Face* div_face, Hedge* inter1, Hedge* inter2, DirLine bisec ) {
    // Puede suceder el caso que v_inter2 sea igual a inter2->dest
    Vertex v_inter1 = line_intersection( inter1, bisec );
    Vertex v_inter2 = line_intersection( inter2, bisec );

    Hedge* incident_div = new Hedge( v_inter1, v_inter2 );
    Hedge* twin         = new Hedge( v_inter2, v_inter1 );

    Hedge* prev = new Hedge( inter1->origin, v_inter1 );
    Hedge* next = new Hedge( v_inter2, inter2->dest );

    // set prev
    prev->prev          = inter1->prev;
    prev->prev->next    = prev;
    prev->next          = incident_div;
    incident_div->prev  = prev;
    prev->incident_face = div_face;

    // Prueba
    prev->twin = inter1->twin;
    // inter1->twin = prev;

    // set next
    if ( v_inter2 == inter2->dest ) {
        incident_div->next       = inter2->next;
        incident_div->next->prev = incident_div;
    } else {
        next->prev          = incident_div;
        incident_div->next  = next;
        next->next          = inter2->next;
        next->next->prev    = next;
        next->incident_face = div_face;
        // Prueba
        next->twin = inter2->twin;
        // inter2->twin = next;
    }
    incident_div->incident_face = div_face;

    incident_div->twin = twin;
    twin->twin         = incident_div;

    inter2->next = nullptr;

    div_face->finish_build( incident_div );
    return twin;
}

void voronoi::fix_twins( Face* v_face ) {
    Hedge* first_edge = v_face->get_outer_component();
    Hedge* edge       = first_edge;
    Hedge *adj1, *adj2;

    do {
        if ( edge->twin != nullptr && edge->prev->twin != nullptr ) {
            adj1       = edge->twin->next;
            adj2       = edge->prev->twin->prev;
            adj1->twin = adj2;
            adj2->twin = adj1;
        }

        edge = edge->next;
    } while ( edge != first_edge );
}

void voronoi::add_voronoi( Vertex v ) {
    // Buscar en que cara esta v.
    Face* original_face = find_face( v );
    Face* div_face      = original_face;

    Face* v_face = new Face( v );
    faces.push_back( v_face );

    bool add_prev = false;

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

            if ( add_prev ) {
                // Agregar arista al siguiente vertice.
                v_face->push( v_hedge->origin );
                add_prev = false;
            }
            v_face->push( v_hedge );
        }

        // Hay que movernos a la siguiente cara.
        // Si estamos en la frontera la siguiente cara es nullptr, esto es un caso especial.
        if ( aristas.first->twin != nullptr ) {
            // Existe la cara adyacente.
            div_face = aristas.first->twin->incident_face;
            continue;
        }
        // Si no existe, nos vamos a mover sobre la frontera hasta encontrar la siguiente
        // arista que intersecta un bisector.
        // Las aristas nuevas que se creen durante este proceso tienen twin=nullptr.
        Hedge* edge_frontier = aristas.first;
        Vertex prev_inter    = line_intersection( edge_frontier, bisec );

        bool flag = true;

        // while ( edge_frontier->twin == nullptr && edge_frontier != aristas.second ) {
        while ( flag ) {
            // Si la siguiente arista es solo un punto lo ignoramos.
            if ( prev_inter == edge_frontier->dest ) {
                edge_frontier = edge_frontier->next;
                continue;
            }
            if ( are_collinear( v_face->get_outer_component()->origin, prev_inter,
                                edge_frontier->dest ) ) {
                prev_inter = v_face->get_outer_component()->origin;
                v_face->pop();
            }
            Hedge* v_hedge = new Hedge( prev_inter, edge_frontier->dest );

            v_face->push( v_hedge );
            prev_inter = edge_frontier->dest;

            edge_frontier = edge_frontier->next;
            if ( edge_frontier->twin != nullptr )
                edge_frontier = edge_frontier->twin->next;

            Vertex  v_frontier  = edge_frontier->incident_face->get_center();
            DirLine bisec_front = bisector( v, v_frontier );
            flag                = !is_intersection( *edge_frontier, bisec_front );
        }

        add_prev = true;
        div_face = edge_frontier->incident_face;

    } while ( div_face != original_face );
    v_face->close( add_prev );
    // fix_twins( v_face );
}

void voronoi::insert_first_point() {

    float x_min, x_max, y_min, y_max;
    x_min = y_min = std::numeric_limits< float >::max();
    x_max = y_max = -std::numeric_limits< float >::max();

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
    Face* v_face = new Face( pts[0] );
    faces.push_back( v_face );

    Vertex v1( x_min, y_min );
    Vertex v2( x_max, y_min );
    Vertex v3( x_max, y_max );
    Vertex v4( x_min, y_max );

    Hedge* h1 = new Hedge( v1, v2 );
    Hedge* h2 = new Hedge( v2, v3 );
    Hedge* h3 = new Hedge( v3, v4 );
    Hedge* h4 = new Hedge( v4, v1 );

    h1->prev          = h4;
    h1->next          = h2;
    h1->incident_face = v_face;

    h2->prev          = h1;
    h2->next          = h3;
    h2->incident_face = v_face;

    h3->prev          = h2;
    h3->next          = h4;
    h3->incident_face = v_face;

    h4->prev          = h3;
    h4->next          = h1;
    h4->incident_face = v_face;

    v_face->finish_build( h1 );
}

void voronoi::incremental_voronoi() {
    insert_first_point();
    update_txt();
    for ( int i = 1; i < pts.size(); i++ ) {
        add_voronoi( pts[i] );
        update_txt();
    }
}