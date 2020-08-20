#include <Python.h>
#include <iostream>

#include "operator.h"

static PyObject* RdmaError;


static PyObject* start_server(PyObject* self, PyObject* args) {

}

static PyObject* create_client(PyObject* self, PyObject* args) {

}

static PyObject* connect_to_server(PyObject* self, PyObject* args) {

}

static PyObject* mem_register(PyObject* self, PyObject* args) {

}

static PyObject* mem_unregister(PyObject* self, PyObject* args) {

}

/*
 * Rdma.send_binary_nonblock(sockid, data, size, trans_mode)
*/
static PyObject* send_binary_nonblock(PyObject* self, PyObject* args) {
	long sockid;
	char* data;
	int size;
	int trans_mode;
	bool is_send = true;
	PyObject* pycallback;
	int status;
	
	printf("before PyArg_ParseTuple\n");
        if (!PyArg_ParseTuple(args, "lsiiO", &sockid, &data, &size, 
            &trans_mode, &pycallback)) {
		PyErr_Print();
		PyErr_Clear();
		return Py_None;
	}	
	printf("PyArg_ParseTuple done\n");

	auto callbackFunc = [pycallback](void* ptr, size_t arg) {
		PyGILState_STATE gstate;
                gstate = PyGILState_Ensure();
                PyObject* arglist;
                arglist = Py_BuildValue("(i)", arg);
		PyObject* result = PyObject_CallObject(pycallback, arglist);
		Py_DECREF(arglist);
                PyGILState_Release(gstate);
	};

        Py_BEGIN_ALLOW_THREADS
	BaseOperator binaryOp(is_send);
//	binaryOp.SetLocalTransferId(sockid);
//	binaryOp.PrepareSend(data, size, trans_mode);
	printf("before RunSend\n");
	status = binaryOp.RunSend(callbackFunc);
        printf("RunSend done\n"); 
        if (!WIFEXITED(status)) {
 		return PyErr_Format(RdmaError, "Execute RunSend failed");
	}
        Py_END_ALLOW_THREADS

	return Py_BuildValue("i", WEXITSTATUS(status));
}


/*
 * Rdma.recv_binary_nonblock(sockid, data, size, trans_mode)
*/
static PyObject* recv_binary_nonblock(PyObject* self, PyObject* args) {
	long sockid;
	char* data;
	int size;
	int trans_mode;
	bool is_send = false;
	PyObject* pycallback;
	int status;
	
	
	if (!PyArg_ParseTuple(args, "lsiii|o", &sockid, &data, &size, 
            &trans_mode, &pycallback)) {
		PyErr_Print();
		PyErr_Clear();
		return Py_None;
	}	
	
	auto callbackFunc = [pycallback](void* ptr, size_t arg) {
		PyObject* arglist = Py_BuildValue("i", arg);
		PyObject* result = PyObject_CallObject(pycallback, arglist);
		Py_DECREF(arglist);
	        int ret;
                if (!PyArg_ParseTuple(result, "i", &ret)) {
		    PyErr_Print();
		    PyErr_Clear();
		  //  return Py_None;			
		}
		//return ret;
	};
	
	Py_BEGIN_ALLOW_THREADS
	
	BaseOperator binaryOp(is_send);
//	binaryOp.PrepareRecv(data, size, trans_mode);
	//need specify the transfer ID
	status = binaryOp.RunRecv(callbackFunc);
	if (!WIFEXITED(status)) {
 		return PyErr_Format(RdmaError, "Execute RunRecv failed");
	}

	Py_END_ALLOW_THREADS	
	
	return Py_BuildValue("i", WEXITSTATUS(size));

}

static PyObject* send_stream_nonblock(PyObject* self, PyObject* args) {

}

static PyObject* send_binary_block(PyObject* self, PyObject* args) {

}

static PyObject* send_stream_block(PyObject* self, PyObject* args) {

}

static PyObject* recv_stream_nonblock(PyObject* self, PyObject* args) {

}

static PyObject* recv_binary_block(PyObject* self, PyObject* args) {

}

static PyObject* recv_stream_block(PyObject* self, PyObject* args) {

}

static PyObject* send_multi_partitions(PyObject* self, PyObject* args) {

}

static PyObject* recv_multi_partitions(PyObject* self, PyObject* args) {

}

static PyMethodDef RdmaMethods[] = {
    {
        "start_server",
        start_server,
        METH_VARARGS,
        "Start the rdma server.\n\n"
        "If the startup service fails, return -1.",
    },
    {
	"create_client",
	create_client,
        METH_VARARGS,
	"Create a rdma client.\n\n"
	"Create a rdma client endpoint and return rdma transfer handle",
    },
    {
	"connect_to_server",
	connect_to_server,
        METH_VARARGS,
	"Connect to rdma server.\n\n"
	"Create a link to the server and return 0 on success.",
    },
    {
        "mem_register",
	mem_register,
        METH_VARARGS,
	"Register host memory to the RDMA network card.\n\n"
	"Register a Memory Region to allow remote read and write to " 
        "it, return 0 on success.",
    },
    {
	"mem_unregister",
	mem_unregister,
	METH_VARARGS,
	"Deregister host memory with the RDMA network card.\n\n"
	"Deregister a Memory Region, return 0 on success",
    },
    {
	"send_binary_nonblock",
	send_binary_nonblock,
        METH_VARARGS,
	"Send binary data in non-blocking mode.\n\n"
	"If the send fails returns a non-zero exit status.",
    },
    {
	"send_stream_nonblock",
	send_stream_nonblock,
	METH_VARARGS,
	"Send stream data in non-blocking mode.\n\n"
	"If the send fails returns a non-zero exit status.",
		
    },
    {
	"send_binary_block",
	send_binary_block,
        METH_VARARGS,
	"Send binary data in blocking mode.\n\n"
	"If the send fails returns a non-zero exit status.",
    },
    {
	"send_stream_block",
	send_stream_block,
	METH_VARARGS,
	"Send stream data in blocking mode.\n\n"
	"If the send fails returns a non-zero exit status.",
	
    },
    {
	"recv_binary_nonblock",
	recv_binary_nonblock,
	METH_VARARGS,
	"Recieve binary data in non-blocking mode.\n\n"
	"If the reception is successful, return data size.",
	
    },
    {
	"recv_stream_nonblock",
	recv_stream_nonblock,
	METH_VARARGS,
	"Recieve stream data in non-blocking mode.\n\n"
	"If the reception is successful, return total data size",
	
    },
    {
	"recv_binary_block",
	recv_binary_block,
	METH_VARARGS,
	"Recieve binary data in blocking mode.\n\n"
	"If the reception is successful, return data size.",
	
    },
    {
	"recv_stream_block",
	recv_stream_block,
	METH_VARARGS,
	"Recieve stream data in blocking mode.\n\n"
	"If the reception is successful, return total data size.",
    },
    {
	"send_multi_partitions",
	send_multi_partitions,
	METH_VARARGS,
	"Send non-contiguous memory that memory addresses are stored in a list.\n\n"
	"If the amount of memory exceeds the single transmission limit of the network "
        "card, it will become multiple sending like stream send."		
    },
    {
	"recv_multi_partitions",
	recv_multi_partitions,
	METH_VARARGS,
	"Recieve non-contiguous memory into a block contiguous memory.\n\n"
	"If the reception is successful, return data size.",
    },
    {NULL, NULL, 0, NULL},  // sentinel
};


static PyModuleDef rdmamodule = {
    PyModuleDef_HEAD_INIT,
    "rdma",
    "RDMA Python C++ extension module.",
    -1,
    RdmaMethods,
};

PyMODINIT_FUNC PyInit_rdma() {
    PyObject* module;

    module = PyModule_Create(&rdmamodule);
    if (module == NULL) {
        return NULL;
    }
    RdmaError = PyErr_NewException("rdma.Error", NULL, NULL);
    Py_INCREF(RdmaError);
    PyModule_AddObject(module, "Error", RdmaError);
    return module;
}
