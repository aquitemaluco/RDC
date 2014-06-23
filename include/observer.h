#ifndef OBSERVER_H
#define OBSERVER_H

typedef struct __sinet_observer {
	void * object;
	void (*notify)(struct __sinet_observer *, void *);
} sinet_observer_t;

sinet_observer_t * sinet_new_observer(void *,
		void(*)(sinet_observer_t *, void *));

#endif

