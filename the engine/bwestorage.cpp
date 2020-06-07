#include "bwestorage.h"
#pragma once

template <typename T>
EnumContainer<T>::EnumContainer() : created(), destroyed(), items()
{

}

template<typename T>
EnumContainer<T>::~EnumContainer()
{
	for (T* i : items) {
		delete i;
	}
}

template <typename T>
int EnumContainer<T>::add(T* item) {
	int position;
	if (destroyed.size() > 0) {
		position = destroyed.back();
		items.at(position) = item;
		created.push_back(position);
		destroyed.pop_back();
	}
	else {
		position = items.size();
		items.push_back(item);
		created.push_back(position);
	}
	return position;
}

template <typename T>
T* EnumContainer<T>::get(int i) {//might want some safety on
	return items.at(i);
}

template <typename T>
bool EnumContainer<T>::remove(int itemid) {
	for (size_t i = 0; i < created.size(); i++) {
		if (created[i] == itemid) {
			destroyed.push_back(i);
			created.erase(created.begin() + i, created.begin() + i + 1);
			return true;
		}
	}
	return false;
}

template <typename T>
int EnumContainer<T>::getCreated(int index) {
	if (index < created.size()) {
		return created[index];
	}
	return -1;
}