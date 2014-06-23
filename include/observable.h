#ifndef OBSERVABLE_H
#define OBSERVABLE_H

typedef struct __sinet_observable {
	void * object;
	void ** observerCollection;
	int (*registerObserver)(struct __sinet_observable *,
			sinet_observer_t *);
	int (*unregisterObserver)(struct __sinet_observable *,
			sinet_observer_t *);
	void (*notifyObservers)(struct __sinet_observable *, void *);
} sinet_observable_t;

sinet_observable_t * sinet_new_observable(void *);

#endif
