#include "operator.h"

int main(void) {
  auto send_cb = [](void *ptr, size_t size) {
    LOG(INFO) << "finish send content in addr " << ptr << ", send size "
              << size;
  };

  BaseOperator op(true);
  op.RunSend(send_cb);
  return 0;
}
