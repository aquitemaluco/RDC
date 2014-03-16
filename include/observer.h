#ifndef OBSERVER_H
#define OBSERVER_H

	typedef struct {
		void *object;
		void (*notify)(void *data);
	} sinet_observable_t;

	typedef struct {
		void ** observer_collection;
	} sinet_observer_t;

	sinet_register_object(sinet_observer_t observer, void *object);
	sinet_notify_objects(sinet_observer_t observer);


#endif

