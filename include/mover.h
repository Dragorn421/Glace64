#ifndef MOVER_H
#define MOVER_H

typedef struct Entity {
  float position[3];
  float speed;
} Entity;

extern Entity test_entity;

void entity_init();
void entity_update();
void entity_draw();
void entity_clean();

#endif // !MOVER_H
