#pragma once

#include <iostream>
#include <functional>
#include <thread>
#include "cqueue.h"

using namespace concurrent;

typedef std::function<void()> Action;

struct Token {
private:
	std::atomic<bool> isActivated_ = { false };

public:
	void activate();
	bool isActivated();
};

typedef Token* PToken;

struct Task {
private:
	Action action_;
	std::atomic<bool> isFinished_ = { false };
	Spinlock lock_;

	Task() = delete;
	Task(Action action);

	void execute();

public:
	bool isFinished();
	void wait(PToken token = nullptr);

	friend struct TaskOwner;
};

typedef Task* PTask;

struct TaskOwner {
private:
	PTask task_;

public:
	TaskOwner(Action action);

	PTask getTask();
	void execute();
};

typedef TaskOwner* PTaskOwner;


class Dispatcher {
private:
	Queue<PTaskOwner>* queue_;
	std::thread* threads;
	int threadsCount_;
	std::atomic<bool> isActive_{ false };
	std::atomic<int> waitings{0};
	PToken disposeToken_;

	void threadMain();

public:
	Dispatcher();
	Dispatcher(int threadsCount);

	bool invoke(Action action);
	PTask beginInvoke(Action action);
	void stopProcessing();
	void resumeProcessing();
	int queueSize();
	bool isEmpty();

	~Dispatcher();
};