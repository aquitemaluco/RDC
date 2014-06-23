#include "observer.h"
#include "observable.h"
#include  <stdlib.h>

static int _registerObserver(sinet_observable_t * this,
		sinet_observer_t * sinet_observer)
{
	int i, size;
	size = sizeof(this->observerCollection);
	for(i = 0; i < size; i++) {
		if (NULL == this->observerCollection[i]) {
			this->observerCollection[i] = sinet_observer;
			return 1;
		}
	}
	return 0;
}

static int _unregisterObserver(sinet_observable_t * this,
		sinet_observer_t * sinet_observer)
{
	int i, size;
	size = sizeof(this->observerCollection);
	for(i = 0; i < size; i++) {
		void * observer = this->observerCollection[i];
		if (observer == sinet_observer) {
			observer = NULL;
			return 1;
		}
	}
	return 0;
}

static void _notifyObservers(sinet_observable_t * this, void * object)
{
	int i, size;
	size = sizeof(this->observerCollection);
	sinet_observer_t * observer;
	for(i = 0; i < size; i++) {
		observer = this->observerCollection[i];
		if (NULL != observer) {
			observer->notify(observer, object);
		}
	}
}

sinet_observable_t * sinet_new_observable(void * object)
{
	sinet_observable_t * this;
	this = (sinet_observable_t *) calloc(1, sizeof(*this));
	this->object = object;
	this->registerObserver = _registerObserver;
	this->unregisterObserver = _unregisterObserver;
	this->notifyObservers = _notifyObservers;
	return this;
}
