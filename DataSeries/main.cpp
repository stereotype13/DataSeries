#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>

template <class T>
class DataSeries {
private:
	std::vector<std::shared_ptr<T>> _vector;
	std::map<std::string, std::shared_ptr<T>> _map;

	void makeDeepCopy(const DataSeries<T>& other) {
		std::vector<std::shared_ptr<T>> _newVector;
		std::map<std::string, std::shared_ptr<T>> _newMap;

		for (const auto& i : other._map) {
			std::pair<std::string, std::shared_ptr<T>> tempPair;
			tempPair.first = i.first;
			tempPair.second = std::make_shared<T>(*(i.second));
			_newMap.emplace(tempPair);
		}

		for (const auto& i : other._vector) {
			_newVector.push_back(std::make_shared<T>(*i));
		}

		this->_vector.clear();
		this->_map.clear();
		this->_vector = _newVector;
		this->_map = _newMap;
	}

public:

	DataSeries<T>() = default;

	DataSeries<T>(const DataSeries<T>& other) {
		//make a deep copy
		makeDeepCopy(other);
	}

	DataSeries<T>& operator=(const DataSeries<T>& other) {
		if (this == &other)
			return *this;

		//make a deep copy
		makeDeepCopy(other);
		return *this;
	}

	DataSeries<T>(DataSeries<T>&& other) {
		std::swap(_vector, other._vector);
		std::swap(_map, other._map);
	}

	DataSeries<T>& operator=(DataSeries<T>&& other) {
		std::swap(_vector, other._vector);
		std::swap(_map, other._map);

		return *this;
	}

	T& operator[](long int index) {
		return *(_vector[index]);
	}

	T& operator[](std::string dateTime) {

		if (_map.find(dateTime) != _map.end())
			return *(_map[dateTime]);

		auto t = std::make_shared<T>();
		_map[dateTime] = t;
		_vector.push_back(t);
		return *t;
	}

	void operator*=(double n) {
		for (unsigned int i = 0; i < _vector.size(); ++i)
			*(_vector[i]) = *(_vector[i]) * n;
	}

	void operator*=(const DataSeries<T>& other) {
		std::vector<std::shared_ptr<T>> tempVector;
		for (unsigned int i = 0; i < _vector.size(); ++i)
			tempVector.push_back(std::make_shared<T>(*(_vector[i]) * *(other._vector[i])));

		_vector.clear();
		_vector = tempVector;
	}

	void operator/=(double n) {
		for (unsigned int i = 0; i < _vector.size(); ++i)
			*(_vector[i]) = *(_vector[i]) / n;
	}

	void operator/=(const DataSeries<T>& other) {
		std::vector<std::shared_ptr<T>> tempVector;
		for (unsigned int i = 0; i < _vector.size(); ++i)
			tempVector.push_back(std::make_shared<T>(*(_vector[i]) / *(other._vector[i])));

		_vector.clear();
		_vector = tempVector;
	}


	class Iterator {
	public:
		typename std::vector<std::shared_ptr<T>>::iterator it;
		
		T operator*() {
			if (*it == nullptr) {
			
				T temp;
				memset(&temp, 0, sizeof(T));
				return temp;
			}

			return **it;
		}

		Iterator& operator++() {
			++it;
			return *this;
		}

		bool operator!=(const Iterator& other) {
			return this->it != other.it;
		}
		
	};

	
	Iterator begin() {
		Iterator iterator;
		iterator.it = _vector.begin();
		return iterator;
	}

	Iterator end() {
		Iterator iterator;
		iterator.it = _vector.end();
		return iterator;
	}

	DataSeries<T> shift(long int shift) {
		DataSeries<T> shiftedDataSeries;
		std::vector<std::shared_ptr<T>> tempVector;

		if (shift < 0) {
			shift *= -1;
		}
		else {
			for (unsigned int i = 0; i < shift; ++i) {
				T temp;
				tempVector.push_back(std::make_shared<T>(temp));
			}
			shift = 0;
		}

		for (unsigned int i = shift; i < _vector.size(); ++i) {
			tempVector.push_back(std::make_shared<T>(*(_vector[i])));
		}
		
		shiftedDataSeries._vector.clear();
		shiftedDataSeries._vector = tempVector;
		return shiftedDataSeries;
	}
	
	
};

//test
class OHLC {
public:
	double Open, High, Low, Close;
	bool isNaN = true;

	OHLC() = default;
	OHLC(double open, double high, double low, double close) : Open{ open }, High{ high }, Low{ low }, Close{ close }, isNaN{ false } { }
	OHLC(const OHLC& other) : Open{ other.Open }, High{ other.High }, Low{ other.Low }, Close{ other.Close }, isNaN{ other.isNaN } {}

	OHLC& operator=(const OHLC& other) {
		if (this == &other)
			return *this;

		Open = other.Open;
		High = other.High;
		Low = other.Low;
		Close = other.Close;
		isNaN = other.isNaN;

		return *this;
	}
	
	OHLC(OHLC&& other) {
		std::swap(Open, other.Open);
		std::swap(High, other.High);
		std::swap(Low, other.Low);
		std::swap(Close, other.Close);
		std::swap(isNaN, other.isNaN);
	}

	OHLC& operator=(OHLC&& other) {
		std::swap(Open, other.Open);
		std::swap(High, other.High);
		std::swap(Low, other.Low);
		std::swap(Close, other.Close);
		std::swap(isNaN, other.isNaN);

		return *this;
	}

	OHLC operator/(const OHLC& other) {
		OHLC temp;
		temp.Open = Open / other.Open;
		temp.High = High / other.High;
		temp.Low = Low / other.Low;
		temp.Close = Close / other.Close;

		temp.isNaN = isNaN;

		return temp;
	}

	OHLC operator/(double n) {
		OHLC temp;
		temp.Open = Open / n;
		temp.High = High / n;
		temp.Low = Low / n;
		temp.Close = Close / n;

		temp.isNaN = isNaN;

		return temp;
	}

	OHLC operator*(const OHLC& other) {
		OHLC temp;
		temp.Open = Open * other.Open;
		temp.High = High * other.High;
		temp.Low = Low * other.Low;
		temp.Close = Close * other.Close;

		temp.isNaN = isNaN;

		return temp;
	}

	OHLC operator*(double n) {
		OHLC temp;
		temp.Open = Open * n;
		temp.High = High * n;
		temp.Low = Low * n;
		temp.Close = Close * n;

		temp.isNaN = isNaN;

		return temp;
	}

	friend std::ostream& operator<<(std::ostream& os, const OHLC& ohlc) {
		if (ohlc.isNaN) {
			os << "NaN" << '\t' << "NaN" << '\t' << "NaN" << '\t' << "NaN";
			return os;
		}
			
		os << ohlc.Open << '\t' << ohlc.High << '\t' << ohlc.Low << '\t' << ohlc.Close;
		return os;
	}
};

class Decimal {
private:
	double _value;
	bool _isNaN;

public:
	Decimal() : _value{ 0.0 }, _isNaN{ true } {}
	Decimal(const double value) : _value{ value }, _isNaN{ false } {}
	Decimal(const Decimal& other) : _value{ other._value }, _isNaN{ other._isNaN } {}
	
	Decimal(Decimal&& other) : _value{ 0.0 }, _isNaN{ true } {
		std::swap(_value, other._value);
		std::swap(_isNaN, other._isNaN);
	}

	Decimal operator=(const Decimal& other) {
		if (this == &other)
			return *this;

		_value = other._value;
		_isNaN = other._isNaN;

		return *this;
	}

	Decimal operator=(Decimal&& other) {
		std::swap(_value, other._value);
		std::swap(_isNaN, other._isNaN);

		return *this;
	}

	Decimal operator+(const double value) {
		return this->_value + value;
	}

	Decimal operator+(const Decimal& other) {
		return this->_value + other._value;
	}

	Decimal operator-(const double value) {
		return this->_value - value;
	}

	Decimal operator-(const Decimal& other) {
		return this->_value - other._value;
	}

	Decimal operator*(const double value) {
		return this->_value * value;
	}

	Decimal operator*(const Decimal& other) {
		return this->_value * other._value;
	}

	Decimal operator/(const double value) {
		return this->_value / value;
	}

	Decimal operator/(const Decimal& other) {
		return this->_value / other._value;
	}

	//////////////
	void operator+=(const double value) {
		this->_value += value;
	}

	void operator+=(const Decimal& other) {
		this->_value += other._value;
	}

	void operator-=(const double value) {
		this->_value -= value;
	}

	void operator-=(const Decimal& other) {
		this->_value -= other._value;
	}

	void operator*=(const double value) {
		this->_value *= value;
	}

	void operator*=(const Decimal& other) {
		this->_value *= other._value;
	}

	void operator/=(const double value) {
		this->_value /= value;
	}

	void operator/=(const Decimal& other) {
		this->_value /= other._value;
	}


	friend std::ostream& operator<<(std::ostream& os, const Decimal& Decimal) {
		if (Decimal._isNaN) {
			os << "NaN";
			return os;
		}

		os << Decimal._value;
		return os;
	}
};

int main(int argc, char* argv[]) {

	DataSeries<double> series;
	series["2019/5/6"] = 3.14;
	series["2019/5/7"] = 2.71;
	series["2019/5/8"] = 1.23;

	DataSeries<double> returns = series.shift(-1);
	returns /= series;

	for (const auto& i : returns)
		std::cout << i << std::endl;

	std::cout << std::endl;



	for (const auto& i : series)
		std::cout << i << std::endl;

	std::cout << std::endl;

	
	series /= series[0];
	series *= 2;

	for (const auto& i : series)
		std::cout << i << std::endl;

	std::cout << std::endl;

	series *= series;

	for (const auto& i : series)
		std::cout << i << std::endl;

	std::cout <<  std::endl;

	DataSeries<double> shifted = series.shift(1);

	for (const auto& i : shifted)
		std::cout << i << std::endl;

	std::cout << std::endl;

	DataSeries<OHLC> series2;

	series2["2019/5/6"] = OHLC{ 40, 65, 35, 60 };
	series2["2019/5/7"] = OHLC{ 55, 60, 45, 50 };
	series2["2019/5/8"] = OHLC{ 60, 80, 60, 70 };

	for (const auto& i : series2)
		std::cout << i << std::endl;

	std::cout << std::endl;

	DataSeries<OHLC> returns2 = series2.shift(-1);
	returns2 /= series2;

	for (const auto& i : returns2)
		std::cout << i << std::endl;

	std::cout << std::endl;

	DataSeries<Decimal> Decimals;
	Decimals["2019/5/6"] = 3.14;
	Decimals["2019/5/7"] = 2.71;
	Decimals["2019/5/8"] = 1.23;

	Decimals[0] = Decimals[1] + Decimals[2];

	std::cout << "Decimals[0] = " << Decimals[0] << std::endl;

	Decimals = Decimals.shift(1);
	std::cout << "Decimals[0] = " << Decimals[0] << std::endl;
	std::cout << "Decimals[1] = " << Decimals[1] << std::endl;

	std::cin.get();

	return 0;
}