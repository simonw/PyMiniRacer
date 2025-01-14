#include "isolate_memory_monitor.h"

namespace MiniRacer {

IsolateMemoryMonitor::IsolateMemoryMonitor(v8::Isolate* isolate)
    : isolate_(isolate),
      soft_memory_limit_(0),
      soft_memory_limit_reached_(false),
      hard_memory_limit_(0),
      hard_memory_limit_reached_(false) {
  isolate_->AddGCEpilogueCallback(StaticGCCallback, this);
}

void IsolateMemoryMonitor::StaticGCCallback(v8::Isolate* isolate,
                                            v8::GCType /*type*/,
                                            v8::GCCallbackFlags /*flags*/,
                                            void* data) {
  static_cast<IsolateMemoryMonitor*>(data)->GCCallback(isolate);
}

void IsolateMemoryMonitor::GCCallback(v8::Isolate* isolate) {
  v8::HeapStatistics stats;
  isolate->GetHeapStatistics(&stats);
  size_t used = stats.used_heap_size();

  soft_memory_limit_reached_ =
      (soft_memory_limit_ > 0) && (used > soft_memory_limit_);
  isolate->MemoryPressureNotification((soft_memory_limit_reached_)
                                          ? v8::MemoryPressureLevel::kModerate
                                          : v8::MemoryPressureLevel::kNone);
  if ((hard_memory_limit_ > 0) && used > hard_memory_limit_) {
    hard_memory_limit_reached_ = true;
    isolate->TerminateExecution();
  }
}

void IsolateMemoryMonitor::SetHardMemoryLimit(size_t limit) {
  hard_memory_limit_ = limit;
  hard_memory_limit_reached_ = false;
}

void IsolateMemoryMonitor::SetSoftMemoryLimit(size_t limit) {
  soft_memory_limit_ = limit;
  soft_memory_limit_reached_ = false;
}

IsolateMemoryMonitor::~IsolateMemoryMonitor() {
  isolate_->RemoveGCEpilogueCallback(StaticGCCallback, this);
}

}  // end namespace MiniRacer
