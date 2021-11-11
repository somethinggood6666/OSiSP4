#include "dispatcher.h"

using namespace concurrent;

Dispatcher::Dispatcher()
	:Dispatcher(8)
{
	
}

Dispatcher::Dispatcher(int threadsCount)
{
	queue_ = new Queue<PTaskOwner>();

	disposeToken_ = new Token();
	threadsCount_ = threadsCount;
	threads = new std::thread[threadsCount_];
	for (int i = 0; i < threadsCount_; i++)
		threads[i] = std::thread(&Dispatcher::threadMain, this);

	waitings = 0;
	isActive_ = true;
}

void Dispatcher::threadMain() {
	PTaskOwner owner = nullptr;
	while (isActive_) 
	{
		if (queue_->tryDequeue(&owner))
		{
			owner->execute();
			delete owner;
		}
			
	}
}

bool Dispatcher::invoke(Action action)
{
	waitings++;
	PTask task = beginInvoke(action);
	task->wait(disposeToken_);
	bool isFinished = task->isFinished();
	delete task;
	waitings--;

	return isFinished;
}

PTask Dispatcher::beginInvoke(Action action)
{
	PTaskOwner owner = new TaskOwner(action);
	PTask task = owner->getTask();
	queue_->enqueue(owner);
	return task;
}

void Dispatcher::stopProcessing()
{
	isActive_ = false;
	for (int i = 0; i < threadsCount_; i++)
		threads[i].join();
}

void Dispatcher::resumeProcessing()
{
	isActive_ = true;
	for (int i = 0; i < threadsCount_; i++)
		threads[i] = std::thread(&Dispatcher::threadMain, this);
}

int Dispatcher::queueSize()
{
	return queue_->count();
}

bool Dispatcher::isEmpty()
{
	return queue_->isEmpty();
}

Dispatcher::~Dispatcher()
{
	disposeToken_->activate();
	stopProcessing();
	PTaskOwner owner = nullptr;
	while (queue_->tryDequeue(&owner))
	{
		delete owner;
	}

	delete queue_;
	delete [] threads;

	while (waitings > 0)
	{
		std::this_thread::yield();
	}
	
	delete disposeToken_;
}

Task::Task(Action action)
{
	isFinished_ = false;
	action_ = action;
}

void Task::execute() 
{
	lock_.lock();

	if (isFinished_)
	{
		lock_.unlock();
		return;
	}
		

	if (action_ != nullptr) {
		action_();
	}

	isFinished_ = true;

	lock_.unlock();
}

bool Task::isFinished()
{
	return isFinished_;
}

void Task::wait(PToken token)
{
	if (token == nullptr)
	{
		while (!isFinished_)
		{
			std::this_thread::yield();
		}
	}
	else
	{
		while (!token->isActivated() && !isFinished_)
		{
			std::this_thread::yield();
		}
	}
}

TaskOwner::TaskOwner(Action action)
{
	task_ = new Task(action);
}

PTask TaskOwner::getTask()
{
	return task_;
}

void TaskOwner::execute()
{
	task_->execute();
}

void Token::activate()
{
	isActivated_ = true;
}

bool Token::isActivated()
{
	return isActivated_;
}
