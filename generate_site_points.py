import numpy as np
import sys

# from skimage.viewer import ImageViewer
# from skimage.data import astronaut
from skimage.segmentation import slic
from skimage.util import img_as_float
from skimage.io import imread


def read_image(file_name):
    img = img_as_float(imread(file_name))  # height x width x channels
    img = np.swapaxes(img, 0, 1)  # width x height x channels
    # print(type(img), img.shape)
    return img


def get_representative_points(img):
    segments_slic = slic(img, n_segments=3500, compactness=10, sigma=1)
    # print("SLIC number of segments: ", len(np.unique(segments_slic)))

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


def save_site_points_colors(img, points, colors):
    with open('site_points.txt', 'w') as point_file:
        print(len(points), img.shape[0], img.shape[1], file=point_file)
        for point in points:
            print(*list(point), file=point_file)

    np.save('site_colors', colors)


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print('Usage: python voronoi_art.py <image file name> <# of site points>')
        exit(0)

    img = read_image(sys.argv[1])
    points, colors = get_representative_points(img, int(sys.argv[2]))
    save_site_points_colors(img, points, colors)
