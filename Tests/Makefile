unittests: testcpp testpython

testpython:
	echo "running python tests"
	python unittest1.py
	cd heligeom && python heligeom_test.py

testcpp:
	echo "running C++ tests"
	python cxxtestgen.py --error-printer ptoolstest.h > runner.cpp
	g++ -O2 runner.cpp -I../headers -I. -L.. -lptools -o ptoolstest.bin
	./ptoolstest.bin

