#ifndef __STREAM_OPERATOR_H_
#define __STREAM_OPERATOR_H_

#include "operator.h"

class StreamNonBlockOperator : public BaseOperator {
 public:
  StreamNonBlockOperator(bool is_send);
  ~StreamNonBlockOperator();

  // TODO: (zxy) does stream need to handle bulk send?
  int PrepareStreamSend(void *ptr, size_t total_len, size_t send_len_each,
                        MemoryType mem_type, void *reg = nullptr);

  int PrepareStreamRecv(void *ptr, size_t total_len, size_t recv_len_each,
                        MemoryType mem_type, void *reg = nullptr);

  virtual int RunSend(SendDoneCallback done) override;
  virtual int RunRecv(RecvDoneCallback done) override;

 private:
  std::vector<void *> stream_send_ptr_;
  std::vector<size_t> stream_send_size_;
};

class StreamBlockOperator : public BaseOperator {
 public:
  StreamBlockOperator(bool is_send);
  ~StreamBlockOperator();

  // TODO: (zxy) does stream need to handle bulk send?
  int PrepareStreamSend(void *ptr, size_t total_len, size_t send_len_each,
                        MemoryType mem_type, void *reg = nullptr);

  int PrepareStreamRecv(void *ptr, size_t recv_len, MemoryType mem_type,
                        void *reg = nullptr);

  virtual int RunRecv(RecvDoneCallback done) override;
  virtual int RunSend(SendDoneCallback done) override;
};

#endif
