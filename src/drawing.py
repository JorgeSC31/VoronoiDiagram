from manim import *
config.frame_width = 35

class Voronoi(Scene): 
    def construct(self):
        file = open("test.txt","r")

        num_iterations = int(file.readline())
        file.readline() #skip

        for i in range(num_iterations):
            #Draw cells points
            num_points = int(file.readline()) 
            for j in range(num_points):
                l = file.readline()
                s = l.split(" ")
                x = float(s[0])
                y = float(s[1])
                self.add(Dot([x, y, 0], color= PURE_RED, radius = 0.15))

            file.readline() #skip


            #Draw vertices
            num_vertices = int(file.readline()) 
            for j in range(num_vertices):
                l = file.readline()
                s = l.split(" ")
                x = float(s[0])
                y = float(s[1])
                self.add(Dot([x, y, 0], radius = 0.15))

            file.readline() #skip

            #Draw half edges
            num_hedges = int(file.readline())
            for j in range(num_hedges):
                l = file.readline()
                s = l.split(" ")
                x1 = float(s[0])
                y1 = float(s[1])
                x2 = float(s[2])
                y2 = float(s[3])
                self.add(Line([x1, y1, 0], [x2, y2, 0]))
            
            file.readline() #skip
            self.wait(1) #Wait 1 second
            self.clear() #Erase
