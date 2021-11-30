from manim import *
# config.frame_width = 14


class Voronoi(Scene):
    def construct(self):
        # file = open("res/case3.out", "r")
        print("Aquí estamos")

        self.add(Dot([0, 0, 0], color=PURE_RED, radius=2))

        self.wait(3)  # Wait 1 second
        self.clear()  # Erase

        # file.close()
