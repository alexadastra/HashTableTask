#pragma once
#include <iostream>

template<typename K>
	class KeyStorage {
	public:
		KeyStorage() : _size(0), _capacity(0), _count(0){}

		~KeyStorage() {
			delete[] data;
		}
		// добавление индекса по хэшу
		size_t AddIndexByHash(const uint32_t& hash) {
			// если хэш больше максимально возможного на данный момент,
			// перевыделяется память
			if (hash >= _size) {
				ExpandStorage(hash + 1);
			}
			// в массив заносится новый индекс
			data[hash] = _count++;
			return data[hash];
		}
		// инвалидация индекса
		void EraseIndex(uint32_t hash) {
			// из-за присвоения беззнаковому типу будет 
			// вызывать ошибки при попытке обращения 
			data[hash] = -1; 
		}
		// получение индекса по хэшу
		size_t operator[](const size_t& hash) const{
			return data[hash];
		}
		size_t GetIndexByHash(const size_t& hash) {
			return data[hash];
		}
		// количество выделенных ячеек под хэши
		uint32_t size() const {
			return _size;
		}
		// получение начала и конца хранилища 
		size_t* begin() {
			return data;
		}
		size_t* end() {
			return data + _size;
		}

	private:
		uint32_t _size; 
		uint32_t _count; // счётчик для индексов 
		uint32_t _capacity;

		size_t* data = NULL;

		struct Node {
			Node(uint32_t new_hash, uint32_t new_skip) {
				hash = new_hash;
				skip = new_skip;
			}
			Node() {}
			uint32_t hash = -1;
			uint32_t skip = 0;
		};
		// метод для выделения памяти под инлексы
		void ExpandStorage(uint32_t new_size) {
			size_t* _new_data = new size_t[new_size];
			std::move(begin(), end(), _new_data);
			delete[] data;
			data = _new_data;
			_size = new_size;
		}
};