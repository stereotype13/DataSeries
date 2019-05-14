#pragma once


class Decimal {
private:
	double _value;
	bool _isNaN;

public:
	Decimal();
	Decimal(const double value);
	Decimal(const Decimal& other);

	Decimal(Decimal&& other);

	Decimal operator=(const Decimal& other);
	

	Decimal operator=(Decimal&& other);

	Decimal operator+(const double value);

	Decimal operator+(const Decimal& other);

	Decimal operator-(const double value);

	Decimal operator-(const Decimal& other);

	Decimal operator*(const double value);

	Decimal operator*(const Decimal& other);

	Decimal operator/(const double value);

	Decimal operator/(const Decimal& other);

	//////////////
	void operator+=(const double value);

	void operator+=(const Decimal& other);

	void operator-=(const double value);

	void operator-=(const Decimal& other);

	void operator*=(const double value);

	void operator*=(const Decimal& other);

	void operator/=(const double value);

	void operator/=(const Decimal& other);

	~Decimal();

	friend std::ostream& operator<<(std::ostream& os, const Decimal& Decimal);
};