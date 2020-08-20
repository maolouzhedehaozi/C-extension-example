from distutils.core import setup, Extension

def main():
    module = Extension("rdma", 
                       libraries = [], 
                       library_dirs= ["/usr/lib64/", "/home/centos/zmn/webank/python"], 
                       extra_compile_args= ["-std=c++1z", "-lstdc++"],
                       sources = ["log.cc", "operator.cc", "non_stream_operator.cc", "pyrdma.cpp"])

    setup(name="rdma",
            version="1.0.0",
            description="RDMA python interface",
            author="zmn",
            author_email="mandy.zhang@clustar.ai",
            ext_modules=[module])

if __name__ == "__main__":
    main()
