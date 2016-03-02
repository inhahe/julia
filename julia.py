import psyco
psyco.full()

from math import *
import string
res = 10
iters = 10
sf = 4.0/res
sets = [[],[]]
angle = 3.14159
xorigin, yorigin = 0, 0
outfile = open("angle90.pov","w")
points = [[],[]]

class point:
  connection=None
  pass

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

""")

outfile.write("mesh {\n")

for z in range(res):
  cr = sin(angle)*(z-res/2.0)*sf
  ci = cos(angle)*(z-res/2.0)*sf

  print cr,ci

  side = z%2
  sets[side] = [[False]*res]*res

  for y in range(res):
    print z,"solve",y
    for x in range(res):
      zr = x*sf-2
      zi = y*sf-2
      for i in range(iters):
        zr, zi = zr*zr-zi*zi+cr, 2*zr*zi+ci
        if hypot(zr, zi) >=2:
          break
      else:
        sets[side][y][x] = True

  points[side] = []
  for y in range(res):
    print z, "outline",y
    for x in range(res):
      if sets[side][x][y]: 
        yy=y-(y>0)
        go=True
        while yy<res and go:
          for xx in range(x-(x>0), x+(x<res-1)+1):
            if not sets[side][xx][yy]:
              points[side].append(point())
              points[side][-1].x=x
              points[side][-1].y=y

              go=False
              break
          yy=yy+1

  for x1, p1 in enumerate(points[side]):
    print z,"connect",x1,"/",len(points[side])
    for p2 in points[side][x1+1:]:
      if p1.x==p2.x and p1.y==p2.y: print "collision",
      if p2.connection != p1:
        if abs(p2.x-p1.x)<=1 and abs(p2.y-p1.y)<=1:
          p1.connection = p2

  if z>0:
    for side2 in [0,1]:
      for n, p in enumerate(points[side2]):
        print z, "triangle", side2, n, "/", len(points[side2]),
        if p.connection:
          print ", connection",
          pcx = p.connection.x
          pcy = p.connection.y
          mpx = (p.x+pcx)/2.0
          mpy = (p.y+pcy)/2.0
          mind = None
          for p2 in points[not side2]:
            d = hypot(mpx-p2.x, mpy-p2.y)
            if not mind or d<mind:
              mind = d
              x2 = p2.x
              y2 = p2.y
          if mind:
            print ", opposing point",
            outfile.write("  triangle {<" + str(p.x*sf-2)  \
            + ", " + str(p.y*sf-2) + ", " + str((z-(side2+side)%2)*sf-2) \
            + "><" + str(pcx*sf-2) + ", " + str(pcy*sf-2)  \
            + ", " + str((z-(side2+side)%2)*sf-2) + "><" + str(x2*sf-2) \
            + ", " + str(y2*sf-2) + ", " +  str((z-(side2+side+1)%2)*sf-2) \
            + ">}\n")
        print

outfile.write("texture { pigment {color rgb <1,0,0>} }\n")
outfile.write("}")

