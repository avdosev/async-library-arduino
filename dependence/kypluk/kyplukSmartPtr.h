#pragma once

#include "kyplukDefine.h"

namespace kypluk {

template <class type>
class UniquePtr {
	private:
		using pointer = type*;
		pointer ptr;
        UniquePtr(const UniquePtr&) {}
        void operator = (const UniquePtr&) {}
	public:
		UniquePtr(pointer ptr = NULL) {
			this->ptr = ptr;
		}
		~UniquePtr() {
			if (ptr) delete ptr;
		}
		
		pointer release() {
			pointer temp = ptr;
			ptr = NULL;
			return temp;
		} 
		void reset(pointer ptr = NULL) {
			if (this->ptr) delete this->ptr;
			this->ptr = ptr;
		}
		
		void swap(UniquePtr &other) {
			this->ptr = other.ptr;
			other.ptr = NULL;
		}
		
		pointer get() {
			return ptr;
		}
		
		UniquePtr& operator = (UniquePtr &other) {
			this->swap(other);
			return *this;
		}
		
		UniquePtr& operator = (pointer other) {
			this->reset(other);
			return *this;
		}
		
		operator bool () {
			return ptr != NULL;
		}
		
		type& operator * () {
			return *ptr;
		}
		
        pointer operator -> () {
			return get();
		}
		
        operator pointer () {
			return ptr;
		}
};

template <class type>
class SharedPtr {
	private:
		using pointer = type*;
		struct Node {
			mutable Size_t size;
			type* value;
			Node(type *value = NULL) {
				this->value = value;
				size = 1; 
			}
		};
		Node * node;
	public:
		SharedPtr(type *value = NULL) {
			node = new Node(value);
		}
		
		SharedPtr(const SharedPtr & other) {
			node = other.node;
			other.node->size++;
		}
		
		~SharedPtr() {
			node->size--;
			if (node->size == 0) {
				if (node->value) delete node->value;
				delete node;
			}
		}
		 
		void reset(pointer ptr = NULL) {	
			if (node->size == 1) {
				if (node->value) delete node->value;
				node->value = ptr;
			} else {
				node->size--;
				node = new Node(ptr);
			}
		}
		
		bool unique() {
			return node->value;
		}
		
		void swap(SharedPtr &other) {
			swap(this->node, other->node);
		}
		
		pointer get() {
			return node->value;
		}
		
		SharedPtr& operator = (const SharedPtr &other) {
			node->size--;
			if (node->size == 0) {
				if (node->value) delete node->value;
				delete node;
			}
			this->node = other.node;
			this->node->size++;
			return *this;
		}
		
		SharedPtr& operator = (pointer other) {
			this->reset(other);
			return *this;
		}
		
		operator bool () {
			return node->value != NULL;
		}
		
		type& operator * () {
			return *get();
		}
		
		pointer operator -> () {
			return get();
		}
};

}
