
all:
	(cd cython && python setup.py build_ext --inplace)

install:
	(cd cython && python setup.py install --user)

test: all
	(cd cython && ipython test.py)

clean:
	@(cd cython && rm -f *.cpp *.o *.so *~ core)
	@(cd cython && rm -rf build)
