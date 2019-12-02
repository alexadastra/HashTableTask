#pragma once
#include "Bucket.h"

template<typename Value, typename EqualOperator>
	class ValueStorage {
	public:
		ValueStorage() {}

		~ValueStorage() {
			delete[] data;
		}
		// добавление элемента по индексу
		void AddValueByIndex(const std::pair<size_t, Value>& arg) {
			if (arg.first >= _capacity) {
				ExpandValueStorage();
			}
			(data + arg.first)->push_front(arg.second);
			if ((data + arg.first)->size() == 1) {
				++_size;
			}
		}
		// удаление элемента
		void EraseValue(size_t idx) {
			data[idx].pop_front();
			if ((data + idx)->size() == 0) {
				--_size;
			}
		}
		// поиск элемента
		Value& operator[](size_t idx) {
			return data[idx].GetFront();
		}
		// размер коллекции
		size_t size() const {
			return _size;
		}
		// размер ведра по индексу
		size_t BucketSize(size_t idx) const {
			return data[idx].size();
		}
		// указатели на первое и последнее ведро в хранилище
		Bucket<Value, EqualOperator>* begin() {
			return data;
		}
		Bucket<Value, EqualOperator>* end() {
			return data + _size;
		}

	private:
		uint32_t _size = 0;			// количество вёдер
		uint32_t _capacity = 0;		// возможное количество вёдер 
									// без перевыделения памяти

		// указатель на начало хранилища
		Bucket<Value, EqualOperator>* data = NULL; 
		// метод перевыделения памяти блоками вдважды больше предыдущего
		void ExpandValueStorage() {
			auto new_capacity = _capacity == 0 ? 1 : 2 * _capacity;
			Bucket<Value, EqualOperator>* _new_data = 
				new Bucket<Value, EqualOperator>[new_capacity];
			std::move(this->begin(), this->end(), _new_data);
			data = _new_data;
			_capacity = new_capacity;
		}
};