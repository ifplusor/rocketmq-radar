/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ROCKETMQ_UTILITY_THREAD_THREAD_HPP_
#define ROCKETMQ_UTILITY_THREAD_THREAD_HPP_

#include <functional>
#include <memory>
#include <string>
#include <system_error>
#include <thread>
#include <utility>

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <pthread.h>
#if defined(__APPLE__)
#include <sys/proc_info.h>
#endif
#endif

#include "utility/memory/MakeUnique.hpp"

namespace rocketmq {

#ifdef _WIN32
// From https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
// Note: The thread name is only set for the thread if the debugger is attached.

const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push, 8)
typedef struct tagTHREADNAME_INFO {
  DWORD dwType;      // Must be 0x1000.
  LPCSTR szName;     // Pointer to name (in user addr space).
  DWORD dwThreadID;  // Thread ID (-1=caller thread).
  DWORD dwFlags;     // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void SetWindowsThreadName(DWORD dwThreadID, const char* threadName) {
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = threadName;
  info.dwThreadID = dwThreadID;
  info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable : 6320 6322)
  __try {
    RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
  } __except (EXCEPTION_EXECUTE_HANDLER) {
  }
#pragma warning(pop)
}
#endif

class Thread {
 public:
  virtual ~Thread() = default;

  Thread() = default;
  Thread(std::string name) : name_(std::move(name)) {}

  template <class Function, class... Args>
  Thread(std::string name, Function&& f, Args&&... args)
      : name_(std::move(name)), target_(std::bind(std::forward<Function>(f), std::forward<Args>(args)...)) {}

  // disable copy
  Thread(const Thread&) = delete;
  Thread& operator=(const Thread&) = delete;

  // enable move
  Thread(Thread&&) = default;
  Thread& operator=(Thread&&) = default;

  template <class Function, class... Args>
  void SetTarget(Function&& f, Args&&... args) {
    target_ = std::bind(std::forward<Function>(f), std::forward<Args>(args)...);
  }

  void Start() { thread_ = MakeUnique<std::thread>(&Thread::DoRun, this); }

  virtual void Run() {
    if (target_ != nullptr) {
      target_();
    }
  }

  bool Joinable() const noexcept {
    if (thread_ != nullptr) {
      return thread_->joinable();
    }
    return false;
  }

  std::thread::id GetId() const noexcept {
    if (thread_ != nullptr) {
      return thread_->get_id();
    }
    return {};
  }

  void Join() {
    if (thread_ != nullptr) {
      return thread_->join();
    }
    throw std::system_error(std::make_error_code(std::errc::invalid_argument));
  }

  static void SetThreadName(const std::string& name) {
    Thread*& this_thread = GetThisThread();
    if (this_thread != nullptr) {
      if (this_thread->name_ == name) {
        return;
      }
      this_thread->name_ = name;
    }

#if defined(_WIN32)
    // Naming should not be expensive compared to thread creation and connection set up, but if
    // testing shows otherwise we should make this depend on DEBUG again.
    SetWindowsThreadName(::GetCurrentThreadId(), name.c_str());
#elif defined(__APPLE__)
    // Maximum thread name length on OS X is MAXTHREADNAMESIZE (64 characters). This assumes
    // OS X 10.6 or later.
    pthread_setname_np(name.substr(0, MAXTHREADNAMESIZE - 1).c_str());
#elif defined(__linux__)
    // Maximum thread name length supported on Linux is 16 including the null terminator. Ideally
    // we use short and descriptive thread names that fit: this helps for log readibility as well.
    // Since several components set verbose thread names with a uniqifier at the end, we do a split
    // truncation of "first7bytes.last7bytes".
    if (name.size() > 15) {
      std::string shortName = name.substr(0, 7) + '.' + name.substr(name.size() - 7);
      pthread_setname_np(pthread_self(), shortName.c_str());
    } else {
      pthread_setname_np(pthread_self(), name.c_str());
    }
#endif
  }

 private:
  static Thread*& GetThisThread() {
    static thread_local Thread* this_thread;
    return this_thread;
  }

  void DoRun() {
    GetThisThread() = this;

    if (!name_.empty()) {
      std::string name = name_;
      name_.clear();
      SetThreadName(name);
    }

    Run();
  }

 private:
  std::string name_;
  std::unique_ptr<std::thread> thread_;
  std::function<void()> target_;
};

}  // namespace rocketmq

#endif  // ROCKETMQ_UTILITY_THREAD_THREAD_HPP_
