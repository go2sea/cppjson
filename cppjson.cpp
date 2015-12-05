#include "cppjson.h"

int skip32(string& s, int index) {
	//���ڽ�����ͣס
	while (s[index] && s[index] <= 32) {
		cout << 11 << endl;
		index++;
	}
	return index;
}




cppjsonString* cppjson::parseString(string& s, int& index) {
	cout << "parseString!" << endl;

	//skip32Ҫ���ϴβ�����ĩβ��ɣ�����
	if (s[index] != '\"') {
		cout << "parseString ��ʽ����" << endl;
		return NULL;
	}
	int esc_count = 0;//ת���ַ�����
	index = skip32(s, ++index);
	string temp_value;
	while (!(esc_count == 0 && s[index] == '\"')) {
		temp_value.push_back(s[index]);
		//�޸�ת���ַ�����
		if (s[index] == '\\')
			esc_count = (esc_count + 1) % 2;
		else
			esc_count = 0;
		index++;
	}
	index = skip32(s, index + 1);//Խ�����ַ���
	return new cppjsonString(temp_value);
}

cppjsonObject* cppjson::parseObject(string& s, int& index) {
	cout << "parseObject!" << endl;

	//skip32Ҫ���ϴβ�����ĩβ��ɣ�����
	if (s[index] != '{') {
		cout << "parseObject ��ͷ ��ʽ����" << endl;
		return NULL;
	}
	index = skip32(s, ++index);
	bool at_the_begin = true;//���ڵ�һ�ν���whileѭ��
	cppjsonObject* result = new cppjsonObject();
	while (at_the_begin || s[index] != '}') {//������'}'�ͼ��п�����','
		cout << "һ��" << endl;
		cout << "s[index]:" << s[index] << endl;
		if (s[index] == '}')//��ͷ��
			return result;
		if (!at_the_begin && s[index] != ',') {	//�ǵ�һ�θ�ʽ����
			cout << "parseObject while��ͷ ��ʽ����" << endl;
			return NULL;
		}
		if (at_the_begin)
			at_the_begin = false;
		else
			//��һ��ʱindex�Ѿ�ָ����˫������
			index = skip32(s, ++index);
		/* key���� */
		cppjsonString* key = parseString(s, index);
		/* ð�Ų��� */
		if (s[index] != ':') {
			cout << "parseObject ð�� ��ʽ����" << endl;
			return NULL;
		}
		else
			index = skip32(s, index + 1);
		/* value���� */
		cppjsonValue* value = parse(s, index);
		//��ӵ�dic��
		result->append(key->getString(), value);
	}
	index = skip32(s, index + 1);
	return result;
}

cppjsonArray* cppjson::parseArray(string& s, int& index) {
	cout << "parseArray!" << endl;
	//skip32Ҫ���ϴβ�����ĩβ��ɣ�����
	if (s[index] != '[') {
		cout << "parseArray ��ͷ ��ʽ����" << endl;
		return NULL;
	}
	index = skip32(s, ++index);
	bool at_the_begin = true;//���ڵ�һ�ν���whileѭ��
	cppjsonArray* result = new cppjsonArray();
	while (at_the_begin || s[index] != ']') {//������'}'�ͼ��п�����','
		cout << "array һ��" << endl;
		cout << "s[index]:" << s[index] << endl;
		if (s[index] == ']')//��ͷ��
			return result;
		if (!at_the_begin && s[index] != ',') {	//�ǵ�һ�θ�ʽ���� 
			cout << "parseArray while��ͷ ��ʽ����" << endl;
			return NULL;
		}
		if (at_the_begin)
			at_the_begin = false;
		else
			//��һ��ʱindex�Ѿ�ָ���һ��Ԫ�صĵ�һ���ַ���
			index = skip32(s, ++index);
		cppjsonValue* value = parse(s, index);
		//��ӵ�dic��
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
	//bool�ĸ�ʽ������ⲿ
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
	//��һ��parse�Ŀ�ʼҪ��skip32
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


/********************testPrintϵ��******************/
//print����ԭ��object�������⣬�л��д���������������

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
	//����
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
		if (!(i == list[list.size() - 1]))//�������һ��Ԫ��
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
		if (!(i == list[list.size() - 1]))//�������һ��Ԫ��
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
	//����
	for (int i = 0; i < depth; i++)
		result.append("\t");
	result.append("}");
	return result;
}

/**************setValueϵ��***************/
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
		cout << "cppjsonArray::setValue Խ��!" << endl;
		return false;
	}
	//TODO:�쳣����
	//TODO:ԭ��λ��Ԫ�ص��ͷ�
	if (index < (int)list.size())
		list[index] = value;
	else if (index == (int)list.size())
		list.push_back(value);
	return true;
}

/***************appendϵ��****************/
bool cppjsonObject::append(string key, cppjsonValue* value) {
	if (dic.find(key) != dic.end()) {
		cout << "cppjsonObject::append key�Ѵ��ڣ�" << endl;
		return false;
	}
	//TODO:�쳣����
	setValue(key, value);
	return true;
}

bool cppjsonArray::append(cppjsonValue* value)  {
	setValue((int)list.size(), value);
	return true;
}


/***************getValueϵ��******************/
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
		cout << "cppjsonObject::getValue δ�ҵ�!" << endl;
		return false;
	}
	value = dic[key];
	return true;
}

bool cppjsonArray::getValue(int index, cppjsonValue*& value) {
	if (index >= (int)list.size()) {
		cout << "cppjsonArray::getValue Խ��!" << endl;
		return false;
	}
	value = list[index];
	return true;
}