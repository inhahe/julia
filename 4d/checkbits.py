ifile = open('d:\\juliabits.bin','rb')
nulls = 0
while 1:
  a = ifile.read(10000)
  if len(a)<10000: break
  nulls += a.count('\0')
print nulls

