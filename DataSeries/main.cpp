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
		
		T& operator*() {
			if (*it == nullptr) {
				std::cout << "NaN";
			
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
				tempVector.push_back(std::shared_ptr<T>());
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

	std::cin.get();

	return 0;
}