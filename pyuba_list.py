from pyuba import *

class pyuba_list(object):
  def __init__(self, size):
    self.pyuba = new_pyuba(size)
    return

  def __getitem__(self, num):
    return getitem(self.pyuba, num)

  def insert(self, index, item):
    return insert(self.pyuba, index, item)
