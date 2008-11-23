unittests: testcpp testpython

testpython:
	echo "running python tests"
	python unittest1.py

testcpp:
	echo "running C++ tests"
	python cxxtestgen.py --error-printer ptoolstest.h > runner.cpp
	g++ runner.cpp -I.. -I. -L.. -lptools -o ptoolstest.bin
	./ptoolstest.bin