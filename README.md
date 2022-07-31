# Schedule_lib
## What it does?
Schedule_lib allows for calling any function after some time. Everything is done on a separate thread, so you don't have to worry about anything.  

## Example
```cpp
#include <iostream>
#include <functional>
#include <chrono>
#include <atomic>
#include <schedule_lib.h>

template<class T>
void print_value(const T& val) {
	std::cout << val << '\n';
}

int main() {
	using namespace std::chrono_literals;

	std::atomic<double> d = 0; // just to make sure everything is fine

	// Repeats `print_value` function every second.
	Schedule::make_repeating().time(1s).call(std::bind(print_value<decltype(d)>, std::ref(d)));

	while(true) {
		// Increase value by small amount. +- 1 per second on my machine
		d += 2e-8;
	}
}
```
The library is used here for debugging. We can see a value of variable `d` while main thread is in an  infinite loop.

## How to use
The library gives us two functions. Both constructs an object and stores it in a container.  
These functions are named `Schedule::make_delay()` and `Schedule::make_repeating()`.  
Each returns an lvalue reference to the constructed object. Then, using methods, we can configure these objects.  

There are two methods: `time()` and `call()`.  
`time()` sets interval or delay after which function will be called.  
`call()` provides a function to call.  

`Repeating` has also 2 additional methods.  
`stop()` stops executing and destroys object.  
`limit()` sets limit to how many time function should be called before automatically calling `stop()`. (WIP)
