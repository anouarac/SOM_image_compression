import subprocess
import matplotlib.pyplot as plt
from PIL import Image, ImageChops
import numpy as np
import os
path_to_exe = r"C:\Users" # path to jpeg encoder exe
diffs = []
sizes = []
orgsize = os.stat(r"C:\Users").st_size # size of the original image
for quality in range(1, 101) :
  print(quality)
  subprocess.run([path_to_exe], input=f"{quality}\n0", encoding="ascii") # Produces image.jpeg -> runs "comp.exe i"
  size = os.stat(r"C:\Users\Froopie\Desktop\Testing TIPE\image.jpg").st_size
  sizes.append(size/orgsize)
  jpeg = Image.open(r"C:\Users") # path to new image
  png = Image.open(r"C:\Users") # path to original image
  difference = ImageChops.difference(jpeg, png)
  # difference.getcolors() # [(cnt, (r, g, b)) .. ]
  sum = 0
  for (cnt, l) in difference.getcolors():
    sum += cnt*l*l
  diffs.append(np.sqrt(sum)/2000/1976/255*1000000)

approx = False

path_to_exe = r"C:\Users" # path to kohonen exe
mini = 1e12
idsz,idoutsz=0,0
diffs2 = []
sizes2 = []
for bsz in range(2,31):
  parc = list(range(2,31))
  if bsz > 5 and (bsz <= 10 ):
    parc.append(min(100,max(30,bsz*bsz)))
  size = (2000*1976/(bsz*bsz)*5/8+4) # 5/8 byte per int + height and width (p^2 < 32)
  locmin = 1e12
  for outsz in parc:
    print(bsz, outsz)
    #subprocess.run([path_to_exe], input=f"0\n{bsz}\n{outsz}\n1\n1\n0\ny\n", encoding="ascii") # Produces image.jpeg -> runs "comp.exe i"
    print("generated")
    pngname = f"output_{bsz}_{outsz}.png" # name of the new file
    if approx:
      pngname = f"output_approx_{bsz}_{outsz}.png"
    new = Image.open(pngname).convert("L")
    orig = Image.open(r"C:\User").convert("L") #path to the original file
    difference = ImageChops.difference(jpeg, png)
    # difference.getcolors() # [(cnt, (r, g, b)) .. ]
    sum = 0
    for (cnt, l) in difference.getcolors():
      sum += cnt*l*l
    if sum < mini:
      mini = sum
      idsz = bsz
      idoutsz = outsz
    locmin = min(locmin,sum)
    if sum < mini:
      mini = sum
      idsz = bsz
      idoutsz = outsz
  diffs2.append(np.sqrt(locmin)/2000/1976/255*1000000)
  sizes2.append(size/orgsize)

print(diffs)
print(mini,idsz,idoutsz)
plt.plot(sizes2,diffs2,color='red',label="SOM")
plt.plot(sizes,diffs,label="JPEG")
plt.legend()
plt.grid()

#plt.subplot(1,2,2).legend()
#plt.subplot(1,2,1).legend()
plt.ylabel("Error rate")
plt.xlabel("File size reduction ratio")
#plt.subplot(1,2,2).set_xlabel("File size")
plt.savefig('plot_target_square_grid.png')
plt.show()