#ifndef __LIST_H__
#define __LIST_H__

template<typename T>
class List
{
public:
	List(T elem);
	List<T>* next();
	T elem();
	void add(T elem);

private:
	T element;
	List<T>* nextptr;
};

template<typename T>
List<T>::List(T elem) : 
	element{elem}, 
	nextptr{nullptr}
{
}

template<typename T>
List<T>* List<T>::next()
{
	return this->nextptr;
}

template<typename T>
T List<T>::elem()
{
	return this->element;
}

template<typename T>
void List<T>::add(T elem)
{
	List<T> newList = List<T>(elem);
	this->nextptr = &newList;
}

//int k(int a, int b);

#endif