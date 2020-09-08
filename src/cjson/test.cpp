#include "cjson.h"
#include <iostream>

int main() {
	tlib::CJSON* json1 = new tlib::CJSON("int_key", 123);
	tlib::CJSON* json2 = new tlib::CJSON("double_key", 1.5);
	tlib::CJSON* json3 = new tlib::CJSON("string_key", "hahaha");
	tlib::CJSON* json4 = new tlib::CJSON("bool_key", true);
	
	tlib::CJSON* json5 = new tlib::CJSON("name", "tyao12333");
	tlib::CJSON* json6 = new tlib::CJSON("age", 25);
	tlib::CJSON* json7 = new tlib::CJSON("info", json5);

	json1->append(json2);
	json2->append(json3);
	json3->append(json4);
	json4->append(json7);
	json5->append(json6);

	tlib::StringList* stringList = new tlib::StringList();
	tlib::StringNode* stringNode1 = new tlib::StringNode("tony");
	tlib::StringNode* stringNode2 = new tlib::StringNode("sam");
	tlib::StringNode* stringNode3 = new tlib::StringNode("tim");
	stringList->push_back(stringNode1);
	stringList->push_back(stringNode2);
	stringList->push_back(stringNode3);
	tlib::CJSON* json11 = new tlib::CJSON("name_list", stringList);
	json6->append(json11);
	char* result = json1->to_string();
	std::cout << result << std::endl;

	tlib::CJSON* parse_result = json1->parse_string(result);
	parse_result->print_json();

	return 0;
}