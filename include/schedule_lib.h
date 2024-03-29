#ifndef SCHEDULE_LIB_H
#define SCHEDULE_LIB_H

#include <thread>
#include <atomic>
#include <functional>
#include <chrono>
#include <list>

class Schedule {
	static Schedule instance;

	Schedule() : thread{ std::bind(&Schedule::thread_func, this) }, active{ true }, sync{ false } {}

	~Schedule();

	std::thread thread;
	bool active;

	void thread_func();

	void synchronize();

	void iterate_delays();
	void iterate_repeats();

public:
	class Delay {
		friend class Schedule;

		std::chrono::milliseconds duration;
		std::function<void()> function;

		std::chrono::system_clock::time_point start_point;

		Delay() : start_point{ std::chrono::system_clock::now() }, duration{ 0 }, function{ nullptr } {};

	public:
		// Set delay after which callable object will be called.
		Delay& time(std::chrono::milliseconds duration);
		// Set callable object to call.
		Delay& call(std::function<void()> function);
	};

	// Create `Delay` object.
	static Delay& make_delay();

	class Repeating {
		friend class Schedule;

		std::chrono::milliseconds interval;
		std::function<void()> function;
		long long left_calls;

		std::chrono::system_clock::time_point last_call;

		std::list<Repeating>::iterator iterator;

		Repeating() : last_call{ std::chrono::system_clock::now() }, interval{ 0 }, function{ nullptr }, left_calls{ -1 } {};

	public:
		// Set interval for calling.
		Repeating& time(std::chrono::milliseconds interval);
		// Set callable object to call.
		Repeating& call(std::function<void()> function);
		// Stop further repeating and delete object.
		void stop();
		// Set limit to how many calls will be made.
		Repeating& limit(long long left_calls);
	};

	// Create `Repeating` object.
	static Repeating& make_repeating();

private:
	std::list<Delay> delays;
	std::list<Repeating> repeats;

	std::atomic<bool> sync;		// idk if it's needed //TODO: test that

	friend class Schedule::Delay;
	friend class Schedule::Repeating;
};

#endif
