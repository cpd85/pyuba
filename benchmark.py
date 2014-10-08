import pyuba
import time

#I'm guessing the reason pyuba is slow right now is because
#the calls are wrapped in a class.

mypyuba = pyuba.new_pyuba(500)
mylist = [0] * 8

regliststart = time.time()
for i in xrange(500):
  mylist.append(0)
reglistend = time.time()

pyubastart = time.time()
for i in xrange(500):
  pyuba.append(mypyuba,0)
pyubafin = time.time()

print "reglist =", (reglistend-regliststart)
print "pyuba =", (pyubafin - pyubastart)
