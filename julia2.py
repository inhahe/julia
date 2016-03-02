#Richard Albert Nichols III (http://www.gate.net/~inhahe)

import psyco
psyco.full()

from math import *
import string
res = 200
iters = 100
sf = 4.0/res
angle = 3.14159 #radians
xorigin, yorigin = 0, 0
outfile = open("angle90.pov","w")

outfile.write("""
camera {
  location <0,0,-5>
  look_at <0,0,0>  
  }
light_source
  {
  <0, 0, -10>
  color rgb 1
  parallel
  point_at 0
  }

blob
  {
""")


for z in range(res):

  print z


  cr = sin(angle)*(z-res/2.0)*sf
  ci = cos(angle)*(z-res/2.0)*sf

  for y in range(res):
    for x in range(res):
      zr = x*sf-2
      zi = y*sf-2
      for i in range(iters):
        zr, zi = zr*zr-zi*zi+cr, 2*zr*zi+ci
        if hypot(zr, zi) >=2:
          break
      else:
        outfile.write("\n  sphere {<%f, %f, %f>, %f, 1}" \
        % (x*sf-2, y*sf-2, z*sf-2, sf/2*3))
outfile.write("\n  pigment {rgb <1,0,0>}\n}")
