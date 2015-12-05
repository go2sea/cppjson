#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <map>
using namespace std;

enum TYPE_ENUM {
	type_null ,
	type_string,
	type_number,
	type_bool,
	type_array,
	type_object,
};

int skip32(string& s, int index);

class cppjsonValue {
	friend class cppjsonString;
public:
	virtual void testPrint(int depth = 0) = 0;
	virtual string toString(int depth = 0) = 0;
	virtual TYPE_ENUM getType() = 0;
	/***getvalue***/
	//由于已重载，错误类型的调用会调到父类的方法，所以直接异常
	virtual bool getValue(string& s) { return false;  /*TODO:异常处理*/ };		//string类型重载
	virtual bool getValue(double& d) { return false; /*TODO:异常处理*/ };		//number类型重载
	virtual bool getValue(bool& b) { return false; /*TODO:异常处理*/ };			//bool类型重载
	virtual bool getValue(string key, cppjsonValue*& value) { return false; /*TODO:异常处理*/ };	//object类型重载
	virtual bool getValue(int index, cppjsonValue*& value) { return false; /*TODO:异常处理*/ };	//array类型重载
	/***setValue***/
	virtual bool setValue(double d) { return false; };		//number类型重载
	virtual bool setValue(string s) { return false; };		//string类型重载
	virtual bool setValue(bool b) { return false; };		//bool类型重载
	virtual bool setValue(string key, cppjsonValue* value) { return false; };	//object类型重载
	virtual bool setValue(int index, cppjsonValue* value) { return false; };	//array类型重载
	/***append***/
	virtual bool append(string key, cppjsonValue* value) { return false; };	//object类型重载
	virtual bool append(cppjsonValue* value) { return false; };				//array类型重载

	void fo() {}
	string t;
protected:
	string value;
};

class cppjsonObject : public cppjsonValue {
public:
	cppjsonObject() {};
	virtual void testPrint(int depth = 0);
	virtual string toString(int depth = 0);
	virtual TYPE_ENUM getType() { return type; }
	virtual bool setValue(string key, cppjsonValue* value);
	virtual bool append(string key, cppjsonValue* value);
	virtual bool getValue(string key, cppjsonValue*& value);
	/***非重载函数***/
	unordered_map<string, cppjsonValue*> getObject() { return dic; }

private:
	TYPE_ENUM type = type_object;
	unordered_map<string, cppjsonValue*> dic;
};

class cppjsonArray : public cppjsonValue {
public:
	cppjsonArray(){};
	virtual void testPrint(int depth = 0);
	virtual string toString(int depth = 0);
	TYPE_ENUM getType() { return type; }
	virtual bool setValue(int index, cppjsonValue* value);
	virtual bool append(cppjsonValue* value);
	virtual bool getValue(int index, cppjsonValue*& value);
	/***非重载函数***/
	vector<cppjsonValue*> getArray() { return list; }
private:
	TYPE_ENUM type = type_array;
	vector<cppjsonValue*> list;
};

class cppjsonString : public cppjsonValue {
public:
	cppjsonString(){};
	cppjsonString(string value_string) { value = value_string; }
	virtual void testPrint(int depth = 0);
	virtual string toString(int depth = 0);
	virtual bool setValue(string s);
	TYPE_ENUM getType() { cout << "type_string" << endl; return type; }
	virtual bool getValue(string& s);
	/***非重载函数***/
	string getString() { return value; }
	string t = "t";
private:
	TYPE_ENUM type = type_string;
	string value;
};

class cppjsonNumber : public cppjsonValue {
public:
	cppjsonNumber(){};
	cppjsonNumber(double d) { value = d; }
	virtual void testPrint(int depth = 0);
	virtual string toString(int depth = 0);
	virtual bool setValue(double d);
	TYPE_ENUM getType() { return type; }
	virtual bool getValue(double& d);
	/***非重载函数***/
	double getNumber() { return value; }
private:
	TYPE_ENUM type = type_number;
	double value;
};

class cppjsonBool : public cppjsonValue {
public:
	cppjsonBool() {};
	cppjsonBool(bool b) { value = b; }
	virtual void testPrint(int depth = 0);
	virtual string toString(int depth = 0);
	virtual bool setValue(bool b);
	TYPE_ENUM getType() { return type; }
	virtual bool getValue(bool& b);
	/***非重载函数***/
	bool getBool() { return value; }
private:
	TYPE_ENUM type = type_bool;
	bool value;
};

class cppjsonNull : public cppjsonValue {
public:
	cppjsonNull(){};
	virtual void testPrint(int depth = 0);
	virtual string toString(int depth = 0);
	TYPE_ENUM getType() { return type; }
private:
	TYPE_ENUM type = type_null;
};

class cppjson {
public:
	cppjson(){};
	cppjsonValue* parse(string& s, int& index);
	cppjsonObject* parseObject(string& s, int& index);
	cppjsonArray* parseArray(string& s, int& index);
	cppjsonString* parseString(string& s, int& index);
	cppjsonNumber* parseNumber(string& s, int& index);
	cppjsonBool* parseBool(string& s, int& index);
	cppjsonNull* parseNull(string& s, int& index);
	void print(cppjsonValue* v) { v->testPrint(0); }
	//或者void print(cppjsonValue* v) { cout << v->toString(0); }
	string getStr(cppjsonValue* v) { return v->toString(0); }
private:
};