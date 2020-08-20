#ifndef __NON_STREAM_OPERATOR_H_
#define __NON_STREAM_OPERATOR_H_

#include "operator.h"

class NonStreamNonBlockOperator : public BaseOperator {
 public:
  NonStreamNonBlockOperator(bool is_send);
  ~NonStreamNonBlockOperator();

  virtual int RunSend(SendDoneCallback done) override;
  virtual int RunRecv(RecvDoneCallback done) override;
};

class NonStreamBlockOperator : public BaseOperator {
 public:
  NonStreamBlockOperator(bool is_send);

  virtual int RunSend(SendDoneCallback done) override;
  virtual int RunRecv(RecvDoneCallback done) override;
};

#endif
