scalac Rdma.scala
SCALA_LIB_HOME="/usr/local/share/scala-2.11.8/lib/"
SCALA_CP=$SCALA_LIB_HOME/scala-library.jar:$SCALA_LIB_HOME/scala-reflect.jar
javah -cp $SCALA_CP:. Rdma
