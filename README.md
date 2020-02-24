# Work-Stealing Queue

A fast single-header work-stealing queue template written in modern C++17

# How to Use

A work-stealing queue enables one thread (queue owner) to 
push/pop items into/from one end of the queue,
and multiple threads (thieves) to steal items from the other end.
The following example
shows the basic use of [wsq.hpp](wsq.hpp).

```cpp
// work-stealing queue of integer numbers
WorkStealingQueue<int> queue;

// only one thread can push and pop items from one end
std::thread owner([&] () {
  for(int i=0; i<100000000; i=i+1) {
    queue.push(i);
  }
  while(!queue.empty()) {
    std::optional<int> item = queue.pop();
  }
});

// multiple threads can steal items from the other end
std::thread thief([&] () {
  while(!queue.empty()) {
    std::optional<int> item = queue.steal();
  }
});

owner.join();
thief.join();
```

The library is a single header file. 
Simply compile your source with include path to [wsq.hpp](wsq.hpp).

# Compile Examples and Unittests

We use cmake to manage the package. We recommand using out-of-source build:

```bash
~$ mkdir build
~$ cd build
~$ make & make test
```

# Technical Details

This library implements the work-stealing queue algorithm
described in the paper, 
"[Correct and Efficient Work-Stealing for Weak Memory Models](references/ppopp13.pdf)," 
published by Nhat Minh Lê, 
Antoniu Pop, Albert Cohen, and Francesco Zappa Nardelli
at 2013 ACM Principles and Practice of Parallel Programming (PPoPP).

# Robustness

This library is part of the 
[Cpp-Taskflow](https://github.com/cpp-taskflow/cpp-taskflow)
project.
It has experienced thousands of tests in both micro-benchmarks
and large-scale real-world parallel applications.




