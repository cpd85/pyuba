import pyuba

my_puba = pyuba.new_pyuba(8)

pyuba.insert(my_puba, 0, 8)

assert(8 == pyuba.getitem(my_puba, 0))

pyuba.remove(my_puba, 0)

print "passed"
