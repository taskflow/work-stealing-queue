#include <thread>
#include <iostream>
#include <wsq.hpp>

int main() {
  
  // work-stealing queue of integer items
  WorkStealingQueue<int> queue;
  
  // only one thread can push and pop
  std::thread owner([&] () {
    for(int i=0; i<100000000; i=i+1) {
      queue.push(i);
    }
    while(!queue.empty()) {
      std::optional<int> item = queue.pop();
    }
  });

  // multiple thives can steal
  std::thread thief([&] () {
    while(!queue.empty()) {
      std::optional<int> item = queue.steal();
    }
  });

  owner.join();
  thief.join();

  return 0;
}


