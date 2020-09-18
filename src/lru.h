#pragma once

#ifndef __LRU_H__
#define __LRU_H__

#include <unordered_map>

namespace tlib {

template <typename Key_Type, typename Value_Type>
class ListNode {
public:
	ListNode() : next(nullptr), prev(nullptr) {};
	ListNode(Key_Type key_, Value_Type val_) : key(key_), val(val_), next(nullptr), prev(nullptr) {}
	Key_Type getKey() { return this->key; }
	Value_Type getVal() { return this->val; }
	void setNext(ListNode<Key_Type, Value_Type>* node) { this->next = node; }
	void setPrev(ListNode<Key_Type, Value_Type>* node) { this->prev = node; }
	ListNode<Key_Type, Value_Type>* getNext() { return this->next; }
	ListNode<Key_Type, Value_Type>* getPrev() { return this->prev; }

private:
	Key_Type key;
	Value_Type val;
	ListNode<Key_Type, Value_Type>* next;
	ListNode<Key_Type, Value_Type>* prev;
};

template<typename Key_Type, typename Value_Type>
class LinkList {
public:
	LinkList() {
		this->head = new ListNode<Key_Type, Value_Type>();
		this->tail = new ListNode<Key_Type, Value_Type>();
		this->head->setNext(this->tail);
		this->tail->setPrev(this->head);
	}

	void remove(ListNode<Key_Type, Value_Type>* node) {
		node->getNext()->setPrev(node->getPrev());
		node->getPrev()->setNext(node->getNext());
		delete node;
	}

	void insertHead(ListNode<Key_Type, Value_Type>* node) {
		node->setNext(this->head->getNext());
		node->setPrev(this->head);
		this->head->setNext(node);
		node->getNext()->setPrev(node);
	}

	void removeTail() {
		if (this->tail->getPrev() == this->head) return;
		ListNode<Key_Type, Value_Type>* tmp = this->tail->getPrev();
		this->tail->setPrev(tmp->getPrev());
		this->tail->getPrev()->setNext(this->tail);
		delete tmp;
	}

	ListNode<Key_Type, Value_Type>* getTailKey() {
		return this->tail->getPrev();
	}

private:
	ListNode<Key_Type, Value_Type>* head;
	ListNode<Key_Type, Value_Type>* tail;
};

template<typename Key_Type, typename Value_Type>
class LRU {
public:
	LRU() = delete;
	LRU(int capacity) {
		this->m_capacity = capacity;
		this->m_size = 0;
		this->m_list = new LinkList<Key_Type, Value_Type>();
	}

	int get(int key) {
		auto iter = this->m_map.find(key);
		
		// if the key appears in the map.
		if (iter != this->m_map.end()) {
			ListNode<Key_Type, Value_Type>* new_node = new ListNode<Key_Type, Value_Type>(key, this->m_map[key]->getVal());
			this->m_list->remove(iter->second);
			this->m_list->insertHead(new_node);
			this->m_map[key] = new_node;
			return new_node->getVal();
		}
		return -1;
	}

	void put(int key, int val) {
		auto iter = this->m_map.find(key);
		// if the key already existed in the map.
		if (iter != this->m_map.end()) {
			ListNode<Key_Type, Value_Type>* new_node = new ListNode<Key_Type, Value_Type>(key, val);
			this->m_list->remove(iter->second);
			this->m_list->insertHead(new_node);
			this->m_map[key] = new_node;
		}
		// otherwise.
		else {
			// if current size less than the capacity.
			if (this->m_size < this->m_capacity) {
				ListNode<Key_Type, Value_Type>* new_node = new ListNode<Key_Type, Value_Type>(key, val);
				this->m_list->insertHead(new_node);
				this->m_map[key] = new_node;
				this->m_size++;
			}
			// if current size ge the capacity.
			else {
				ListNode<Key_Type, Value_Type>* new_node = new ListNode<Key_Type, Value_Type>(key, val);
				this->m_map.erase(this->m_list->getTailKey()->getKey());
				this->m_list->removeTail();
				this->m_list->insertHead(new_node);
				this->m_map[key] = new_node;
			}
		}
	}

private:
	int m_capacity;
	int m_size;
	std::unordered_map<int, ListNode<Key_Type, Value_Type>*> m_map;
	LinkList<Key_Type, Value_Type>* m_list;
};
}

#endif
