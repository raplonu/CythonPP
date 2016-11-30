# -*- coding: utf-8 -*-
"""
Created on Wed Apr 20 14:10:19 2016

@author: jbernard
"""

import numpy as np
cimport numpy as np
from libcpp.string cimport string

cdef extern from "../include/CythonUtils.h":
    cdef cppclass Interface[T]:
        @staticmethod
        T* castVariant(np.uint64_t ptr)
        @staticmethod
        T get(np.uint64_t ptr)

    np.uint64_t makePtr[T](T val)
    np.uint64_t castPtr[T](T* ptr)
    void deletePtr(np.uint64_t ptr)

cdef extern from "../include/TaskManager.h":
    cdef cppclass GObject:
        GObject()
        np.uint64_t call(string methodName, np.uint64_t* args)

        string names()
        string signature(string methodName)
        string returnValue(string methodName)

    cdef cppclass TaskManager:
        TaskManager()
        np.uint64_t makeObject(string classBame, string constructorName, np.uint64_t* args)
        np.uint64_t call(string funName, np.uint64_t* args)

        string classNames()
        string constructorNames(string className)
        string classNameById(string signature)

        string classSignature(string className, string constructorName)
        string classReturnValue(string className)

        string names()
        string signature(string name)
        string returnValue(string name)

cdef class Object:
    cdef GObject *go

cdef class GTaskMan:
    cdef TaskManager *tm
