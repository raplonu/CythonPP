# -*- coding: utf-8 -*-
"""
Created on Wed Apr 20 14:10:19 2016

@author: jbernard
"""

import numpy as np
cimport numpy as np
from libcpp.string cimport string
from libcpp cimport bool as bool_t

SEPARATOR = ','
SUPERSEPARATOR = ';'

cdef extern from "../inc/CythonUtils.h":
    cdef cppclass Interface[T]:
        @staticmethod
        T* castVariant(np.uint64_t ptr)
        @staticmethod
        T get(np.uint64_t ptr)

    np.uint64_t makePtr[T](T val)
    np.uint64_t castPtr[T](T* ptr)
    void deletePtr(np.uint64_t ptr)

cdef extern from "../inc/TaskManager.h":
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



def nop(*args):
    pass

def set_float32(val):
    return val, makePtr[np.float32_t](val)

def set_float64(val):
    return val, makePtr[np.float64_t](val)

def set_int8(val):
    return val, makePtr[np.int8_t](val)

def set_int16(val):
    return val, makePtr[np.int16_t](val)

def set_int32(val):
    return val, makePtr[np.int32_t](val)

def set_int64(val):
    return val, makePtr[np.int64_t](val)

def set_uint8(val):
    return val, makePtr[np.uint8_t](val)

def set_uint16(val):
    return val, makePtr[np.uint16_t](val)

def set_uint32(val):
    return val, makePtr[np.uint32_t](val)

def set_uint64(val):
    return val, makePtr[np.uint64_t](val)

def set_string(val):
    return val, makePtr[string](val)

def set_bool(val):
    return val, makePtr[bool_t](val)



def get_float32(np.uint64_t ptr):
    return Interface[np.float32_t].get(ptr)

def get_float64(np.uint64_t ptr):
    return Interface[np.float32_t].get(ptr)

def get_int8(np.uint64_t ptr):
    return Interface[np.int8_t].get(ptr)

def get_int16(np.uint64_t ptr):
    return Interface[np.int16_t].get(ptr)

def get_int32(np.uint64_t ptr):
    return Interface[np.int32_t].get(ptr)

def get_int64(np.uint64_t ptr):
    return Interface[np.int64_t].get(ptr)

def get_uint8(np.uint64_t ptr):
    return Interface[np.uint8_t].get(ptr)

def get_uint16(np.uint64_t ptr):
    return Interface[np.uint16_t].get(ptr)

def get_uint32(np.uint64_t ptr):
    return Interface[np.uint32_t].get(ptr)

def get_uint64(np.uint64_t ptr):
    return Interface[np.uint64_t].get(ptr)

def get_string(np.uint64_t ptr):
    return Interface[string].get(ptr)

def get_bool(np.uint64_t ptr):
    return Interface[bool_t].get(ptr)

def noUpdate(dst, src, ptr):
    deletePtr(ptr)



def setArray_float32(array):
    cdef np.ndarray[np.float32_t] buf = array.flatten()
    return (buf, makePtr(<np.float32_t*>buf.data))

def setArray_float64(array):
    cdef np.ndarray[np.float64_t] buf = np.array(array.flatten())
    return (buf, makePtr(<np.float64_t*>buf.data))

def setArray_int8(array):
    cdef np.ndarray[np.int8_t] buf = np.array(array.flatten())
    return (buf, makePtr(<np.int8_t*>buf.data))

def setArray_int16(array):
    cdef np.ndarray[np.int16_t] buf = np.array(array.flatten())
    return (buf, makePtr(<np.int16_t*>buf.data))

def setArray_int32(array):
    cdef np.ndarray[np.int32_t] buf = np.array(array.flatten())
    return (buf, makePtr(<np.int32_t*>buf.data))

def setArray_int64(array):
    cdef np.ndarray[np.int64_t] buf = np.array(array.flatten())
    return (buf, makePtr(<np.int64_t*>buf.data))

def setArray_uint8(array):
    cdef np.ndarray[np.uint8_t] buf = np.array(array.flatten())
    return (buf, makePtr(<np.uint8_t*>buf.data))

def setArray_uint16(array):
    cdef np.ndarray[np.uint16_t] buf = np.array(array.flatten())
    return (buf, makePtr(<np.uint16_t*>buf.data))

def setArray_uint32(array):
    cdef np.ndarray[np.uint32_t] buf = np.array(array.flatten())
    return (buf, makePtr(<np.uint32_t*>buf.data))

def setArray_uint64(array):
    cdef np.ndarray[np.uint64_t] buf = np.array(array.flatten())
    return (buf, makePtr(<np.uint64_t*>buf.data))

def noGetArray(np.uint64_t ptr):
    pass

def updateReq(dst, src, offset):
    if len(dst.shape) == 1:

        dst[:] = src[offset:offset + dst.shape[0]]
    else:
        for i in xrange(dst.shape[0]):
            updateReq(dst[i], src, i * dst.shape[1] + offset)

def updateArray(dst, src, ptr):
    deletePtr(ptr)
    updateReq(dst, src, 0)


#Simple type declaration
typeDictionnary = [
    {'name' : 'Float',    'set' : set_float32,    'get' : get_float32,    'update' : noUpdate, 'mangled' : ['f'],     'ptype' : [np.float32]},
    {'name' : 'Double',   'set' : set_float64,    'get' : get_float64,    'update' : noUpdate, 'mangled' : ['d'],     'ptype' : [np.float64]},

    {'name' : 'Char',     'set' : set_int8,       'get' : get_int8,       'update' : noUpdate, 'mangled' : ['c'],     'ptype' : [np.int8]},
    {'name' : 'Short',    'set' : set_int16,      'get' : get_int16,      'update' : noUpdate, 'mangled' : ['s'],     'ptype' : [np.short, np.int16]},
    {'name' : 'Integer',  'set' : set_int32,      'get' : get_int32,      'update' : noUpdate, 'mangled' : ['i'],     'ptype' : [int, np.int, np.int32]},
    {'name' : 'Long',     'set' : set_int64,      'get' : get_int64,      'update' : noUpdate, 'mangled' : ['l'],     'ptype' : [long, np.long, np.int64]},

    {'name' : 'U Char',   'set' : set_uint8,      'get' : get_uint8,      'update' : noUpdate, 'mangled' : ['h'],     'ptype' : [np.uint8]},
    {'name' : 'U Short',  'set' : set_uint16,     'get' : get_uint16,     'update' : noUpdate, 'mangled' : ['t'],     'ptype' : [np.uint16]},
    {'name' : 'U Int',    'set' : set_uint32,     'get' : get_uint32,     'update' : noUpdate, 'mangled' : ['j'],     'ptype' : [np.uint32]},
    {'name' : 'U Long',   'set' : set_uint64,     'get' : get_uint64,     'update' : noUpdate, 'mangled' : ['m'],     'ptype' : [np.uint64]},

    {'name' : '*Float',    'set' : setArray_float32,    'get' : noGetArray,   'update' : updateArray, 'mangled' : ['Pf'],     'ptype' : [np.ndarray], 'dtype' : [np.float32]},
    {'name' : '*Double',   'set' : setArray_float64,    'get' : noGetArray,   'update' : updateArray, 'mangled' : ['Pd'],     'ptype' : [np.ndarray], 'dtype' : [np.float64, np.float]},

    {'name' : '*Char',     'set' : setArray_int8,       'get' : noGetArray,   'update' : updateArray, 'mangled' : ['Pc'],     'ptype' : [np.ndarray], 'dtype' : [np.int8]},
    {'name' : '*Short',    'set' : setArray_int16,      'get' : noGetArray,   'update' : updateArray, 'mangled' : ['Ps'],     'ptype' : [np.ndarray], 'dtype' : [np.int16, np.short]},
    {'name' : '*Integer',  'set' : setArray_int32,      'get' : noGetArray,   'update' : updateArray, 'mangled' : ['Pi'],     'ptype' : [np.ndarray], 'dtype' : [np.int32]},
    {'name' : '*Long',     'set' : setArray_int64,      'get' : noGetArray,   'update' : updateArray, 'mangled' : ['Pl','Px'],'ptype' : [np.ndarray], 'dtype' : [np.int64, np.long]},

    {'name' : '*U Char',   'set' : setArray_uint8,      'get' : noGetArray,   'update' : updateArray, 'mangled' : ['Ph'],     'ptype' : [np.ndarray], 'dtype' : [np.uint8]},
    {'name' : '*U Short',  'set' : setArray_uint16,     'get' : noGetArray,   'update' : updateArray, 'mangled' : ['Pt'],     'ptype' : [np.ndarray], 'dtype' : [np.uint16, np.ushort]},
    {'name' : '*U Int',    'set' : setArray_uint32,     'get' : noGetArray,   'update' : updateArray, 'mangled' : ['Pj'],     'ptype' : [np.ndarray], 'dtype' : [np.uint32]},
    {'name' : '*U Long',   'set' : setArray_uint64,     'get' : noGetArray,   'update' : updateArray, 'mangled' : ['Pm'],     'ptype' : [np.ndarray], 'dtype' : [np.uint64]},

    {'name' : 'String',   'set' : set_string,     'get' : get_string,     'update' : noUpdate, 'mangled' : ['Ss'],    'ptype' : [str, bytes]},
    {'name' : 'Boolean',  'set' : set_bool,       'get' : get_bool,       'update' : noUpdate, 'mangled' : ['b'],     'ptype' : [bool]},
    {'name' : 'Void',     'set' : nop,            'get' : nop,            'update' : noUpdate, 'mangled' : ['v'],    'ptype' : [None]}

]

def translatePType(arg, string typeDst):
    if(type(arg) is np.ndarray):
        for d in typeDictionnary:
            if np.ndarray in d['ptype']:
                if arg.dtype in d['dtype']:
                    return d[typeDst]
    else:
        for d in typeDictionnary:
            if type(arg) in d['ptype']:
                return d[typeDst]
    return None


def translate(arg, string typeSrc, string typeDst):
    for d in typeDictionnary:
        if arg in d[typeSrc]:
            return d[typeDst]
    return None

def translateList(args, string typeSrc, string typeDst):
    cdef string ptype = 'ptype'
    if(typeSrc == ptype):
        return [translatePType(arg, typeDst) for arg in args]
    else:
        return [translate(arg, typeSrc, typeDst) for arg in args]

def typeToStr(t):
    return str(t).split('\'')[1]

def typesToStr(types):
    res = ''
    for t in types:
        res += typeToStr(t)+' '
    return res


def getTypeOf(sig):
    for d in typeDictionnary:
        if sig in d['mangled']:
            return d
    raise NotImplementedError('Unknow type '+str(sig)+' not implemented')

def testArgs(args, sig):
    l = len(args)
    if l == 0 and len(sig) == 1 and sig[0] == '':
        return
    if l != len(sig):
        raise TypeError('Call takes exactly '+str(len(sig))+' argument(s) ('+str(l)+' given)')
    for i in xrange(l):
        info = getTypeOf(sig[i])
        if type(args[i]) not in info['ptype']:
            raise TypeError('Invalid type for the element '+str(i)+' : arg is '+typeToStr(type(args[i]))+', required types : '+typesToStr(info['ptype']))

def setArgs(args, sig):
    l = len(args)
    cdef np.ndarray[np.uint64_t] buf = np.empty(l, dtype=np.uint64)
    anchor = ['']*l
    for i in xrange(l):
        anchor[i], buf[i] = getTypeOf(sig[i])['set'](args[i])
    return anchor, buf

def updateArgs(args, sig, anchor, np.ndarray[np.uint64_t] buf):
    for i in xrange(len(args)):
        getTypeOf(sig[i])['update'](args[i], anchor[i], buf[i])

def callFun(args, sig, ret, call):
    testArgs(args, sig)
    cdef np.ndarray[np.uint64_t] buf
    anchor, buf = setArgs(args, sig)
    cdef np.uint64_t retPtr = call(buf)
    updateArgs(args, sig, anchor, buf)
    return getTypeOf(ret)['get'](retPtr)


cdef class Object:

    def __cinit__(self, ptr):
        self.go = <GObject*>( Interface[GObject].castVariant(ptr) )

    def __dealloc__(self):
        del self.go

    def __call(self, string name, np.ndarray[np.uint64_t] args):
        return self.go.call(name, <np.uint64_t*>args.data)

    def call(self, name, *args):
        if name not in self.go.names().split(SUPERSEPARATOR):
            raise TypeError('Invalid method name')
        sig = self.go.signature(name).split(',')
        ret = self.go.returnValue(name)
        call = lambda buf : self.__call(name, buf)
        return callFun(args, sig, ret, call)

    def names(self):
        return self.go.names().split(SUPERSEPARATOR)

    def signature(self, string name):
        return translateList(self.go.signature(name).split(SEPARATOR), 'mangled', 'name')


cdef class GTaskMan:

    def __init__(self):
        pass

    def __getConstructorName(self, string className, args):
        consNames = self.tm.constructorNames(className).split(SUPERSEPARATOR)
        sig = translateList(args, 'ptype', 'mangled')
        for cons in consNames:
            c = cons.split(SEPARATOR)
            if len(cons) == 0 and len(args) == 0 :
                return cons
            if len(c) != len(args):
                continue
            if not all(s1 in s2 for s1, s2 in zip(c, sig)):
                continue
            return cons
        raise TypeError('Invalid constructor signature')

    def __make(self, string className, string constructorName, np.ndarray[np.uint64_t] args):
        return self.tm.makeObject(className, constructorName, <np.uint64_t*>args.data)

    def __call(self, string funName, np.ndarray[np.uint64_t] args):
        return self.tm.call(funName, <np.uint64_t*>args.data)

    def make(self, string className, *args):
        constructorName = self.__getConstructorName(className, args)
        sig = constructorName.split(SEPARATOR)
        make = lambda buf : self.__make(className, constructorName, buf)
        return callFun(args, sig, '7GObject', make)

    def call(self, funName, *args):
        if funName not in self.funNames():
            raise TypeError('Invalid method name')
        sig = self.tm.signature(funName).split(',')
        ret = self.tm.returnValue(funName)
        call = lambda buf : self.__call(funName, buf)
        return callFun(args, sig, ret, call)


    def classNames(self):
        return self.tm.classNames().split(SUPERSEPARATOR)

    def funNames(self):
        return self.tm.names().split(SUPERSEPARATOR)

    def classSignature(self, string className):
        return [translateList(signature.split(SEPARATOR), 'mangled', 'name') for signature in self.tm.constructorNames(className).split(SUPERSEPARATOR)]

    def funSignature(self, string name):
        return translateList(self.tm.signature(name).split(SEPARATOR), 'mangled', 'name')






def set_object(obj):
    pass #return obj, makePtr[GObject](<GObject*>(obj.go))

def updateObj(dst, src, ptr):
    pass

def get_object(np.uint64_t ptr):
    return Object( ptr )

typeDictionnary.append(
    {'name' : ['GObject'], 'set' : set_object,      'get' : get_object,     'update' : updateObj, 'mangled' : ['7GObject'], 'ptype' : [Object]})
