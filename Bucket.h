#pragma once

template <typename T>
struct Node {
	T value;
	Node* next = NULL;
};
/*
ведро хэш-таблицы, работающее по принципу стэка:
в случае коллизии будет доступен только элемент, 
добавленный последним, пока он не будет удалён
*/
template <typename T, typename EqualOperator>
class Bucket {
public:
	Bucket() : _size(0){
		front = new Node<T>;
	}

	~Bucket() {
		while (_size> 0) {
			pop_front();
		}
	}
	// добавление элемента в начало стэка
	void push_front(const T& arg) {
		if (!(equaliser(arg, front->value))) {
			Node<T>* new_front = new Node<T>;
			new_front->next = front;
			new_front->value = arg;
			front = new_front;
			++_size;
		}
	}
	// извлечение элемента из начала
	void pop_front() {
		auto old_front = front;
		front = front->next;
		delete old_front;
		--_size;
	}
	// количество элементов в ведре
	size_t size() const {
		return _size;
	}
	// константнй и неконстантный методы получения элемента
	T& GetFront() {
		return front->value;
	}
	T& GetFront() const{
		return front->value;
	}

private:
	uint32_t _size;
	Node<T>* front = NULL;
	EqualOperator equaliser;
};