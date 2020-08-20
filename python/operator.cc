#include <assert.h>

#include "operator.h"

BaseOperator::BaseOperator(bool is_send) {
  is_send_ = is_send;
  local_id_ = nullptr;

  send_size_ = 0;
  send_ptr_ = nullptr;
  send_reg_ = nullptr;

  recv_size_ = 0;
  recv_ptr_ = nullptr;
  recv_reg_ = nullptr;

  n_bulk_ = 0;
  bulk_send_ = false;

  bulk_send_ptr_ = nullptr;
  bulk_send_reg_ = nullptr;
  bulk_send_size_ = nullptr;
}

BaseOperator::~BaseOperator() {
  if (bulk_send_ptr_) free(bulk_send_ptr_);
  if (bulk_send_reg_) free(bulk_send_reg_);
  if (bulk_send_size_) free(bulk_send_size_);
}

int BaseOperator::PrepareSend(void *ptr, size_t len, MemoryType mem_type,
                              void *reg) {
  n_bulk_ = 0;
  bulk_send_ = false;

  // TODO: (zxy) add support for fpga memory.
  assert(mem_type != FPGA_MEMORY);

  send_ptr_ = ptr;
  send_size_ = len;

  if (reg == nullptr) {
    // TODO: (zxy) run rcache lookup.
  } else {
    send_reg_ = reg;
  }

  return 0;
}

int BaseOperator::PrepareRecv(void *ptr, size_t len, MemoryType mem_type,
                              void *reg) {
  // TODO: (zxy) add support for fpga memory.
  assert(mem_type != FPGA_MEMORY);

  recv_ptr_ = ptr;
  recv_size_ = len;

  if (reg == nullptr) {
    // TODO: (zxy) run rcache lookup.
  } else {
    recv_reg_ = reg;
  }

  // TODO: (zxy) post recv buffer to rnic.
  return 0;
}

int BaseOperator::PrepareBulkSend(std::vector<void *> &bulk_ptr,
                                  std::vector<size_t> &bulk_len) {
  if (bulk_ptr.size() != bulk_len.size()) {
    LOG(ERROR) << "provide " << bulk_ptr.size()
               << " memory chunk, but only provide " << bulk_len.size()
               << " memory chunk length.";

    return -1;
  }

  bulk_send_ = true;
  n_bulk_ = bulk_ptr.size();

  bulk_send_ptr_ = reinterpret_cast<void **>(malloc(sizeof(void *) * n_bulk_));
  if (bulk_send_ptr_ == nullptr) {
    LOG(ERROR) << "Out of memory.";
    return -1;
  }

  bulk_send_size_ =
      reinterpret_cast<size_t *>(malloc(sizeof(size_t) * n_bulk_));
  if (bulk_send_size_ == nullptr) {
    LOG(ERROR) << "Out of memory.";
    free(bulk_send_ptr_);
    bulk_send_ptr_ = nullptr;
    return -1;
  }

  bulk_send_reg_ = reinterpret_cast<void **>(malloc(sizeof(void *) * n_bulk_));
  if (bulk_send_reg_ == nullptr) {
    LOG(ERROR) << "Out of memory.";
    free(bulk_send_ptr_);
    free(bulk_send_size_);
    bulk_send_ptr_ = nullptr;
    bulk_send_size_ = nullptr;
    return -1;
  }

  for (size_t i = 0; i < n_bulk_; i++) {
    // TODO: (zxy) run rcache lookup.
    bulk_send_ptr_[i] = bulk_ptr[i];
    bulk_send_size_[i] = bulk_len[i];
  }

  return 0;
}

int BaseOperator::PrepareBulkSend(std::vector<void *> &bulk_ptr,
                                  std::vector<size_t> &bulk_len,
                                  std::vector<void *> &bulk_reg) {
  if (bulk_ptr.size() != bulk_len.size()) {
    LOG(ERROR) << "provide " << bulk_ptr.size()
               << " memory chunk, but only provide " << bulk_len.size()
               << " memory chunk length.";

    return -1;
  }

  if (bulk_ptr.size() != bulk_reg.size()) {
    LOG(ERROR) << "provide " << bulk_ptr.size()
               << " memory chunk, but only provide " << bulk_len.size()
               << " register result.";

    return -1;
  }

  bulk_send_ = true;
  n_bulk_ = bulk_ptr.size();

  bulk_send_ptr_ = reinterpret_cast<void **>(malloc(sizeof(void *) * n_bulk_));
  if (bulk_send_ptr_ == nullptr) {
    LOG(ERROR) << "Out of memory.";
    return -1;
  }

  bulk_send_size_ =
      reinterpret_cast<size_t *>(malloc(sizeof(size_t) * n_bulk_));
  if (bulk_send_size_ == nullptr) {
    LOG(ERROR) << "Out of memory.";
    free(bulk_send_ptr_);
    bulk_send_ptr_ = nullptr;
    return -1;
  }

  bulk_send_reg_ = reinterpret_cast<void **>(malloc(sizeof(void *) * n_bulk_));
  if (bulk_send_reg_ == nullptr) {
    LOG(ERROR) << "Out of memory.";
    free(bulk_send_ptr_);
    free(bulk_send_size_);
    bulk_send_ptr_ = nullptr;
    bulk_send_size_ = nullptr;
    return -1;
  }

  for (size_t i = 0; i < bulk_ptr.size(); i++) {
    bulk_send_ptr_[i] = bulk_ptr[i];
    bulk_send_reg_[i] = bulk_reg[i];
    bulk_send_size_[i] = bulk_len[i];
  }

  return 0;
}
