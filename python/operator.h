#ifndef __BASE_OPERATOR_H_
#define __BASE_OPERATOR_H_

#include <cstdlib>
#include <functional>
#include <iostream>
#include <vector>

#include "interface.h"
#include "log.h"

/*
 * Base class define common interface, and the derived class implement
 * the interface to enable non-stream non-block, non-stream block, stream
 * non-block, stream block interact, adding more process logical if need.
 */
class BaseOperator {
 public:
  // The operator provide send and recv interface,
  // so the derived class maybe perform send or recv.
  // The 'is_send' indicate this class instance will
  // perform send operation or recv operation.
  BaseOperator(bool is_send);
  ~BaseOperator();

  // Communicate layer's id, use it to run send or recv.
  void SetLocalTransferId(rtransfer_id *id) { local_id_ = id; }

  // Run actual send operation.
  typedef std::function<void(void *, size_t)> SendDoneCallback;
  virtual int RunSend(SendDoneCallback done) {
    std::cout << "enter into RunSend\n";
    done(reinterpret_cast<void*>(0x10), 10);
    std::cout << "RunSend pycallback done\n";
    return 0;
  }

  // Run actual recv operation.
  typedef std::function<void(void *, size_t)> RecvDoneCallback;
  virtual int RunRecv(RecvDoneCallback done) { 
    done(reinterpret_cast<void*>(0x10), 10);
    return 0; 
  }

  enum MemoryType { HOST_MEMORY, GPU_MEMORY, FPGA_MEMORY };
  // Fill send buffer, buffer size, buffer location,
  // register result to class instance, this is import
  // for send. The register result is optional, if not
  // provide, the implement of this interface will run
  // a lookup.
  virtual int PrepareSend(void *ptr, size_t len, MemoryType mem_type,
                          void *reg = nullptr);

  // Fill recv buffer, buffer size, buffer location,
  // register result to class instance, this is import
  // for send. The register result is optional, if not
  // provide, the implement of this interface will run
  // a lookup.
  virtual int PrepareRecv(void *ptr, size_t len, MemoryType mem_type,
                          void *reg = nullptr);

  // Set up bulk send information.
  virtual int PrepareBulkSend(std::vector<void *> &bulk_ptr,
                              std::vector<size_t> &bulk_len);

  virtual int PrepareBulkSend(std::vector<void *> &bulk_ptr,
                              std::vector<size_t> &bulk_len,
                              std::vector<void *> &bulk_reg);

 private:
  bool is_send_;
  rtransfer_id *local_id_;

  // Bulk send: send content in more
  // than one piece of memory chunks.
  size_t n_bulk_;
  bool bulk_send_;

  void **bulk_send_ptr_;
  void **bulk_send_reg_;
  size_t *bulk_send_size_;

  // Single send: send content
  // in contig memory chunk.
  void *send_ptr_;
  void *send_reg_;
  size_t send_size_;

  // Recv:
  // no matter bulk send or not, send content
  // will save into a contig memory chunk.
  void *recv_ptr_;
  void *recv_reg_;
  size_t recv_size_;
};

#endif
