import os
import sys

from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

os.environ["CC"] = "g++"
os.environ["CXX"] = "g++"

cythonpp = os.environ['CYTHONPP_ROOT']

sys.path.append(cythonpp +'/cython')

# Here is how to use the library built above.
ext_modules = cythonize([
    Extension("*",
              language='c++',
              sources=["*.pyx"],
              include_dirs=[
                  os.getcwd(),
                  os.getcwd()+'../inc',
                  cythonpp + '/cython',
                  cythonpp + '/include'
              ],# path to .h file(s)
              library_dirs=[
                  '../lib'
              ],  # path to .a or .so file(s)
              libraries=[

              ],
              extra_compile_args=['-std=c++0x', '-Wunknown-pragmas']
          )
])

setup(
    name='TaskManager',
    ext_modules=ext_modules,
)
