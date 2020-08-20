SOURCE=log.cc operator.cc non_stream_operator.cc rdma.cc
FLAGS= -I ./ -Wall -I ${JAVA_HOME}/include \
       -I ${JAVA_HOME}/include/linux

all:lib

lib:
	g++ -g -Wall -shared -fPIC ${FLAGS} ${SOURCE} -o librdma.so -std=c++1z

clean:
	rm *.so
