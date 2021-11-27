#include <iostream>

#include "voronoi.h"

int main() {
    int N = 10; //Numero de puntos del INPUT

    //Crear txt e imprimir en el txt el número de puntos del INPUT
    std::ofstream out_file("voronoi_diagram.txt");
    out_file<< N <<"\n";

    return 0;
}
