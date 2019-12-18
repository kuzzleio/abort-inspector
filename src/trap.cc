#include <stdio.h>
#include <signal.h>
#include <v8.h>
#include <node.h>
#include <nan.h>
#include <stdlib.h>
#include <unistd.h>
#include <execinfo.h>
#include <ucontext.h>

void abortTrap (int signum, siginfo_t *info, void * ptr) {
  fprintf(stderr, "================== SIGABRT TRAPPED ==================\n");

  if (info->si_pid != getpid()) {
    fprintf(stderr, "Origin: signal sent by an external process (PID: %u)\n", info->si_pid);
  }
  else {
    void *traces[50];
    size_t size = backtrace(traces, 50);

    char **messages = backtrace_symbols(traces, size);

    for (size_t i = 1; i < size && messages != NULL; ++i) {
      fprintf(stderr, "[bt]: (%lu) %s\n", i, messages[i]);
    }
  }

}

void monitor(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(v8::Null(isolate));

  struct sigaction action;
  action.sa_sigaction = &abortTrap;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_SIGINFO;

  sigaction(SIGABRT, &action, NULL);
  fprintf(stderr, "Abort Inspector: now trapping SIGABRT signals.");
}

void init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "monitor", monitor);
}


NODE_MODULE(NODE_GYP_MODULE_NAME, init)
