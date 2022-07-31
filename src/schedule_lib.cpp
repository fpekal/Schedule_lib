#include "../include/schedule_lib.h"

static Schedule Schedule::instance;

Schedule::~Schedule() {
	active = false;
	thread.join();
}

void Schedule::thread_func() {
	while(active) {
		synchronize();
		iterate_delays();
		iterate_repeats();
	}
}

void Schedule::synchronize() {
	if (sync.load(std::memory_order_acquire)) {
		sync.store(false, std::memory_order_relaxed);
	}
}

void Schedule::iterate_delays() {
	for (auto iter = delays.begin(); iter != delays.end(); ++iter) {
		auto delta_time = std::chrono::system_clock::now() - iter->start_point;

		if (delta_time >= iter->duration) {
			if (iter->function != nullptr)
				iter->function();
			delays.erase(iter);
			break;
		}
	}
}

void Schedule::iterate_repeats() {
	for (auto iter = repeats.begin(); iter != repeats.end(); ++iter) {
		auto delta_time = std::chrono::system_clock::now() - iter->last_call;

		if (delta_time >= iter->interval) {
			if (iter->function != nullptr)
				iter->function();
			iter->last_call = std::chrono::system_clock::now();
		}
	}
}

Schedule::Delay& Schedule::Delay::time(std::chrono::seconds duration) {
	this->duration = duration;
	instance.sync.store(true, std::memory_order_release);
	return *this;
}

Schedule::Delay& Schedule::Delay::call(std::function<void()> function) {
	this->function = function;
	instance.sync.store(true, std::memory_order_release);
	return *this;
}

Schedule::Delay& Schedule::make_delay() {
	instance.delays.push_back(Delay{});
	instance.sync.store(true, std::memory_order_release);
	return instance.delays.back();
}

Schedule::Repeating& Schedule::Repeating::time(std::chrono::seconds interval) {
	this->interval = interval;
	instance.sync.store(true, std::memory_order_release);
	return *this;
}

Schedule::Repeating& Schedule::Repeating::call(std::function<void()> function) {
	this->function = function;
	instance.sync.store(true, std::memory_order_release);
	return *this;
}

void Schedule::Repeating::stop() {
	instance.repeats.erase(iterator);
}

Schedule::Repeating& Schedule::make_repeating() {
	instance.repeats.push_back(Repeating{});
	instance.sync.store(true, std::memory_order_release);
	instance.repeats.back().iterator = --instance.repeats.end();
	return instance.repeats.back();
}
