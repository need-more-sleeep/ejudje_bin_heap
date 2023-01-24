#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <exception>
#include <sstream>
class heap {
	//K         V
	std::vector <std::pair<int64_t, std::string> >data;
	//K       pos int vector
	std::unordered_map <int64_t, size_t> map;

public:

	int parent(int pos) const {
		return (pos - 1) / 2;
	}
	int left(int pos) const {
		return 2 * pos + 1;
	}
	int right(int pos) const {
		return 2 * pos + 2;
	}


	int index(int64_t key) {
		auto el = map.find(key);
		if (el != map.end()) {
			return (*el).second;
		}
		return -1;
	}

	bool insert(int64_t key, const std::string& value) {
		if (map.find(key) != map.end()) {
			return false;
		}

		data.push_back(std::pair<int64_t, std::string>(key, value));
		map[key] = data.size() - 1;

		heapify_up(data.size() - 1);

		return true;
	}

	void set(int64_t key, const std::string& value) {
		if (map.find(key) == map.end()) {
			throw std::runtime_error("no such element");
		}
		data[map[key]].second = value;
	}

	const std::pair<int64_t, std::string>* search(int64_t key) {
		if (map.find(key) != map.end()) {
			return &data[map[key]];
		}
		return nullptr;
	}

	void heapify(int i) {
		int l = left(i);
		int r = right(i);
		int min;
		if (l < data.size() && data[l].first < data[i].first) {
			min = l;
		}
		else {
			min = i;
		}
		if (r < data.size() && data[r].first < data[min].first) {
			min = r;
		}
		if (min != i) {
			map[data[i].first] = min;
			map[data[min].first] = i;
			std::swap(data[i], data[min]);
			heapify(min);
		}
	}

	std::pair<int64_t, std::string> extract() {
		if (data.size() == 0) {
			throw std::runtime_error("heap is empty");
		}
		auto min = data[0];
		data[0] = data[data.size() - 1];
		map[data[0].first] = 0;
		map.erase(min.first);
		data.pop_back();
		heapify(0);
		return min;
	}

	const std::pair<int64_t, std::string>* min()const {

		if (data.size() == 0) {
			return nullptr;
		}
		return &data[0];
	}

	const std::pair<int64_t, std::string>* max()const {

		if (data.size() == 0) {
			return nullptr;
		}
		auto max = &data[data.size() / 2];
		for (size_t i = data.size() / 2; i < data.size(); i++)
		{
			if (data[i].first > max->first) {
				max = &data[i];
			}
		}
		return max;
	}


	void heapify_up(int i) {
		int par = parent(i);
		while (i > 0 && data[i].first < data[par].first) {
			map[data[i].first] = par;
			map[data[par].first] = i;
			std::swap(data[i], data[par]);
			i = par;
			par = parent(i);
		}
	}

	bool erase(int64_t key) {
		if (map.find(key) == map.end()) {
			return false;
		}
		auto el = map.find(key);
		size_t pos = (*el).second;
		if (pos == data.size() - 1) {
			data.pop_back();
			map.erase(key);
			return true;
		}

		map[data[pos].first] = data.size() - 1;
		map[data[data.size() - 1].first] = pos;
		std::swap(data[data.size() - 1], data[pos]);

		map.erase(key);
		data.pop_back();

		heapify_up(pos);
		heapify(pos);
		return true;
	}


	friend void print(const heap& h, std::string& answer) {

		int count = 2;
		if (h.data.size() == 0) {
			answer += "_\n";
		}
		for (size_t i = 0; i < h.data.size();) {
			if (i == 0) {
				answer += "[" + std::to_string(h.data[i].first) + " " + h.data[i].second + "]\n";
				++i;
			}
			else {
				for (size_t j = 0; j < count; j++, i++)
				{
					if (i < h.data.size()) {
						if (j == count - 1) {
							answer += "[" + std::to_string(h.data[i].first) + " " + h.data[i].second + " " + std::to_string(h.data[h.parent(i)].first) + "]\n";
						}
						else {
							answer += "[" + std::to_string(h.data[i].first) + " " + h.data[i].second + " " + std::to_string(h.data[h.parent(i)].first) + "] ";
						}
					}
					else {
						if (j == count - 1) {
							answer += "_\n";
						}
						else {
							answer += "_ ";

						}
					}
				}
				count <<= 1;
			}
		}
	}
};




void split(std::string& s, std::vector<std::string>& output)
{
	size_t start = 0;
	size_t end = s.find_first_of(' ');

	output.clear();
	while (end <= std::string::npos)
	{
		output.emplace_back(s.substr(start, end - start));

		if (end == std::string::npos)
			break;

		start = end + 1;
		end = s.find_first_of(' ', start);
	}

}



int main() {
	heap h;
	std::stringstream out;
	std::string line;
	std::vector<std::string> split_line;
	std::string answer;
	while (std::getline(std::cin, line)) {
		if (line.empty()) {
			continue;
		}
		split(line, split_line);
		if (split_line[0] == "add" && split_line.size() == 3) {
			int64_t temp = std::stoll(split_line[1]);
			if (h.insert(temp, split_line[2])) {
				continue;
			}

		}
		else if (split_line[0] == "set" && split_line.size() == 3) {
			try {

				int64_t temp = std::stoll(split_line[1]);

				h.set(temp, split_line[2]);
				continue;
			}
			catch (std::exception) {}

		}
		else if (split_line[0] == "search" && split_line.size() == 2) {
			try {
				int64_t temp = std::stoll(split_line[1]);

				auto el = h.search(temp);
				if (el != nullptr) {
					answer += "1 " + std::to_string(h.index(el->first)) + " " + el->second + "\n";
					continue;
				}
				else {
					answer += "0\n";
					continue;
				}
			}
			catch (std::invalid_argument) {}
		}
		else if (split_line[0] == "delete" && split_line.size() == 2) {
			try {
				int64_t temp = std::stoll(split_line[1]);

				auto del = h.erase(temp);
				if (del == true) {
					continue;
				}

			}
			catch (std::invalid_argument) {}
		}
		else if (split_line[0] == "extract" && split_line.size() == 1) {
			try {
				auto el = h.extract();
				answer += std::to_string(el.first) + " " + el.second + "\n";
				continue;
			}
			catch (std::runtime_error) {}

		}
		else if (split_line[0] == "min" && split_line.size() == 1) {
			auto el = h.min();
			if (el != nullptr) {
				answer += std::to_string(el->first) + " " + std::to_string(h.index(el->first)) + " " + el->second + "\n";
				continue;
			}
		}
		else if (split_line[0] == "max" && split_line.size() == 1) {
			auto el = h.max();
			if (el != nullptr) {
				answer += std::to_string(el->first) + " " + std::to_string(h.index(el->first)) + " " + el->second + "\n";
				continue;
			}
		}
		else if (split_line[0] == "print" && split_line.size() == 1) {
			print(h, answer);
			continue;
		}

		answer += "error\n";
	}
	printf(answer.c_str());
}