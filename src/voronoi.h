#pragma once
#include <vector>
#include "DCEL.h"

class voronoi {
  public:
    voronoi( std::vector< std::pair< float, float > > _pts );
    voronoi( std::string file_name );

  protected:
    std::vector< Vertex > pts;

    void incremental_voronoi();
};