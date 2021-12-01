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

Para probar el algoritmo se agregó también el archivo [gen_cases.py](src/gen_cases.py) para generar archivos de entrada con puntos aleatorios de distribución uniforme.