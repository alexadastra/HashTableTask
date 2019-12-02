#pragma once
#include <utility>

#include "KeyStorage.h"
#include "ValueStorage.h"

// ��������� ��������� ��� ����������� �������� ������� ������
// ���� ��� ������������� 
// �� ����������� ���-�������, �� ������������ �� ����������� ����������
template <typename T>
struct default_hash_func {
public:
	uint32_t operator()(const T& arg) {
		return std::hash<T> {}(arg);
	}
};
// ���� ��� �������������
// �� ����������� ���-�������, �� ������������ �������� ==
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
	// ���������� �������� �� ���� ����-��������
	void insert(const std::pair<K, V>& arg) {
		auto hash_value = hasher(arg.first); //���������� ����
		size_t idx;
		// �������� �� ��������: ���� ��� ��������, �� �������� ������ ���
		// ��� ������������ ����� � ��������� ���-�� ���������
		if (hash_value < keys.size() && 
			values.BucketSize(keys.GetIndexByHash(hash_value)) != 0) {
			idx = keys.GetIndexByHash(hash_value);
		}
		// � ��������� ������ �������� ����� ������
		else {
			idx = keys.AddIndexByHash(hash_value);
		}
		// �� ������� �������� ����������� � �����
		values.AddValueByIndex({ idx, arg.second });
	}
	// �������� �������� �� �����
	void erase(K key) {
		auto hash_value = hasher(key);
		KeyValidation(hash_value);
		// �������� �������� �� �����
		values.EraseValue(keys.GetIndexByHash(hash_value));
		// ���� ����� ������, ������������ � ������
		if (values.BucketSize(keys.GetIndexByHash(hash_value)) == 0) {
			keys.EraseIndex(hash_value);
		}
	}
	// ����� �������� �� �����
	V& operator[](const K& key) {
		if (KeyValidation(hasher(key))) {
			V item;
			insert({ key, item });
		}
		return values[keys.GetIndexByHash(hasher(key))];
	}
	// ����� �������� �� �����
	V& at(const K& key) {
		KeyValidation(hasher(key));
		return values[keys.GetIndexByHash(hasher(key))];
	}
	// ���������� ��������� � ����������
	uint32_t size() const{
		return values.size();
	}

private:
	KeyStorage<K> keys;						// ��������� "��� -> ������"
	ValueStorage<V, EqualOperator> values;	// ��������� "������ -> ��������"
	HashFunction hasher;					// ��������� ���-�������

	// ������� ��� �������� ������������� ����� � ���������
	bool KeyValidation(uint32_t hash) {
		return (!(hash < keys.size() &&
			values.BucketSize(keys.GetIndexByHash(hash)) != 0));
		// {
			//throw(std::invalid_argument("No such element in table!"));
		//}
	}
};