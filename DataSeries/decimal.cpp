#include <ostream>
#include "decimal.h"


Decimal::Decimal() : _value{ 0.0 }, _isNaN{ true } {}
Decimal::Decimal(const double value) : _value{ value }, _isNaN{ false } {}
Decimal::Decimal(const Decimal& other) : _value{ other._value }, _isNaN{ other._isNaN } {}

Decimal::Decimal(Decimal&& other) : _value{ 0.0 }, _isNaN{ true } {
	std::swap(_value, other._value);
	std::swap(_isNaN, other._isNaN);
}

Decimal Decimal::operator=(const Decimal& other) {
	if (this == &other)
		return *this;

	_value = other._value;
	_isNaN = other._isNaN;

	return *this;
}

Decimal Decimal::operator=(Decimal&& other) {
	std::swap(_value, other._value);
	std::swap(_isNaN, other._isNaN);

	return *this;
}

Decimal Decimal::operator+(const double value) {
	return this->_value + value;
}

Decimal Decimal::operator+(const Decimal& other) {
	return this->_value + other._value;
}

Decimal Decimal::operator-(const double value) {
	return this->_value - value;
}

Decimal Decimal::operator-(const Decimal& other) {
	return this->_value - other._value;
}

Decimal Decimal::operator*(const double value) {
	return this->_value * value;
}

Decimal Decimal::operator*(const Decimal& other) {
	return this->_value * other._value;
}

Decimal Decimal::operator/(const double value) {
	return this->_value / value;
}

Decimal Decimal::operator/(const Decimal& other) {
	return this->_value / other._value;
}

//////////////
void Decimal::operator+=(const double value) {
	this->_value += value;
}

void Decimal::operator+=(const Decimal& other) {
	this->_value += other._value;
}

void Decimal::operator-=(const double value) {
	this->_value -= value;
}

void Decimal::operator-=(const Decimal& other) {
	this->_value -= other._value;
}

void Decimal::operator*=(const double value) {
	this->_value *= value;
}

void Decimal::operator*=(const Decimal& other) {
	this->_value *= other._value;
}

void Decimal::operator/=(const double value) {
	this->_value /= value;
}

void Decimal::operator/=(const Decimal& other) {
	this->_value /= other._value;
}


std::ostream& operator<<(std::ostream& os, const Decimal& Decimal) {
	if (Decimal._isNaN) {
		os << "NaN";
		return os;
	}

	os << Decimal._value;
	return os;
}

Decimal::~Decimal() {}