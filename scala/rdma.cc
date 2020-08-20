#include "Rdma.h"
#include <iostream>
#include <string.h>
#include "operator.h"

struct JniContext
{
  JavaVM   *javaVM;
  jclass   jniRdmaClz;
  jobject  jniRdmaObj;
};
JniContext g_ctx;


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
  JNIEnv* env;
  memset(&g_ctx, 0, sizeof(g_ctx));
  g_ctx.javaVM = vm;
  if (vm->GetEnv((void**)&env, JNI_VERSION_1_8) != JNI_OK) {
        return JNI_ERR; // JNI version not supported.
  }

  jclass clz = env->FindClass("Rdma");
  g_ctx.jniRdmaClz = reinterpret_cast<jclass>(env->NewGlobalRef(clz));
  jmethodID cbFunc= env->GetMethodID(g_ctx.jniRdmaClz, "<init>", "()V");
  jobject handler = env->NewObject(g_ctx.jniRdmaClz, cbFunc);
  g_ctx.jniRdmaObj = env->NewGlobalRef(handler);
  return JNI_VERSION_1_8;

}


JNIEXPORT jint JNICALL Java_Rdma_send_1binary_1nonblock
  (JNIEnv *env, jobject instance, jlong sockid, jbyteArray bytes, jlong size, jint trans_mode)
{
  bool is_send = true;
  char *data;
  JniContext *pctx = &g_ctx;
  jbyte *array_head = env->GetByteArrayElements(bytes, 0);
  data = (char *)array_head;

  auto callbackFunc = [pctx, env](void* ptr, size_t arg) {
      jmethodID funcId = env->GetMethodID(pctx->jniRdmaClz, "callback", "(I)I");
      int result = env->CallIntMethod(pctx->jniRdmaObj, funcId, (jint)arg);
      return result;
  };

  BaseOperator binaryOp(is_send);
  binaryOp.RunSend(callbackFunc);
  int retcode = 0;
  return retcode;
}


