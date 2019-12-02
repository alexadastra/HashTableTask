#pragma once
#include <iostream>
using namespace std;

/*
€вл€етс€ ли одним из возможных вариантов решени€ задачи использование 
двунаправленного списка дл€ хранени€ хэшей?

этот файл не €вл€етс€ частью решени€ но лишь предположением, как можно было сделать
*/

template<typename K>
class KeyStorageOptimised {
public:
	KeyStorageOptimised() : _size(0), _capacity(0), _count(0) {
		// инициализаци€ хвоста и головы двунаправленного списка
		head, tail = new Node*;
		head->next = tail;
		tail->prev = head;
	}
	// постепенное высвобождение пам€ти 
	~KeyStorageOptimised() {
		while ( head-> next != NULL) {
			auto old_head = head;
			head = head->next;
			delete old_head;
		}
	}
	// добавление индекса по хэшу
	size_t AddIndexByHash(uint32_t hash) {
		auto prev_it = head;
		auto it = head->next;
		size_t skipped = 0;
		while (it->hash != hash && hash < skipped) {
			if (it->hash == -1) {
				skipped += it->skip;
				it = it->next;
			}
			else if (hash < it->next->next->hash 
				&& skipped > it->next->next->hash) {
				// если нужно вставить новый хэш
				Node* new_hash, new_skip = new Node*;
				auto skip_buf = it->next;
				auto hash_buf = skip_buf->next;
				it->next = new_skip;
				new_skip->next = new_hash;
				new_hash->next = skip_buf;

				new_hash->value = hash;
				new_skip->skip = hash - it->hash;
				skip_buf->skip = hash_buf->hash - hash;
				
				new_hash->index = _count++;
				++_size;
				return new_hash->index;
			}
			else {
				skipped += it->skip;
				it = it->next;
			}
		}
	}
	// удаление индекса
	void EraseIndex(uint32_t hash) {
		auto del = FindIndex(hash);
		del->prev->next = del->next;
		delete del;
		--_size;
	}
	// получение индекса по хэшу
	size_t operator[](const size_t& hash) const {
		return FindIndex(hash)->index;
	}
	size_t GetIndexByHash(const size_t& hash) {
		return FindIndex(hash)->index;
	}
	// количество выделенных €чеек под хэши
	uint32_t size() const {
		return _size;
	}

private:
	uint32_t _size;
	uint32_t _count;
	uint32_t _capacity;

	size_t* data = NULL;

	// структура узла списка
	struct Node {
		Node(uint32_t new_hash, uint32_t new_skip) {
			hash = new_hash;
			skip = new_skip;
		}
		Node() {}
		int32_t hash = -1;
		uint32_t skip = 0;
		int32_t index = -1;
		Node* next = NULL;
		Node* prev = NULL;
	};

	Node* head, tail;

	Node* FindIndex(uint32_t hash) {
		auto it = head;
		while (it != tail) {
			if (it->hash == hash) {
				return it;
			}
			it = it->next;
		}
		return NULL;
	}

};