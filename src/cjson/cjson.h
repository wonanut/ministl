/*
 Copyright (c) 2020-2020 Howard Wonanut

 This is the demo version of Json parser, which refer to the wonderful project
 cJSON (Github: https://github.com/DaveGamble/cJSON.git). This project is just
 a toy since there still have many problems that are not fixed, for example,
 the memory release function was not complished so far, some potential bugs. 
 I will update this project quickly to make it available and safe:)

 BE CAUTIOUS IF YOU TRY TO USE THE UNSTABLE VERSION OF CODE.
*/

#pragma once
#pragma warning(disable: 4996)

#ifndef C_JSON
#define C_JSON

#include <unordered_map>
#include <vector>

namespace tlib
{

#define CJSON_MAX_STRING_LEN (20480)
#define CJSON_MAX_KEY_LEN	(128)
#define CJSON_MAX_VAL_LEN	(2048)
#define CJSON_VERSION_MAJOR	(0)
#define CJSON_VERSION_MINOR	(1)

#define CJSON_Invalid	(0)
#define CJSON_Bool		(1 << 0)
#define CJSON_Null		(1 << 1)
#define CJSON_Int		(1 << 2)
#define CJSON_Double	(1 << 3)
#define CJSON_String	(1 << 4)
#define CJSON_Array		(1 << 5)
#define CJSON_Object	(1 << 6)

class StringNode {
public:
	StringNode();
	StringNode(const char* const string_value);

	~StringNode();
	StringNode* get_next();
	StringNode* get_prev();
	void set_next(StringNode* next);
	void set_prev(StringNode* prev);
	const char* const get_value();

private:
	char* value;
	StringNode* next;
	StringNode* prev;
};

class StringList {
public:
	StringList();
	~StringList();

	void push_back(StringNode* node);
	void push_front(StringNode* node);
	void pop_back();
	void insert(StringNode* node, int pos);
	void erase(int pos);
	int size();
	StringNode* get_head();
	StringNode* get_tail();

private:
	int length;
	StringNode* head;
	StringNode* tail;
};


class CJSON {
public:
	using uint16 = unsigned short;

	CJSON();
	CJSON(const char* const key, bool val);
	CJSON(const char* const key, int val);
	CJSON(const char* const key, double val);
	CJSON(const char* const key, const char* const val);
	CJSON(const char* const key, CJSON& val);
	CJSON(const char* const key, CJSON* val);
	CJSON(const char* const key, StringList* val);

	~CJSON();

	char* CJSON_version();

	void append(CJSON& item);
	void append(CJSON* item);
	void print_json();
	void print_json_pretty();
	void delete_json();

	CJSON* parse_string(const char* const json_str);
	char* to_string();

private:
	void __analyse_key(char* json_str, int& end_pos, const char* const key);
	void __analyse_val(char* json_str, int& end_pos, int value);
	void __analyse_val(char* json_str, int& end_pos, double value);
	void __analyse_val(char* json_str, int& end_pos, bool value);
	void __analyse_val(char* json_str, int& end_pos, const char* const value);
	void __analyse_val(char* json_str, int& end_pos, CJSON* value);
	void __analyse_val(char* json_str, int& end_pos, StringList* value);
	void __analyse_val_error(char* json_str, int& end_pos);
		
	void __set_key_name(const char* const key);

	char* __to_string_basic();
	CJSON* __parse_string_basic(const char* const json_str, int& cur_pos);

	char* key_name;

	uint16 value_type;
	int int_val;
	double double_val;
	char* string_val;
	bool bool_val;
	StringList* array_val;

	CJSON* child;
	CJSON* next;
	CJSON* prev;
};

} // namespace tlib

#endif
