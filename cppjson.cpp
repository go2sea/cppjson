#include "cppjson.h"

int skip32(string& s, int index) {
	//会在结束符停住
	while (s[index] && s[index] <= 32) {
		cout << 11 << endl;
		index++;
	}
	return index;
}




cppjsonString* cppjson::parseString(string& s, int& index) {
	cout << "parseString!" << endl;

	//skip32要在上次操作的末尾完成！！！
	if (s[index] != '\"') {
		cout << "parseString 格式错误" << endl;
		return NULL;
	}
	int esc_count = 0;//转义字符计数
	index = skip32(s, ++index);
	string temp_value;
	while (!(esc_count == 0 && s[index] == '\"')) {
		temp_value.push_back(s[index]);
		//修改转义字符计数
		if (s[index] == '\\')
			esc_count = (esc_count + 1) % 2;
		else
			esc_count = 0;
		index++;
	}
	index = skip32(s, index + 1);//越过该字符串
	return new cppjsonString(temp_value);
}

cppjsonObject* cppjson::parseObject(string& s, int& index) {
	cout << "parseObject!" << endl;

	//skip32要在上次操作的末尾完成！！！
	if (s[index] != '{') {
		cout << "parseObject 开头 格式错误" << endl;
		return NULL;
	}
	index = skip32(s, ++index);
	bool at_the_begin = true;//用于第一次进入while循环
	cppjsonObject* result = new cppjsonObject();
	while (at_the_begin || s[index] != '}') {//不等于'}'就极有可能是','
		cout << "一次" << endl;
		cout << "s[index]:" << s[index] << endl;
		if (s[index] == '}')//到头了
			return result;
		if (!at_the_begin && s[index] != ',') {	//非第一次格式错误
			cout << "parseObject while开头 格式错误" << endl;
			return NULL;
		}
		if (at_the_begin)
			at_the_begin = false;
		else
			//第一次时index已经指向左双引号了
			index = skip32(s, ++index);
		/* key部分 */
		cppjsonString* key = parseString(s, index);
		/* 冒号部分 */
		if (s[index] != ':') {
			cout << "parseObject 冒号 格式错误" << endl;
			return NULL;
		}
		else
			index = skip32(s, index + 1);
		/* value部分 */
		cppjsonValue* value = parse(s, index);
		//添加到dic中
		result->append(key->getString(), value);
	}
	index = skip32(s, index + 1);
	return result;
}

cppjsonArray* cppjson::parseArray(string& s, int& index) {
	cout << "parseArray!" << endl;
	//skip32要在上次操作的末尾完成！！！
	if (s[index] != '[') {
		cout << "parseArray 开头 格式错误" << endl;
		return NULL;
	}
	index = skip32(s, ++index);
	bool at_the_begin = true;//用于第一次进入while循环
	cppjsonArray* result = new cppjsonArray();
	while (at_the_begin || s[index] != ']') {//不等于'}'就极有可能是','
		cout << "array 一次" << endl;
		cout << "s[index]:" << s[index] << endl;
		if (s[index] == ']')//到头了
			return result;
		if (!at_the_begin && s[index] != ',') {	//非第一次格式错误 
			cout << "parseArray while开头 格式错误" << endl;
			return NULL;
		}
		if (at_the_begin)
			at_the_begin = false;
		else
			//第一次时index已经指向第一个元素的第一个字符了
			index = skip32(s, ++index);
		cppjsonValue* value = parse(s, index);
		//添加到dic中
		result->append(value);
	}
	index = skip32(s, index + 1);
	return result;
}



cppjsonNumber* cppjson::parseNumber(string& s, int& index) {
	cout << "parseNumber!" << endl;
	int begin = index;
	while (s[index] == '-' || s[index] == '.' || (s[index] >= '0' && s[index] <= '9'))
		index++;
	string value_str = s.substr(begin, index);
	double value = atof(value_str.c_str());
	cppjsonNumber* result = new cppjsonNumber(value);
	index = skip32(s, index);
	return result;
}

cppjsonBool* cppjson::parseBool(string& s, int& index)	{
	cout << "parseBool!" << endl;
	bool value;
	if (s.substr(index, 5) == "False" || s.substr(index, 5) == "false") {
		value = false;
		index = skip32(s, index + 5);
	}
	else if (s.substr(index, 4) == "true" || s.substr(index, 4) == "True") {
		value = true;
		index = skip32(s, index + 4);
	}
	//bool的格式检查在外部
	cppjsonBool* result = new cppjsonBool(value);
	return result;
}

cppjsonNull* cppjson::parseNull(string& s, int& index) {
	cout << "parseNull!" << endl;
	index = skip32(s, index + 4);
	cppjsonNull* result = new cppjsonNull();
	return result;
}

cppjsonValue* cppjson::parse(string& s, int& index) {
	cout << "parse!" << endl;
	//第一次parse的开始要做skip32
	index = skip32(s, index);
	cout << "s[index] in parse:" << s[index] << endl;
	if (s[index] == '{')
		return parseObject(s, index);
	if (s[index] == '[')
		return parseArray(s, index);
	if (s[index] == '\"')
		return parseString(s, index);
	if (s[index] == '-' || (s[index] >= '0' && s[index] <= '9'))
		return parseNumber(s, index);
	if (s.substr(index, 5) == "False" || s.substr(index, 5) == "false"
		|| s.substr(index, 4) == "true" || s.substr(index, 4) == "True"
		)
		return parseBool(s, index);
	if (s.substr(index, 4) == "null")
		return parseNull(s, index);
	return NULL;
}


/********************testPrint系列******************/
//print换行原则：object类型特殊，有换行处理，其他都不换行

void cppjsonObject::testPrint(int depth)	{
	cout << "{" << endl;
	for (auto i : dic) {
		for (int j = 0; j < depth + 1; j++)
			cout << "\t";
		cout << "\"" + i.first + "\"" + ":";
		i.second->testPrint(depth + 1);
		if (i != (*--dic.end()))
			cout << ","<< endl;
	}
	cout << endl;
	//缩进
	for (int i = 0; i < depth; i++)
		cout << "\t";
	cout << "}";
}


void cppjsonString::testPrint(int depth)	{
	cout << "\"" + value + "\"";
}

void cppjsonNumber::testPrint(int depth)	{
	cout << value;
}

void cppjsonBool::testPrint(int depth) {
	if (value)
		cout << "true";
	else
		cout << "false";
	return;
}

void cppjsonNull::testPrint(int depth) {
	cout << "null";
}

void cppjsonArray::testPrint(int depth) {;
	cout << "[ ";
	for (auto i : list) {
		i->testPrint(depth + 1);
		if (!(i == list[list.size() - 1]))//不是最后一个元素
			cout << ", ";
	}
	cout << "]";
}

string cppjsonString::toString(int depth) {
	return  "\"" + value + "\"";
}

string cppjsonNumber::toString(int depth) {
	stringstream ss;
	ss << value;
	return ss.str();
}

string cppjsonBool::toString(int depth) {
	if (value)
		return "true";
	return "false";
}

string cppjsonNull::toString(int depth) {
	return "null";
}

string cppjsonArray::toString(int depth) {
	string result;
	result.append("[ ");
	for (auto i : list) {
		result.append(i->toString(depth + 1));
		if (!(i == list[list.size() - 1]))//不是最后一个元素
			result.append(", ");
	}
	result.append("]");
	return result;
}

string cppjsonObject::toString(int depth) {
	string result;
	result.append("{\n");
	for (auto i : dic) {
		for (int j = 0; j < depth + 1; j++)
			result.append("\t");
		result.append("\"" + i.first +  "\"" + ":");
		result.append(i.second->toString(depth + 1));
		if (i != (*--dic.end()))
			result.append(",\n");
	}
	result.append("\n");
	//缩进
	for (int i = 0; i < depth; i++)
		result.append("\t");
	result.append("}");
	return result;
}

/**************setValue系列***************/
bool cppjsonNumber::setValue(double d) {
	value = d;
	return true;
}

bool cppjsonString::setValue(string s) {
	value = s;
	return true;
}

bool cppjsonBool::setValue(bool b) {
	value = b;
	return true;
}

bool cppjsonObject::setValue(string key, cppjsonValue* value) {
	dic[key] = value;
	return true;
}

bool cppjsonArray::setValue(int index, cppjsonValue* value) {
	if (index > (int)list.size()) {
		cout << "cppjsonArray::setValue 越界!" << endl;
		return false;
	}
	//TODO:异常处理
	//TODO:原先位置元素的释放
	if (index < (int)list.size())
		list[index] = value;
	else if (index == (int)list.size())
		list.push_back(value);
	return true;
}

/***************append系列****************/
bool cppjsonObject::append(string key, cppjsonValue* value) {
	if (dic.find(key) != dic.end()) {
		cout << "cppjsonObject::append key已存在！" << endl;
		return false;
	}
	//TODO:异常处理
	setValue(key, value);
	return true;
}

bool cppjsonArray::append(cppjsonValue* value)  {
	setValue((int)list.size(), value);
	return true;
}


/***************getValue系列******************/
bool cppjsonString::getValue(string& s) {
	s = value;
	return true;
}

bool cppjsonNumber::getValue(double& d) {
	d = value;
	return true;
}

bool cppjsonBool::getValue(bool& b) {
	b = value;
	return true;
}

bool cppjsonObject::getValue(string key, cppjsonValue*& value) {
	if (dic.find(key) == dic.end()) {
		cout << "cppjsonObject::getValue 未找到!" << endl;
		return false;
	}
	value = dic[key];
	return true;
}

bool cppjsonArray::getValue(int index, cppjsonValue*& value) {
	if (index >= (int)list.size()) {
		cout << "cppjsonArray::getValue 越界!" << endl;
		return false;
	}
	value = list[index];
	return true;
}