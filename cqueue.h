#pragma once

#include "spinlock.h"

namespace concurrent 
{
	template <typename T>
	class Queue
	{
	private:
		template <typename T>
		struct Node {
			T data;
			Node<T>* next;

			Node(T data) {
				this->data = data;
				this->next = nullptr;
			}
		};

		Spinlock lock_;
		Node<T>* first_;
		Node<T>* last_;

	public:
		Queue() {
			first_ = nullptr;
			last_ = nullptr;
		}

		void enqueue(T item) {
			Node<T>* node = new Node<T>(item);
			node->next = nullptr;

			lock_.lock();
			if (first_ != nullptr)
			{
				last_->next = node;
			}
			else
			{
				first_ = node;
			}
			last_ = node;
			lock_.unlock();
		}
		bool tryDequeue(T* res) {
			lock_.lock();
			if (first_ != nullptr) {
				*res = first_->data;

				Node<T>* next = first_->next;
				delete first_;
				first_ = next;

				if (first_ == nullptr)
					last_ = nullptr;

				lock_.unlock();
				return true;
			}
			else
			{
				lock_.unlock();
				return false;
			}
		}
		bool tryPeek(T* res) {
			lock_.lock();
			if (first_ != nullptr) {
				*res = first_->data;
				lock_.unlock();
				return true;
			}
			else
			{
				lock_.unlock();
				return false;
			}
		}
		int count() {
			int res = 0;

			lock_.lock();
			Node<T>* t = first_;
			while (t != nullptr)
			{
				res++;
				t = t->next;
			}
			lock_.unlock();

			return res;
		}
		bool isEmpty() {
			bool res;

			lock_.lock();
			res = first_ == nullptr;
			lock_.unlock();

			return res;
		}
		void clear() {
			lock_.lock();
			while (first_ != nullptr)
			{
				Node<T>* next = first_->next;
				delete first_;
				first_ = next;
			}
			lock_.unlock();
		}

		~Queue() {
			clear();
		}
	};
}


