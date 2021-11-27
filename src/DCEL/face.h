#pragma once

#include <vector>

#include "edge.h"

class Face {
  public:
    Face();

  private:
    std::vector< Hedge > outer_component;

    Vertex center;
};