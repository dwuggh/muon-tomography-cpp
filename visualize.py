import numpy as np
import sys
import os
from scipy import stats
from mayavi import mlab



def read_density(filename, voxel_size, grid):
    density = np.loadtxt(filename)
    print(density.shape)
    grain = grid / voxel_size
    
    figure = mlab.figure('DensityPlot')
    xs = []
    ys = []
    zs = []
    grain = [25, 25, 11]
    # ds = []
    # max = 0
    for x in range(grain[0]):
        for y in range(grain[1]):
            for z in range(grain[2]):
                xs.append(x)
                ys.append(y)
                zs.append(z)

    pts = mlab.points3d(xs, ys, zs, density, scale_mode='none', scale_factor=0.5, mode='sphere', transparent=True)
    mlab.axes()
    mlab.show()


if __name__ == '__main__':
    argc = len(sys.argv)
    filename = sys.argv[1] if argc > 1 else 'result.txt'
    voxel_size = np.array([2, 2, 2])
    grid = np.array([50, 50, 22])
    read_density(filename, voxel_size, grid)
