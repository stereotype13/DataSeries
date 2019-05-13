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

public:

	DataSeries<T>() {
		//_vector.push_back(std::shared_ptr<T>());
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

	void operator*(double n) {
		for (unsigned int i = 0; i < _vector.size(); ++i)
			*(_vector[i]) = *(_vector[i]) * n;
	}

	void operator*(const DataSeries<T>& other) {
		std::vector<std::shared_ptr<T>> tempVector;
		for (unsigned int i = 0; i < _vector.size(); ++i)
			tempVector.push_back(std::make_shared<T>(*(_vector[i]) * *(other._vector[i])));

		_vector.clear();
		_vector = tempVector;
	}

	void operator/(double n) {
		for (unsigned int i = 0; i < _vector.size(); ++i)
			*(_vector[i]) = *(_vector[i]) / n;
	}

	void operator/(const DataSeries<T>& other) {
		std::vector<std::shared_ptr<T>> tempVector;
		for (unsigned int i = 0; i < _vector.size(); ++i)
			tempVector.push_back(std::make_shared<T>(*(_vector[i]) / *(other._vector[i])));

		_vector.clear();
		_vector = tempVector;
	}


	class Iterator {
	public:
		typename std::vector<std::shared_ptr<T>>::iterator it;
		
		T& operator*() {
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

		for (unsigned int i = -shift; i < _vector.size(); ++i) {
			tempVector.push_back(std::make_shared<T>(*(_vector[i])));
		}
		
		shiftedDataSeries._vector = tempVector;
		return shiftedDataSeries;
	}
	
	
};

int main(int argc, char* argv[]) {

	DataSeries<double> series;
	series["2019/5/6"] = 3.14;
	series["2019/5/7"] = 2.71;
	series["2019/5/8"] = 1.23;

	DataSeries<double> returns = series.shift(-1);
	returns / series;

	for (const auto& i : returns)
		std::cout << i << std::endl;

	std::cout << std::endl;



	for (const auto& i : series)
		std::cout << i << std::endl;

	std::cout << std::endl;

	
	series / series[0];
	series * 2;

	for (const auto& i : series)
		std::cout << i << std::endl;

	std::cout << std::endl;

	series * series;

	for (const auto& i : series)
		std::cout << i << std::endl;

	DataSeries<double> shifted = series.shift(-1);

	std::cin.get();

	return 0;
}