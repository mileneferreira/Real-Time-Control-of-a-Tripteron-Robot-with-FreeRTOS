#pragma once

#include "freertos/idf_additions.h"
#include "portmacro.h"

namespace Task {
  /// Syntatic sugar for querying the pthread configuration of the task
  /// in which these methods are called
  ///
  /// NOTE: This is effectively a namespace, since the struct has only
  /// static methods. However, my LSP shows namespaces and structs with
  /// different colors, which annoyed me.
  struct Query {
    struct this_thread {
      /// Get the name of this task
      static auto name() -> const char*;
      /// Get the core in which this task is running
      static auto core() -> BaseType_t;
      /// Get the priority of this task
      static auto priority() -> UBaseType_t;
      /// Get the free stack of this task
      static auto free_stack() -> UBaseType_t;
    };

    struct pthread {
      /// Get the name configured in pthread
      static auto name() -> const char*;
      /// Get the core configured in pthread for the next tasks
      static auto core() -> int;
      /// Get the priority configured in pthread
      static auto priority() -> size_t;
      /// Get the stack size configured in pthread
      static auto stack_size() -> size_t;
    };
  };

}  // namespace Task
