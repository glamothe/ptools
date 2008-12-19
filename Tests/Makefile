unittests: testcpp testpython

testpython:
	echo "running python tests"
	python unittest1.py

testcpp:
	echo "running C++ tests"
	python cxxtestgen.py --error-printer ptoolstest.h > runner.cpp
	g++ runner.cpp -I.. -I. -L.. -lptools -o ptoolstest.bin
	./ptoolstest.bin
	python cxxtestgen.py --error-printer testsuperpose.h   > runnertestsuper.cpp
	g++ runnertestsuper.cpp -I.. -I. -L.. -lptools -o testsuper.bin
	./testsuper.bin
