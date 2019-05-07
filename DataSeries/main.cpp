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
		_vector.push_back(std::shared_ptr<T>());
	}

	T& operator[](long int index) {
		return *(_vector[index]);
	}

	T& operator[](std::string dateTime) {

		if (_map.find(dateTime) != _map.end())
			return *(_map[dateTime]);

		auto t = std::make_shared<T>();
		_map[dateTime] = t;
		_vector.pop_back();
		_vector.push_back(t);
		_vector.push_back(std::shared_ptr<T>());
		return *t;
	}

	void operator*(double n) {
		for (unsigned int i = 0; i < _vector.size() - 1; ++i)
			*(_vector[i]) = *(_vector[i]) * n;
	}

	void operator*(const DataSeries<T>& other) {
		for (unsigned int i = 0; i < _vector.size() - 1; ++i)
			*(_vector[i]) = *(_vector[i]) * *(other._vector[i]);
	}

	void operator/(double n) {
		for (unsigned int i = 0; i < _vector.size() - 1; ++i)
			*(_vector[i]) = *(_vector[i]) / n;
	}

	void operator/(const DataSeries<T>& other) {
		for (unsigned int i = 0; i < _vector.size() - 1; ++i)
			*(_vector[i]) = *(_vector[i]) / *(other._vector[i]);
	}

	template <class T>
	class Iterator {
		std::shared_ptr<T> t;
		std::vector<std::shared_ptr<T>>* vec;
		unsigned int index = 0;
	public:
		Iterator<T>(std::shared_ptr<T> t, std::vector<std::shared_ptr<T>>* vec) : t{ t }, vec{ vec } {

		}

		T& operator*() {
			return *t;
		}

		void operator++() {
			t = (*vec)[++index];
		}

		bool operator!=(const Iterator<T>& other) {
			return t != other.t;
		}
	};

	
	Iterator<T> begin() {
		Iterator<T> begin{ _vector[0], &_vector };
		return begin;
	}

	Iterator<T> end() {
		Iterator<T> end{ _vector[_vector.size() - 1], &_vector };
		return end;
	}
	
	
};

int main(int argc, char* argv[]) {

	DataSeries<double> series;
	series["2019/5/6"] = 3.14;
	series["2019/5/7"] = 2.71;
	series["2019/5/8"] = 1.23;

	for (const auto& i : series)
		std::cout << i << std::endl;

	std::cout << std::endl;

	series * 2;
	series / series[0];

	for (const auto& i : series)
		std::cout << i << std::endl;

	std::cout << std::endl;

	series * series;

	for (const auto& i : series)
		std::cout << i << std::endl;

	std::cin.get();

	return 0;
}