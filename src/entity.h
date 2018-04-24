#ifndef RD_ENTITY_H
#define RD_ENTITY_H

#include <zgcl/zlist.h>

#include "component.h"
#include "vector.h"

/* UNTAGGED is -1 so that an enum of tags
 * can be declared and will automatically work correctl
 */
#define UNTAGGED -1

typedef struct entity {
    vec position;
    int tag;
    int layer;
	int fixed;
	int visible;
    zlist_of(component) components;
    component kill_handler;
} entity;

extern entity *ent_new(void);
extern void ent_destroy(entity*);
extern void ent_cleanup(void);

typedef struct {
    entity *(*next)(void*);
    void (*cleanup)(void*);
    void* data;
} ent_iterator;

typedef struct {
    int (*compatible)(entity*,void*);
    void (*cleanup)(void*);
    void *data;
} ent_condition;

ent_condition any(void);
ent_condition tagged(int);
ent_condition with_component(c_table);
ent_condition within_radius(vec,float);
ent_condition not_entity(entity*);
ent_condition is_not(ent_condition);
ent_condition within_x_range(float,float);
ent_condition within_y_range(float,float);
ent_condition within_x_bounds(float,float);
ent_condition within_y_bounds(float,float);

void ent_process();

ent_iterator ent_all_(size_t,...);
entity *ent_first_(size_t,...);

#define get_component(e,c) ((c*)get_component_(e,c_type(c)))
component get_component_(entity*,c_table);

void add_component(entity*,component);

typedef int (*entity_sort_predicate)(entity*, entity*);

void ent_set_predicate(entity_sort_predicate);

#define ent_all(...) ent_all_(sizeof((ent_condition[]){ __VA_ARGS__ }), __VA_ARGS__)
#define ent_first(...) ent_first_(sizeof((ent_condition[]){ __VA_ARGS__ }), __VA_ARGS__)

#define for_ent(name,it,k) {\
    ent_iterator name ## _iterator = it;\
    entity *name = name ## _iterator.next(name ## _iterator.data);\
    for(; name; name = name ## _iterator.next(name ## _iterator.data))\
        k\
    name ## _iterator.cleanup(name ## _iterator.data);\
}

#endif