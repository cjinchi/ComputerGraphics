#ifndef STATE_H
#define STATE_H

enum shape{LINE,CIRCLE,ELLIPSE,POLYGON,CURVE,CLIP};

enum dragging_state {INVALID,WAITING,DRAGGING,DECIDING,EDITING,MOVING};

enum fill_state{FILL_READY,FILL_WAITING};

enum global_state {NONE,DRAG,PARA,FILL};

#endif // STATE_H
