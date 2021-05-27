import subprocess
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image, ImageChops
import os

path_to_exe = r"C:\Users" # exe path
mini = 1e12
lbsz = range(2,31) # n range
for bsz in lbsz:
  size = (2000*1976/(bsz*bsz)*5/8+4)/1000 # 5/8 byte per int + height and width
  locmin = 1e12
  l = range(2,31) # p range
  for outsz in l:
    print(bsz, outsz)
    subprocess.run([path_to_exe], input=f"0\n{bsz}\n{outsz}\n1\n1\n0\ny\n", encoding="ascii") # Produces image.jpeg -> runs "comp.exe i"
    print("generated")
