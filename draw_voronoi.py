import matplotlib.pyplot as plt
import numpy as np


def read_voronoi_info():
    # read voronoi input
    with open('site_points.txt', 'r') as voronoi_input:
        line = voronoi_input.readline()
        _, width, height = [int(x) for x in line.split()]

    # read voronoi output
    with open('test.out', 'r') as voronoi_output:
        pointLists = [eval(line) for line in voronoi_output.readlines()]

    colors = np.load('site_colors.npy')
    return pointLists[4:], colors, width, height


def draw_voronoi(polygons, colors, image_width, image_height):
    for polygon, color in zip(polygons, colors):
        plt.fill(*zip(*polygon), color=color)

    plt.xlim([0, image_width]), plt.ylim([0, image_height])
    plt.gca().invert_yaxis()
    plt.savefig('voronoi_output.png')
    plt.show()


if __name__ == "__main__":
    polygons, colors, width, height = read_voronoi_info()
    draw_voronoi(polygons, colors, width, height)
