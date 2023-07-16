#ifndef OBJECT_H
#define OBJECT_H

#define NUM_OBJECTS 20

typedef enum ObjectType { OBJ_PLAYER, OBJ_CRAB } ObjectType;

// everything takes in void pointers, need to be very generic about this.
typedef void (*InitFn)(void *); // when it's added to the objects array.
                                // different from construction.
typedef void (*UpdateFn)(void *);
typedef void (*DrawFn)(void *);
typedef void (*CleanFn)(void *); // when it's removed from the objects array.
                                 // different from destruction.

// have a LUT of ObjectFnPointers for each type of object.
// the functions themselves are passed a void *, and trusted to cast to the
// appropriate type.
typedef struct ObjectFnPointers {
  InitFn init;
  UpdateFn update;
  DrawFn draw;
  CleanFn clean;
} ObjectFnPointers;

// handled in the external LUT, an object is linked to a FnPointers structure by
// its type.
typedef struct Object {
  ObjectType type;
} Object;

typedef struct ObjectState {
  Object *objects[NUM_OBJECTS]; // array of pointers.
} ObjectState;

extern ObjectState object_state;

Object *object_add(Object *o);
void object_remove_by_ptr(Object *o);

void object_init();
void object_update();
void object_draw();
void object_clean();

#endif // !OBJECT_H
