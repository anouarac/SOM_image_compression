from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np
import subprocess
import math
import random
from PIL import Image, ImageChops
import os
path_to_exe = r"C:\Users" # path to jpeg encoder exe
diffs = []
sizes = []

bszz = []
outszz = []
mx = 29
save = [[] for i in range(mx+3)]
orgsize = os.stat(r"C:\Users").st_size # original image size

def f(x,y):
    return (np.sin(int(x))+np.cos(int(y)))
path_to_exe = r"C:\Users\Kohonen.exe" # specify path to exe
for bsz in range(2,mx+2):
  for outsz in range(2,mx+2):
    print(bsz, outsz)
    bszz.append(bsz*bsz)
    outszz.append(outsz)
    #subprocess.run([path_to_exe], input=f"0\n{bsz}\n{outsz}\n1\n1\n0\nn\n", encoding="ascii") # Produces image.jpeg -> runs "comp.exe i"
    #print("generated")
    size = (2000*1976/(bsz*bsz)*5/8+4) # 5/8 byte per int + height and width
    pngname = f"output_approx_{bsz}_{outsz}.png"
    new = Image.open(pngname).convert("L")
    orig = Image.open(r"C:\Users").convert("L") # specify path to original image
    difference = ImageChops.difference(orig, new)
    # difference.getcolors() # [(cnt, (r, g, b)) .. ]
    sum = 0
    for (cnt, l) in difference.getcolors():
      sum += cnt*l*l
    save[bsz].append(np.sqrt(sum)/2000/1976/255)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
X,Y = np.arange(2,mx+2), np.arange(2,mx+2)
X, Y = np.meshgrid(X, Y)
Z = np.zeros((mx,mx),dtype='d')
for i in range(mx):
  for j in range(mx):
    Z[i,j] = save[i+2][j]
#X = X.flatten()
#Y = Y.flatten()
#Z = Z.flatten()
#ax.scatter(X,Y,Z)
#surf = ax.plot_trisurf(X, Y, Z, cmap=cm.coolwarm, linewidth=0, antialiased=False)
#surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm, linewidth=0, antialiased=True)
#fig = plt.figure()
#ax = fig.gca(projection='3d')

#ax.plot_trisurf(X, Y, Z, linewidth=0.2, antialiased=True)
fig = plt.figure(figsize=(7,7))
ax = Axes3D(fig)
ax.set_xlabel('Output layer size')
ax.set_ylabel('Input layer size')
ax.set_zlabel('Error rate')
ax.plot_surface(X, Y, Z, cmap='viridis')
ax.view_init(30, 45)
plt.savefig('plot_3d_approx_viridis_line.png')
plt.show()
