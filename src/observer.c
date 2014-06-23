#include "observer.h"
#include <stdlib.h>

sinet_observer_t * sinet_new_observer(void * object,
		void (*notify)(sinet_observer_t *, void *))
{
	sinet_observer_t * this;
	this = (sinet_observer_t *) calloc(1, sizeof(*this));
	this->object = object;
	this->notify = (void (*)(sinet_observer_t *, void *)) notify;
	return this;
}
