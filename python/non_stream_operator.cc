#include "non_stream_operator.h"
#include "log.h"

NonStreamNonBlockOperator::NonStreamNonBlockOperator(bool is_send)
    : BaseOperator(is_send) {}

int NonStreamNonBlockOperator::RunSend(SendDoneCallback done) { return 0; }

int NonStreamNonBlockOperator::RunRecv(RecvDoneCallback done) { return 0; }

NonStreamBlockOperator::NonStreamBlockOperator(bool is_send)
    : BaseOperator(is_send) {}

int NonStreamBlockOperator::RunSend(SendDoneCallback done) { return 0; }

int NonStreamBlockOperator::RunRecv(RecvDoneCallback done) { return 0; }
