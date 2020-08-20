import rdma

def callback(i):
    print(i)


if __name__=="__main__":
    data = b'hello'
    size = len(data)
    rdma.send_binary_nonblock(59, data, size, 1, callback)
