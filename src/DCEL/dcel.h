#pragma once

#include <vector>

#include "vertex.h"
#include "edge.h"
#include "face.h"

class DCEL {
  public:
    DCEL();

  private:
    std::vector< Vertex > vertices;
    std::vector< Hedge >  hedges;
    std::vector< Face >   faces;
};