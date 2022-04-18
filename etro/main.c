
#include <stdio.h>
#include <etro/clist.h>


int main()
{

	void* l = f(7);
	printf("%i, %i\n", g(l), h(l));

	return 0;
}