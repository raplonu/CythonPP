import os


from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

os.environ["CC"] = "g++"
os.environ["CXX"] = "g++"

# Here is how to use the library built above. 
ext_modules = cythonize([
    Extension("*",
              language='c++',
              sources=["*.pyx"],
              include_dirs=[
                  os.getcwd(),
                  os.getcwd()+'../include'
              ],# path to .h file(s)
              library_dirs=[
              ],  # path to .a or .so file(s)
              libraries=[
              ],
              extra_compile_args=['-std=c++0x', '-Wunknown-pragmas']
          )
])

setup(
    name='cythonpp',
    ext_modules=ext_modules,
)

