#pragma once

#include <iostream>

using namespace std;

class Coordinate
{
	pair <int, int> location;
	char value = '1';
public:
	char get_value() {
		return value;
	}
	void set_value(char i) {
		value = i;
	}
	pair <int, int> get_location() {
		return location;
	}
	void set_location(pair <int, int> p) {
		location = p;
	}
};
