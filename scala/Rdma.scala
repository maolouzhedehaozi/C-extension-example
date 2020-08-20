class Rdma {
    init()
    private def init(): Unit = {
    System.loadLibrary("rdma")
    println("load finish")
  }
  
  def callback(i: Int): Int = {
        println("enter into callback")
        return i
  }

  @native def start_server(localIpStr:String, localPort:Int): Int
  @native def stop_server(): Unit
  @native def connect_to_server(sockId:Long): Int
  @native def send_binary_nonblock(sockId:Long, data:Array[Byte], size:Long, trans_mode:Int): Int
  
}

object Rdma {
    def main(args: Array[String]): Unit = {
      val rdma = new Rdma
      val content = new Array[Byte](5)
 
      for (i <- 0 to (content.length - 1)) {
          content(i) = 101
      }
 
      rdma.send_binary_nonblock(59, content, content.length, 0) 
    }
}


