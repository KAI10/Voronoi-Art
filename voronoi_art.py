import matplotlib.pyplot as plt
import numpy as np
import sys

# from skimage.viewer import ImageViewer
# from skimage.data import astronaut
from skimage.segmentation import slic
from skimage.util import img_as_float
from skimage.io import imread

from scipy.spatial import Voronoi, voronoi_plot_2d


def read_image(file_name):
    img = img_as_float(imread(file_name))  # height x width x channels
    # img = img_as_float(astronaut())
    img = np.swapaxes(img, 0, 1)  # width x height x channels
    # print(type(img), img.shape)
    return img


def get_representative_points(img):
    segments_slic = slic(img, n_segments=3500, compactness=10, sigma=1)
    print("SLIC number of segments: ", len(np.unique(segments_slic)))

    clusters = np.unique(segments_slic)
    cluster_centers, cluster_rgb_means = [], []

    # cluster_image = np.zeros(img.shape)

    for cluster in clusters:
        pixel_indices = np.where(segments_slic == cluster)
        pixel_rgbs = img[pixel_indices]
        pixel_coords = np.array(list(zip(pixel_indices[0], pixel_indices[1])))
        cluster_center = pixel_coords.mean(axis=0)
        cluster_rgb_mean = pixel_rgbs.mean(axis=0)

        # cluster_image[pixel_indices] = cluster_rgb_mean

        cluster_centers.append(cluster_center)
        cluster_rgb_means.append(cluster_rgb_mean)

    # viewer = ImageViewer(cluster_image)
    # viewer.show()
    return cluster_centers, cluster_rgb_means


def draw_voronoi_diagram(points, colors):
    vor = Voronoi(points)
    voronoi_plot_2d(vor, show_points=False, show_vertices=False, line_width=0)

    # colorize
    for region_index, color in zip(vor.point_region, colors):
        region = vor.regions[region_index]
        if -1 not in region:
            polygon = [vor.vertices[i] for i in region]
            plt.fill(*zip(*polygon), color=color)

    plt.xlim([0, img.shape[0]]), plt.ylim([0, img.shape[1]])
    plt.gca().invert_yaxis()
    plt.axis('off')

    plt.savefig('voronoi_output.png', bbox_inches='tight', pad_inches=0)
    plt.show()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print('Usage: python voronoi_art.py <image_file_name>')
        exit(0)

    img = read_image(sys.argv[1])
    points, colors = get_representative_points(img)
    draw_voronoi_diagram(points, colors)
