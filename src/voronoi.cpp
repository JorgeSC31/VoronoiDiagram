#include "voronoi.h"

voronoi::voronoi( std::vector< std::pair< float, float > > _pts ) {
    // FIXME
    // No es escencial este constructor.
    // Solo debe pasar los _pts a Vertex.

    incremental_voronoi();
}

voronoi::voronoi( std::string file_name  ) {
    // FIXME
    // Debe leer el número de puntos N
    // Después N lineas con 2 float cada uno
    // hay que ponerlos en el vector<Vertex> pts

    int N;
    float x, y;
    std::ifstream file(file_name);
    
    file >> N;

    for(int j = 0; j < N; j++){
        file >> x >> y;
        Vertex v(x, y);
        pts.push_back(v);
    }
    file.close();
    incremental_voronoi();
}

void voronoi::incremental_voronoi() {}