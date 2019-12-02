#include "HashTable.h"
#include "test_runner.h"

struct StandartHash {
public:
	std::uint32_t operator()(const std::string& arg) {
		std::uint32_t res = 0;
		std::uint16_t q = 1;
		for (char i : arg) {
			res += i * q;
			q *= 10;
		}
		return res;
	}
};

struct SimpleStringHash {
public:
	std::uint32_t operator()(const std::string& arg) {
		return 2 * arg.size();
	}
};

void ConstructionTest() {
	HashTable<std::string, int, StandartHash, int> table;
	HashTable<std::string, int> table2;
	HashTable<std::string, int, StandartHash> table3;
}

void TestOnSimpleStrings() {
	HashTable<std::string, std::string, SimpleStringHash> table;
	table.insert({ "a", "b" });
	ASSERT_EQUAL(table.at("a"), "b");
	table.insert({ "aa", "bb" });
	ASSERT_EQUAL(table["aa"], "bb");
	table.insert({ "aaa", "bbb" });
	table.insert({ "ab", "ba" });
	ASSERT_EQUAL(table.at("ab"), "ba");
	table.erase("ab");
	ASSERT_EQUAL(table.at("ab"), "bb");
	table["ccc"] = "dddd";
	ASSERT_EQUAL(table["ccc"], "dddd");
	//std::vector<std::pair<std::string, std::string>> words(table.begin(), table.end());
}

void TestOnInts() {
	HashTable<std::string, int, StandartHash> table;
	table.insert({ "a", 0 });
	++table["a"];
	ASSERT_EQUAL(table["a"], 1);
}

void TestOnText() {

}

int main()
{
	TestRunner tr;
	RUN_TEST(tr, ConstructionTest);
	RUN_TEST(tr, TestOnSimpleStrings);
	RUN_TEST(tr, TestOnInts);
	
	return 0;
}