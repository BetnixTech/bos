#ifndef MOUSE_H
#define MOUSE_H

void mouse_init();
void mouse_update();       // Reads movement & buttons
int mouse_get_x();
int mouse_get_y();
int mouse_left_pressed();
int mouse_right_pressed();

#endif
