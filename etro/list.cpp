//extern "C" int f(int);
#include <etro/clist.h>
#include <etro/list.h>
void *f(int i)
{
	List<int> *x = new List<int>(i);
	x->add(6);
	return (void*)(x);
}

int g(void* l)
{
	return ((List<int>*)l)->elem();
}

int h(void* l)
{
	return ((List<int>*)l)->next()->elem();
}



