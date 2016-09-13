
from cythonpp cimport *

cdef extern from "../inc/MyTaskMan.h":        
    cdef cppclass MyTaskMan(TaskManager):
        MyTaskMan()

cdef class TaskMan(GTaskMan):
    def __init__(self):
        self.tm = new MyTaskMan()


tm = TaskMan()
