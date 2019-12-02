#pragma once
#include <utility>

#include "KeyStorage.h"
#include "ValueStorage.h"

// шаблонные структуры для стандартных значений шаблона класса
// если при инициализации 
// не указывается хэш-функция, то используется из стандартной библиотеки
template <typename T>
struct default_hash_func {
public:
	uint32_t operator()(const T& arg) {
		return std::hash<T> {}(arg);
	}
};
// если при инициализации
// не указывается хэш-функция, то используется оператор ==
template <typename T>
struct default_eguals_op {
public:
	bool operator()(const T& lhs, const T& rhs) {
		return lhs == rhs;
	}
};

template<typename K, typename V,
typename HashFunction = default_hash_func<V>,
typename EqualOperator = default_eguals_op<V>>
class HashTable {
public:
	HashTable() {}

	~HashTable() {

	}
	// добавление элемента по паре ключ-значение
	void insert(const std::pair<K, V>& arg) {
		auto hash_value = hasher(arg.first); //нахождение хэша
		size_t idx;
		// проверка на коллизии: если она возможна, то достаётся индекс под
		// уже существующее ведро с ненулевым кол-ом элементов
		if (hash_value < keys.size() && 
			values.BucketSize(keys.GetIndexByHash(hash_value)) != 0) {
			idx = keys.GetIndexByHash(hash_value);
		}
		// в противном случае создаётся новый индекс
		else {
			idx = keys.AddIndexByHash(hash_value);
		}
		// по индексу значение добавляется в ведро
		values.AddValueByIndex({ idx, arg.second });
	}
	// удаление элемента по ключу
	void erase(K key) {
		auto hash_value = hasher(key);
		KeyValidation(hash_value);
		// удаление значения из ведра
		values.EraseValue(keys.GetIndexByHash(hash_value));
		// если ведро пустое, инвалидируем и индекс
		if (values.BucketSize(keys.GetIndexByHash(hash_value)) == 0) {
			keys.EraseIndex(hash_value);
		}
	}
	// поиск элемента по ключу
	V& operator[](const K& key) {
		if (KeyValidation(hasher(key))) {
			V item;
			insert({ key, item });
		}
		return values[keys.GetIndexByHash(hasher(key))];
	}
	// поиск элемента по ключу
	V& at(const K& key) {
		KeyValidation(hasher(key));
		return values[keys.GetIndexByHash(hasher(key))];
	}
	// количество элементов в контейнере
	uint32_t size() const{
		return values.size();
	}

private:
	KeyStorage<K> keys;						// хранилище "хэш -> индекс"
	ValueStorage<V, EqualOperator> values;	// хранилище "индекс -> значение"
	HashFunction hasher;					// структура хэш-функции

	// функция для проверки существования ключа в коллекции
	bool KeyValidation(uint32_t hash) {
		return (!(hash < keys.size() &&
			values.BucketSize(keys.GetIndexByHash(hash)) != 0));
		// {
			//throw(std::invalid_argument("No such element in table!"));
		//}
	}
};