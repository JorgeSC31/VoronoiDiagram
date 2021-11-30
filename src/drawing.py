from manim import *
config.frame_width = 15


class Voronoi(Scene):
    def construct(self):
        file = open("res/case2.out", "r")

        num_iterations = int(file.readline())  # Numero de puntos del INPUT

        for i in range(1, num_iterations+1):
            # Draw cells points
            for j in range(i):
                l = file.readline()
                s = l.split(" ")
                x = float(s[0])
                y = float(s[1])
                self.add(Dot([x, y, 0], color=PURE_RED, radius=0.15))

            l = file.readline()
            s = l.split(" ")
            x1 = float(s[0])
            y1 = float(s[1])
            x2 = float(s[2])
            y2 = float(s[3])
            while x1 < 10000:
                dot1 = [x1, y1, 0]
                dot2 = [x2, y2, 0]

                # Add vertices
                self.add(Dot(dot1, radius=0.15))
                self.add(Dot(dot2, radius=0.15))

                # Add half edges
                self.add(Line(dot1, dot2))

                # Next line
                l = file.readline()
                s = l.split(" ")
                x1 = float(s[0])
                y1 = float(s[1])
                x2 = float(s[2])
                y2 = float(s[3])

            self.wait(1)  # Wait 1 second
            self.clear()  # Erase

        file.close()
