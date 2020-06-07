#pragma once

#include <vector>

template <typename T>
class EnumContainer {
public:
	EnumContainer();
	~EnumContainer();

	int add(T* item);
	T* get(int itemid);
	bool remove(int itemid);
	int getCreated(int itemid);
private:
	std::vector<int> created;
	std::vector<int> destroyed;
	std::vector<T*> items;
};

#include "bwestorage.cpp"
