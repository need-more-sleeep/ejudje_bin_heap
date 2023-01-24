#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <exception>
struct node {
	bool is_empty = true;
	long long int key;
	size_t pos;
	std::string data;
	node() = default;
	node(long long int K, const std::string& V, unsigned int position) :key(K), data(V), pos(position), is_empty(false) {}
};

class bin_heap {
	std::vector <node> arr;
	//key, position in arr
	std::unordered_map <long long int, size_t> table;

public:

	friend void print(bin_heap& h);

	~bin_heap() = default;

	bool add(long long int K, const std::string& V);

	void heapify(int i);
	void heapify_up(int i);

	node* search(long long int K);

	node* min();

	node* max();

	bool remove(long long int K);

	bool set(long long int K, const std::string& V);

	int parent(int i);

	node extract();
};

bool bin_heap::add(long long int K, const std::string& V)
{
	auto it = table.find(K);
	if (it == table.end()) {

		node new_el(K, V, arr.size());

		arr.emplace(arr.end(), new_el);
		table[K] = arr.size() - 1;

		size_t  i = arr.size() - 1;

		heapify_up(i);
		return true;
	}
	return false;

}

void bin_heap::heapify(int i) {
	while (true) {
		int left = 2 * i + 1;
		int right = 2 * i + 2;
		int smallest = i;

		if (left < arr.size()) {
			if (arr[left].key < arr[smallest].key) {
				smallest = left;
			}
		}
		if (right < arr.size()) {
			if (arr[right].key < arr[smallest].key) {
				smallest = right;
			}
		}
		if (smallest == i)
		{
			break;
		}
		table[arr[i].key] = smallest;
		table[arr[smallest].key] = i;
		std::swap(arr[i], arr[smallest]);
		i = smallest;
	}
}

void bin_heap::heapify_up(int i) {
	int par = parent(i);
	while (i > 0 && arr[i].key < arr[par].key) {
		table[arr[i].key] = par;
		table[arr[par].key] = i;
		std::swap(arr[i], arr[par]);
		i = par;
		par = parent(i);
	}
}

node* bin_heap::search(long long int K) {
	auto elem = table.find(K);
	if (elem != table.end()) {
		size_t pos = (*elem).second;
		arr[pos].pos = pos;
		return &arr[pos];
	}
	return nullptr;

}

node* bin_heap::min() {
	if (arr.empty()) {
		return nullptr;
	}
	return &arr[0];
}

node* bin_heap::max() {
	if (arr.empty()) {
		return nullptr;
	}
	long long int max = arr[arr.size() / 2].key;
	size_t pos = arr.size() / 2;
	arr[pos].pos = pos;
	for (size_t i = arr.size() / 2; i < arr.size(); i++)
	{
		if (arr[i].key > max) {
			max = arr[i].key;
			pos = i;
			arr[i].pos = pos;
		}
	}
	return &arr[pos];
}

bool bin_heap::remove(long long int K) {
	auto elem = table.find(K);
	if (elem != table.end()) {
		size_t pos = (*elem).second;
		if (pos == arr.size() - 1) {
			table.erase(K);
			arr.pop_back();
			return true;
		}
		table[arr[pos].key] = arr.size() - 1;
		table[arr[arr.size() - 1].key] = pos;
		std::swap(arr[pos], arr[arr.size() - 1]);

		table.erase(K);
		arr.pop_back();

		heapify_up(pos);

		heapify(pos);
		return true;
	}
	return false;

}

bool bin_heap::set(long long int K, const std::string& V) {
	auto elem = table.find(K);
	if (elem != table.end()) {
		size_t pos = (*elem).second;
		arr[pos].data = V;
		return true;
	}
	return false;

}


int bin_heap::parent(int i) {
	return  (i - 1) / 2;
}

node bin_heap::extract() {
	if (arr.size() == 0) {
		throw std::runtime_error("empty heap");
	}
	node temp = arr[0];
	table.erase(arr[0].key);
	table[arr[arr.size() - 1].key] = 0;
	arr[0] = arr[arr.size() - 1];
	arr.pop_back();
	heapify(0);
	return temp;
}

void print(bin_heap& h) {
	if (h.arr.size() == 0) {
		std::cout << "_" << std::endl;
		return;
	}
	std::string path;
	//count of printed nodes, level in tree
	for (int i = 0, count = 0, level = 0; i < h.arr.size(); i++) {
		if (count == (1 << level)) {
			count = 0;
			++level;
			path += "\n";
		}

		if (h.arr[i].is_empty) {
			path += " _";
			return;
		}
		else if (i == 0) {
			path += "[" + std::to_string(h.arr[i].key) + " " + h.arr[i].data + "]";
			++count;
		}
		else {
			if (count == 0) {
				path += "[" + std::to_string(h.arr[i].key) + " " + h.arr[i].data + " " + std::to_string(h.arr[h.parent(i)].key) + "]";
			}
			else {
				path += " [" + std::to_string(h.arr[i].key) + " " + h.arr[i].data + " " + std::to_string(h.arr[h.parent(i)].key) + "]";
			}
			count++;
		}

		if (i == h.arr.size() - 1) {
			for (size_t i = count; i < (1 << level); i++)
			{
				path += " _";
			}
		}
	}
	std::cout << path << std::endl;
}

int main() {

	bin_heap h;

	std::regex regex_add("^add (-?[0-9]+) ([^ ]{0,})$");
	std::regex regex_set("^set (-?[0-9]+) ([^ ]{0,})$");

	std::regex regex_delete("^delete (-?[0-9]+)$");
	std::regex regex_search("^search (-?[0-9]+)$");

	std::regex regex_min("^min$");
	std::regex regex_max("^max$");

	std::regex regex_print("^print$");

	std::regex regex_extract("^extract$");

	std::smatch match;
	std::string str;


	while (std::getline(std::cin, str)) {
		if (str.empty()) {
			continue;
		}
		else if (std::regex_match(str, match, regex_add)) {
			if (!h.add(std::stoll(match[1]), match[2])) {
				std::cout << "error" << std::endl;
			}
		}
		else if (std::regex_match(str, match, regex_set)) {
			if (!h.set(std::stoll(match[1]), match[2])) {
				std::cout << "error" << std::endl;
			}
		}
		else if (std::regex_match(str, match, regex_delete)) {
			if (!h.remove(std::stoll(match[1]))) {
				std::cout << "error" << std::endl;
			}
		}
		else if (std::regex_match(str, match, regex_search)) {
			node* temp = h.search(std::stoll(match[1]));
			if (temp != nullptr) {
				std::cout << "1 " << temp->pos << " " << temp->data << std::endl;
			}
			else {
				std::cout << "0" << std::endl;
			}
		}
		else if (std::regex_match(str, match, regex_min)) {
			node* temp = h.min();
			if (temp != nullptr) {
				std::cout << temp->key << " 0 " << temp->data << std::endl;
			}
			else {
				std::cout << "error" << std::endl;
			}
		}
		else if (std::regex_match(str, match, regex_max)) {
			node* temp = h.max();
			if (temp != nullptr) {
				std::cout << temp->key << " " << temp->pos << " " << temp->data << std::endl;
			}
			else {
				std::cout << "error" << std::endl;
			}
		}
		else if (std::regex_match(str, match, regex_extract)) {
			try {
				node temp = h.extract();
				std::cout << temp.key << " " << temp.data << std::endl;
			}
			catch (std::exception) {
				std::cout << "error" << std::endl;
			}
		}
		else if (std::regex_match(str, match, regex_print)) {
			print(h);
		}
		else {
			std::cout << "error" << std::endl;
		}
	}
}