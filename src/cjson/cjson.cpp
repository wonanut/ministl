#include "cjson.h"
#include <string.h>
#include <iostream>

/* StringNode */
tlib::StringNode::StringNode() : value(nullptr), next(nullptr) {}

tlib::StringNode::StringNode(const char* const value) : next(nullptr) {
	this->value = (char*)std::malloc((strlen(value) + 1) * sizeof(char));
	if (this->value == nullptr) {
		perror("error while malloc memory for key, exiting.\n");
		exit(-1);
	}
	std::strcpy(this->value, value);
}

tlib::StringNode::~StringNode() {
	delete this->value;
}

tlib::StringNode* tlib::StringNode::get_next() {
	if (this == nullptr) {
		return nullptr;
	}
	return this->next;
}

tlib::StringNode* tlib::StringNode::get_prev() {
	if (this == nullptr) {
		return nullptr;
	}
	return this->prev;
}

const char* const tlib::StringNode::get_value() {
	return this->value;
}

void tlib::StringNode::set_next(tlib::StringNode* next) {
	this->next = next;
}

void tlib::StringNode::set_prev(tlib::StringNode* prev) {
	this->prev = prev;
}
/* end StringNode */


/* StringList */
tlib::StringList::StringList() : length(0) {
	this->head = new StringNode("head");
	this->tail = new StringNode("tail");
	this->head->set_next(this->tail);
	this->tail->set_prev(this->head);
}

int tlib::StringList::size() {
	return this->length;
}

tlib::StringList::~StringList() {
	tlib::StringNode* iter = this->head;
	while (iter) {
		StringNode* tmp = iter->get_next();
		free(iter);
		iter = tmp;
	}
}

void tlib::StringList::pop_back() {
	if (this->length == 0) {
		return;
	}

	tlib::StringNode* tmp = this->tail->get_prev()->get_prev();
	free(this->tail->get_prev());
	tmp->set_next(this->tail);
	this->tail->set_prev(tmp);
	this->length -= 1;
}

void tlib::StringList::push_back(tlib::StringNode* node) {
	tlib::StringNode* tmp = this->tail->get_prev();
	tmp->set_next(node);
	node->set_next(this->tail);
	this->tail->set_prev(node);
	node->set_prev(tmp);
	this->length += 1;
}

tlib::StringNode* tlib::StringList::get_head() {
	return this->head;
}

tlib::StringNode* tlib::StringList::get_tail() {
	return this->tail;
}

/* end StringList */

tlib::CJSON::CJSON () : child(nullptr), next(nullptr), prev(nullptr), value_type(CJSON_Invalid) {
	__set_key_name("error");
}

tlib::CJSON::CJSON(const char* const key, bool val) : bool_val(val), value_type(CJSON_Bool), child(nullptr), next(nullptr), prev(nullptr) {
	__set_key_name(key);
}

tlib::CJSON::CJSON(const char* const key, int val) : int_val(val), value_type(CJSON_Int), child(nullptr), next(nullptr), prev(nullptr) {
	__set_key_name(key);
}

tlib::CJSON::CJSON(const char* const key, double val) : double_val(val), value_type(CJSON_Double), child(nullptr), next(nullptr), prev(nullptr) {
	__set_key_name(key);
}

tlib::CJSON::CJSON(const char* const key, const char* const val) : value_type(CJSON_String), child(nullptr), next(nullptr), prev(nullptr) {
	__set_key_name(key);

	this->string_val = (char*)std::malloc((strlen(key) + 1) * sizeof(char));
	if (this->string_val == nullptr) {
		perror("error while malloc memory for string_value, exiting.\n");
		exit(-1);
	}
	std::strcpy(this->string_val, val);
}

tlib::CJSON::CJSON(const char* const key, tlib::CJSON& val) : value_type(CJSON_Object), child(nullptr), next(nullptr), prev(nullptr) {
	__set_key_name(key);

	if (this->child == nullptr and &val != nullptr) {
		this->child = &val;
	}
}

tlib::CJSON::CJSON(const char* const key, tlib::CJSON* val) : value_type(CJSON_Object), child(nullptr), next(nullptr), prev(nullptr) {
	__set_key_name(key);

	if (this->child == nullptr and val != nullptr) {
		this->child = val;
	}
}

tlib::CJSON::CJSON(const char* const key, tlib::StringList* val) : array_val(val), value_type(CJSON_Array), child(nullptr), next(nullptr), prev(nullptr) {
	__set_key_name(key);
}

void tlib::CJSON::__set_key_name(const char* const key) {
	this->key_name = (char*)std::malloc((strlen(key) + 1) * sizeof(char));
	if (this->key_name == nullptr) {
		perror("error while malloc memory for key, exiting.\n");
		exit(-1);
	}
	std::strcpy(this->key_name, key);
}

/* monkey */
tlib::CJSON::~CJSON() {
	
}

void tlib::CJSON::append(CJSON& item) {
	if (&item != nullptr && this->next == nullptr) {
		this->next = &item;
		item.prev = this;
	}
}

void tlib::CJSON::append(CJSON* item) {
	if (item != nullptr && this->next == nullptr) {
		this->next = item;
		item->prev = this;
	}
}

void tlib::CJSON::__analyse_key(char* json_str, int& end_pos, const char* const key) {
	unsigned int key_len = std::strlen(key);
	json_str[end_pos++] = '\"';
	for (unsigned int i = 0; i < key_len; i++) {
		json_str[end_pos++] = key[i];
	}
	json_str[end_pos++] = '\"';
}

void tlib::CJSON::__analyse_val(char* json_str, int& end_pos, int value) {
	if (value == 0) {
		json_str[end_pos++] = '0';
		return;
	}
	
	if (value < 0) {
		json_str[end_pos++] = '-';
		value = -value;
	}

	char buf[20] = { 0 };
	int idx = 0;

	while (value != 0) {
		buf[idx++] = '0' + (value % 10);
		value /= 10;
	}
	buf[idx] = '\0';

	for (int i = std::strlen(buf) - 1; i >= 0; i--) {
		json_str[end_pos++] = buf[i];
	}
}

void tlib::CJSON::__analyse_val(char* json_str, int& end_pos, double value) {
	if (value > -1e-3 && value < 1e-3) {
		json_str[end_pos++] = '0';
		return;
	}

	if (value < 0) {
		json_str[end_pos++] = '-';
		value = -value;
	}

	char double_buff[20];
	sprintf(double_buff, "%.6f", value);
	for (int i = 0; i < std::strlen(double_buff); i++) {
		json_str[end_pos++] = double_buff[i];
	}
}

void tlib::CJSON::__analyse_val(char* json_str, int& end_pos, bool value) {
	if (value == true) {
		json_str[end_pos++] = 't';
		json_str[end_pos++] = 'r';
		json_str[end_pos++] = 'u';
		json_str[end_pos++] = 'e';
	}
	else {
		json_str[end_pos++] = 'f';
		json_str[end_pos++] = 'a';
		json_str[end_pos++] = 'l';
		json_str[end_pos++] = 's';
		json_str[end_pos++] = 'e';
	}
}

void tlib::CJSON::__analyse_val(char* json_str, int& end_pos, const char* const value) {
	unsigned int value_len = std::strlen(value);
	for (unsigned int i = 0; i < value_len; i++) {
		json_str[end_pos++] = value[i];
	}
}

void tlib::CJSON::__analyse_val(char* json_str, int& end_pos, tlib::CJSON* value) {
	char* sub_string;
	sub_string = value->to_string();
	
	this->__analyse_val(json_str, end_pos, sub_string);
}

void tlib::CJSON::__analyse_val(char* json_str, int& end_pos, tlib::StringList* value) {
	int size = value->size();
	json_str[end_pos++] = '[';
	tlib::StringNode* iter = value->get_head()->get_next();
	for (int i = 0; i < size; i++) {
		json_str[end_pos++] = '\"';
		int str_size = std::strlen(iter->get_value());
		for (unsigned int j = 0; j < str_size; j++) {
			json_str[end_pos++] = iter->get_value()[j];
		}
		json_str[end_pos++] = '\"';
		json_str[end_pos++] = ',';
		
		iter = iter->get_next();
	}
	json_str[end_pos - 1] = ']';
}

void tlib::CJSON::__analyse_val_error(char* json_str, int& end_pos) {
	char err_str[5] = { 'e', 'r', 'r', 'o', 'r'};
	for (unsigned int i = 0; i < 5; i++) {
		json_str[end_pos++] = err_str[i];
	}
}

void tlib::CJSON::print_json() {
	std::cout << this->to_string() << std::endl;
}

char* tlib::CJSON::CJSON_version() {
	static char version_buff[10];
	sprintf(version_buff, "%i.%i", CJSON_VERSION_MAJOR, CJSON_VERSION_MINOR);
	return version_buff;
}

char* tlib::CJSON::__to_string_basic() {
	char json_string[CJSON_MAX_STRING_LEN];
	int end_pos = 0;

	tlib::CJSON* iter = this;
	json_string[end_pos++] = '{';
	while (iter) {
		__analyse_key(json_string, end_pos, iter->key_name);
		json_string[end_pos++] = ':';
		switch (iter->value_type) {
		case CJSON_Invalid:
			__analyse_val_error(json_string, end_pos);
			break;
		case CJSON_Bool:
			__analyse_val(json_string, end_pos, iter->bool_val);
			break;
		case CJSON_Int:
			__analyse_val(json_string, end_pos, iter->int_val);
			break;
		case CJSON_Double:
			__analyse_val(json_string, end_pos, iter->double_val);
			break;
		case CJSON_String:
			json_string[end_pos++] = '\"';
			__analyse_val(json_string, end_pos, iter->string_val);
			json_string[end_pos++] = '\"';
			break;
		case CJSON_Object:
			__analyse_val(json_string, end_pos, iter->child);
			break;
		case CJSON_Array:
			__analyse_val(json_string, end_pos, iter->array_val);
		}
		json_string[end_pos++] = ',';
		iter = iter->next;
	}
	json_string[end_pos - 1] = '}';
	json_string[end_pos] = '\0';
	return json_string;
}

char* tlib::CJSON::to_string() {
	return this->__to_string_basic();
}

tlib::CJSON* tlib::CJSON::parse_string(const char* const json_str) {
	int cur_pos = 0;
	return this->__parse_string_basic(json_str, cur_pos);
}

tlib::CJSON* tlib::CJSON::__parse_string_basic(const char* const json_str, int& cur_pos) {
	int str_len = std::strlen(json_str);
	int start_pos = cur_pos;
	tlib::CJSON* head = nullptr;
	tlib::CJSON* cur_node = nullptr;
	tlib::StringList* list_head = nullptr;

	int status = 0;	/* 1-2: parse key, 3-4: parse value*/
	char key_buff[CJSON_MAX_KEY_LEN] = { 0 };
	char val_buff[CJSON_MAX_VAL_LEN] = { 0 };
	int key_idx = 0;
	int val_idx = 0;
	int val_type = -1;
	int cur_int_val = 0;
	int cur_symbol = 1;
	double cur_double_val = 0.0;
	int cur_double_bias = 1;
	bool cur_bool_val = true;

	while (cur_pos < str_len) {
   		char cur_ch = json_str[cur_pos];
		if (cur_ch == '{') {
			if (cur_pos != start_pos) {
				tlib::CJSON* child = __parse_string_basic(json_str, cur_pos);
				tlib::CJSON* new_node = new tlib::CJSON(key_buff, child);
				if (cur_node == nullptr) {
					cur_node = new_node;
					head = cur_node;
				}
				else {
					cur_node->next = new_node;
					new_node->prev = cur_node;
					cur_node = new_node;
				}
			}
			else {
				cur_pos++;
			}
		}
		else if (cur_ch == '}') {
			if (val_type == CJSON_Bool || val_type == CJSON_Int || val_type == CJSON_Double) {
				val_buff[val_idx] = '\0';
				if (val_type == CJSON_Bool) {
					cur_bool_val = 1 - std::strncmp("true", val_buff, 4);
				}

				tlib::CJSON* new_node = nullptr;
				switch (val_type) {
				case CJSON_Int:
					new_node = new tlib::CJSON(key_buff, cur_int_val * cur_symbol); break;
				case CJSON_Bool:
					new_node = new tlib::CJSON(key_buff, cur_bool_val); break;
				case CJSON_Double:
					new_node = new tlib::CJSON(key_buff, (cur_double_val / cur_double_bias + cur_int_val) * cur_symbol); break;
				case CJSON_String:
					new_node = new tlib::CJSON(key_buff, val_buff); break;
				default:
					break;
				}
				if (cur_node == nullptr) {
					cur_node = new_node;
					head = cur_node;
				}
				else {
					cur_node->next = new_node;
					cur_node = new_node;
				}
				val_type = -1;
				val_type = -1;
				cur_int_val = 0;
				cur_double_val = 0;
				cur_bool_val = 0;
				cur_double_bias = 1;
				status = 0;
				val_idx = 0;
				key_idx = 0;
			}
			return head;
		}
		else if (cur_ch == ':') {
			key_idx = 0;
			cur_pos++;
		}
		else if (cur_ch == ',') {
			if (val_type == CJSON_Array) {
				status = 2;
				val_idx = 0;
			}
			else {
				tlib::CJSON* new_node = nullptr;
				val_buff[val_idx] = '\0';
				if (val_type == CJSON_Bool) {
					cur_bool_val = 1 - std::strncmp("true", val_buff, 4);
				}

				switch (val_type) {
				case CJSON_Int:
					new_node = new tlib::CJSON(key_buff, cur_int_val * cur_symbol); break;
				case CJSON_Bool:
					new_node = new tlib::CJSON(key_buff, cur_bool_val); break;
				case CJSON_Double:
					new_node = new tlib::CJSON(key_buff, (cur_double_val / cur_double_bias + cur_int_val) * cur_symbol); break;
				case CJSON_String:
					new_node = new tlib::CJSON(key_buff, val_buff); break;
				default:
					break;
				}
				if (cur_node == nullptr) {
					cur_node = new_node;
					head = cur_node;
				}
				else {
					cur_node->next = new_node;
					cur_node = new_node;
				}
				val_type = -1;
				cur_int_val = 0;
				cur_double_val = 0;
				cur_bool_val = 0;
				cur_double_bias = 1;
				status = 0;
				val_idx = 0;
				key_idx = 0;
			}
			
			cur_pos++;
		}
		else if (cur_ch == '-' && status == 3) {
			cur_symbol = -1;
			cur_pos++;
		}
		else if (val_type == CJSON_Int && cur_ch == '.') {
			val_type = CJSON_Double;
			cur_pos++;
		}
		else if (cur_ch == '\"') {
			if (status == 0) {
				status = (status + 1) % 4;
			}
			else if (status == 2 && val_type == CJSON_Array) {
				status = (status + 1) % 4;
			}
			else if (status == 2) {
				val_type = CJSON_String;
				status = (status + 1) % 4;
			}
			else if (status == 1) {
				key_buff[key_idx] = '\0';
				status = (status + 1) % 4;
			}
			else if (status == 3) {
				if (val_type == CJSON_Array) {
					val_buff[val_idx] = '\0';
					tlib::StringNode* new_node = new tlib::StringNode(val_buff);
					list_head->push_back(new_node);
					status = 2;
				}
			}
			
			cur_pos++;
		}
		else if ((val_type == -1 || val_type == CJSON_Int || val_type == CJSON_Double) && cur_ch >= '0' && cur_ch <= '9') {
			if (val_type == -1 || val_type == CJSON_Int) {
				val_type = CJSON_Int;
				cur_int_val = cur_int_val * 10 + (cur_ch - '0');
			}
			else {
				cur_double_val = cur_double_val * 10 + (cur_ch - '0');
				cur_double_bias *= 10;
			}
			cur_pos++;
		}
		else if (cur_ch == '[') {
			val_type = CJSON_Array;
			list_head = new tlib::StringList();
			cur_pos++;
		}
		else if (cur_ch == ']') {
			val_type = -1;
			tlib::CJSON* new_node = new tlib::CJSON(key_buff, list_head);
			if (cur_node == nullptr) {
				cur_node = new_node;
				head = cur_node;
			}
			else {
				cur_node->next = new_node;
				cur_node = new_node;
			}
			cur_pos++;
		}
		else {
			if (status == 1) {
				key_buff[key_idx++] = cur_ch;
			}
			else if (val_type == CJSON_String || val_type == CJSON_Array) {
				val_buff[val_idx++] = cur_ch;
			}
			else{
				val_type = CJSON_Bool;
				val_buff[val_idx++] = cur_ch;
			}
			cur_pos++;
		}
	}
	return head;
}