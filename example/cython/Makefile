
all:
	python setup.py build_ext --inplace

install:
	python setup.py install --user

test: all
	ipython test.py

clean:
	@rm -f *.o *.so *~ core
	@rm -rf build
