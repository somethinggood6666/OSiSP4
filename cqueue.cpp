#include "cqueue.h"

namespace concurrent {

	template<typename T>
	eNode<T>::QueueNode(T* data) {
		
	}

	template<typename T>
	Queue<T>::Queue() {
		
	};

	template<typename T>
	void Queue<T>::enqueue(T* item) {
		
	};

	template<typename T>
	T* Queue<T>::dequeue() {
		
	};

	template<typename T>
	T* Queue<T>::peek() {
		
	};

	template<typename T>
	int Queue<T>::count() {
		
	};

	template<typename T>
	bool Queue<T>::isEmpty() {
		
	}

	template<typename T>
	void Queue<T>::clear() {
		
	}

	template<typename T>
	Queue<T>::~Queue()
	{
		clear();
	};
}

