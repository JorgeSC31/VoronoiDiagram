# Diagrama de Voronoi 

Se [implementó el algoritmo incremental](src/voronoi.cpp) para calcular el diagrama de Voronoi para N puntos dados en el plano (2 dimensiones). Para ello se realizó también la [implementación de la DCEL](src/DCEL.cpp), "Lista doblemente conectada de aristas" por sus siglas en inglés. El repositorio incluye un [programa en python para la animación del algoritmo](src/drawing.py). 

La implementación del diagrama de Voronoi y de la DCEL se realizaron en c++. La animación fue realizada en python con el uso de manim. Para obtener la animación es necesaria su instalación en https://www.manim.community/.

El programa [main.cpp](src/main.cpp) lee un archivo de texto de entrada. El cual contiene en su primera linea el entero N de número de puntos para el que se calculará el diagrama de Voronoi. En las siguientes N lineas se leen las coordenadas de los N puntos separadas por un espacio. Luego, se calcula el diagrama de Voronoi con el algoritmo incremental. En cada iteracion del algoritmo se exporta en archivo de texto de salida los aristas de la DCEL. Cuando termina el programa se puede general la animación del algortimo ejecutando:

~~~
manim -p drawing.py Voronoi  
~~~

Esto generará un video .mp4 en alta calidad. Para obtener un video de manera más rápida pero de calidad menor ejecutar:

~~~
manim -pql drawing.py Voronoi  
~~~

El programa [drawing.py](src/drawing.py) realiza la animación leyendo el archivo de texto de salida generado anteriormente. 

Se probó el algoritmo con tres casos:
- Caso 1: Con [gen_ran_points.py](src/gen_ran_points.py) se generaron 100 números aleatorios que se imprimen en [case1.in](res/case1.in).
- Caso 2: Con [gen_grid.py](src/grid_generator.py) se generó una cuadrícula de 20 puntos que se imprimen en [case2.in](res/case2.in).
- Caso 3: Con [gen_circle_points.py](src/gen_circle_points.py) se generaron 10 números sobre una circunferencia y se agregá el punto central de la circunferencia. Los puntos se imprimen en [case3.in](res/case3.in).

Luego, se generaron los archivos de salida del algoritmo y se animaron obteniendo los videos que se encuentran [aquí](src/media/videos).
