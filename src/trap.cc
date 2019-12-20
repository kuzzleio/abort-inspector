#include <stdio.h>
#include <signal.h>
#include <v8.h>
#include <node.h>
#include <nan.h>
#include <stdlib.h>
#include <unistd.h>
#include "libunwind.h"
#include <cxxabi.h>

void abortTrap (int signum, siginfo_t *info, void * ptr) {
  fprintf(stderr, "================== SIGABRT TRAPPED ==================\n");

  if (info->si_pid != getpid()) {
    fprintf(stderr, "Origin: signal sent by an external process (PID: %u)\n", info->si_pid);
    return;
  }

  char buffer[1024];
  unw_context_t *context = (unw_context_t*)ptr;
  unw_cursor_t cursor;

  if (unw_init_local(&cursor, context) < 0) {
    fprintf(stderr, "Unable to get aborted context backtrace\n");
    return;
  }

  while (unw_step(&cursor) > 0) {
    unw_word_t ip, sp, offset;

    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    unw_get_reg(&cursor, UNW_REG_SP, &sp);

    buffer[0] = '\0';
    if (!unw_get_proc_name(&cursor, buffer, 1024, &offset)) {
      int status;
      const char * name = abi::__cxa_demangle(buffer, NULL, NULL, &status);
      if (name != NULL) {
        strcpy(buffer, name);
      }
    }

    if (strlen(buffer) == 0) strcpy(buffer, "<unknown>");

    fprintf(stderr, "ip = %lx, sp = %lx, %s\n", (long) ip, (long) sp, buffer);
  }

  fprintf(stderr, "=== End of stack trace.\n");
}

void monitor (const v8::FunctionCallbackInfo<v8::Value>& args) {
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
