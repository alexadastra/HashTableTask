#pragma once

template <typename T>
struct Node {
	T value;
	Node* next = NULL;
};
/*
����� ���-�������, ���������� �� �������� �����:
� ������ �������� ����� �������� ������ �������, 
����������� ���������, ���� �� �� ����� �����
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
	// ���������� �������� � ������ �����
	void push_front(const T& arg) {
		if (!(equaliser(arg, front->value))) {
			Node<T>* new_front = new Node<T>;
			new_front->next = front;
			new_front->value = arg;
			front = new_front;
			++_size;
		}
	}
	// ���������� �������� �� ������
	void pop_front() {
		auto old_front = front;
		front = front->next;
		delete old_front;
		--_size;
	}
	// ���������� ��������� � �����
	size_t size() const {
		return _size;
	}
	// ���������� � ������������� ������ ��������� ��������
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