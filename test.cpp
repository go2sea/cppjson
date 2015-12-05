#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "cppjson.h"
using namespace std;

void main() {
	/*
		parse°¢print°¢toString≤‚ ‘
	*/
	if (0){
		cppjsonValue* i = new cppjsonString("test value");
		string test_content = "{ \"k1\":\"v1\", \"k2\":{\"k3\":1.88, \"k4\":false, \"k5\":null }, \"k6\":[[1,null],{\"sss\":[4,5]}]}";
		cout << test_content << endl;
		cppjson c;
		int index = 0;
		cppjsonValue* result = c.parse(test_content, index);
		c.print(result);
		cout << endl << endl;
		cout << c.getStr(result) << endl;
		string again = c.getStr(result);
		index = 0;
		c.print(c.parse(again, index));
	}
	/*
		setValue≤‚ ‘
	*/
	if (0) {
		string k1 = "k1";
		cppjsonValue* v1 = new cppjsonString();
		v1->setValue("v1");
		string k2 = "k2";
		cppjsonValue* v2 = new cppjsonNumber();
		v2->setValue(4.4);
		string k3 = "k3";
		cppjsonValue* v3 = new cppjsonBool();
		v3->setValue(false);
		string k4 = "k4";
		cppjsonValue* v4 = new cppjsonArray();
		v4->setValue(0, new cppjsonString("v4[0]"));
		v4->setValue(1, new cppjsonString("v4[1]"));
		string k5 = "k5";
		cppjsonValue* v5 = new cppjsonObject();
		v5->setValue("v5k1", new cppjsonString("v5v1"));
		v5->setValue("v5k2", new cppjsonString("v5v2"));

		cppjsonObject result2;
		result2.setValue(k1, v1);
		result2.setValue(k2, v2);
		result2.setValue(k3, v3);
		result2.setValue(k4, v4);
		result2.setValue(k5, v5);
		cout << result2.toString() << endl;
	}
	/*
		getType≤‚ ‘
	*/
	if (0) {
		cppjsonValue* v0 = new cppjsonNull();		cout << v0->getType() << endl;
		cppjsonValue* v1 = new cppjsonString();		cout << v1->getType() << endl;
		cppjsonValue* v2 = new cppjsonNumber();		cout << v2->getType() << endl;
		cppjsonValue* v3 = new cppjsonBool();		cout << v3->getType() << endl;
		cppjsonValue* v4 = new cppjsonArray();		cout << v4->getType() << endl;
		cppjsonValue* v5 = new cppjsonObject();		cout << v5->getType() << endl;
	}
	/*
		append≤‚ ‘
	*/
	if (0) {
		cppjsonValue* result = new cppjsonObject();
		result->append("k1", new cppjsonString("v1"));
		result->append("k2", new cppjsonString("v2"));
		cout << result->toString() << endl;
		cppjsonValue* result1 = new cppjsonArray();
		result1->append(new cppjsonString("v1"));
		result1->append(new cppjsonString("v2"));
		cout << result1->toString() << endl;
		cppjsonString test;
	}
	/*
		getValue≤‚ ‘
	*/
	if(1) {
		cppjsonValue* v1 = new cppjsonString("v1");
		cppjsonValue* v2 = new cppjsonNumber(5.5);
		cppjsonValue* v3 = new cppjsonBool(true);
		cppjsonValue* v4 = new cppjsonArray();
		v4->append(v1);
		cppjsonValue* v5 = new cppjsonObject();
		v5->append("k1", v2);
		string r1;
		v1->getValue(r1);
		cout << r1 << endl;
		double r2;
		v2->getValue(r2);
		cout << r2 << endl;
		bool r3;
		v3->getValue(r3);
		cout << r3 << endl;

		cppjsonValue* r4;
		v4->getValue(0, r4);
		cout << r4->toString() << endl;

		cppjsonValue* r5;
		v5->getValue("k1", r5);
		cout << r5->toString() << endl;

	}


	//cout << endl << endl << endl;
	//cppjsonString v("v");
	//cppjsonValue &v1 = v;
	//cppjsonString v2("v2");
	//v1 = v2;
	//cout << v2.toString() << endl;
	//cout << v1.toString() << endl;
	//cout << v.toString() << endl;
	//string temps = "change";
	//cout<< v1.setValue(temps)<<endl;
	//cout << v2.toString() << endl;
	//cout << v1.toString() << endl;
	//cout << v.toString() << endl;


	system("pause");
}