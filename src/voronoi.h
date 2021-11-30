#pragma once
#include <vector>
#include "DCEL.h"

class voronoi {
  public:
    voronoi( std::vector< std::pair< float, float > > _pts, float _margin = 0.5 );
    voronoi( std::string file_name, float _margin = 0.5 );

  protected:
    float                 margin;
    std::vector< Vertex > pts;
    std::ofstream         out_file;

    void update_txt();

    std::vector< Face* > faces;

    void incremental_voronoi();
    void insert_first_point();
    void add_voronoi( Vertex );

    std::pair< Hedge*, Hedge* > get_face_intersection( const Face*, DirLine );

    Face*  find_face( Vertex );
    Hedge* cut_face( Face*, Hedge*, Hedge*, DirLine bisec );
    void   fix_twins( Face* );
};