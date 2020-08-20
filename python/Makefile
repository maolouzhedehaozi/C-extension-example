SOURCE=log.cc operator.cc non_stream_operator.cc
all:clean pymodule

app:
	g++ -g -Wall -shared -fPIC -std=c++1z -o librtransfer.so ${SOURCE}

pymodule:
	python3 setup.py build_ext --inplace
clean:
#	rm *.so
