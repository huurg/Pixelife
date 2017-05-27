#ifndef LC_TOOLSTATE_HEADER
#define LC_TOOLSTATE_HEADER

#include <SDL.h>

//#include "GridIterator.h"
#include "LC_DiamondIterator.h"
#include "LC_RectIterator.h"
#include "LC_CircleIterator.h"
#include "LC_LineIterator.h"

enum LC_ToolType {
    LC_TOOL_TYPE_NULL,
    LC_TOOL_TYPE_SELECT,
    LC_TOOL_TYPE_CREATE,
    LC_TOOL_TYPE_ERASE
};

enum LC_ToolTarget {
    LC_TOOL_TARGET_NULL,
    LC_TOOL_TARGET_LIFE,
    LC_TOOL_TARGET_TERRAIN
};

enum LC_ToolOperation {
    LC_TOOL_OPERATION_NULL,
    LC_TOOL_OPERATION_DRAW,
    LC_TOOL_OPERATION_FILL,
    LC_TOOL_OPERATION_SPRAY
};

enum LC_ToolShape {
    LC_TOOL_SHAPE_NULL,
    LC_TOOL_SHAPE_CIRCLE,
    LC_TOOL_SHAPE_RECT,
    LC_TOOL_SHAPE_GRID,
    LC_TOOL_SHAPE_LINE
};

struct LC_ToolState {
    LC_ToolType type = LC_TOOL_TYPE_CREATE;
    LC_ToolTarget target = LC_TOOL_TARGET_LIFE;
    LC_ToolOperation operation = LC_TOOL_OPERATION_DRAW;
    LC_ToolShape shape = LC_TOOL_SHAPE_RECT;
    SDL_Color colour;
    //LC_Circle circle;
    //int circle = 10;
    //SDL_Rect rect;
    //GridIterator grid;
    //LC_Line line;
    LC_DiamondIterator diamond;
    LC_RectIterator rect;
    LC_CircleIterator circle;
    LC_LineIterator line;
    double spray_density = 0.05;
    bool periodic_boundary = false;
    int brush_size = 20;
};

#endif // LC_TOOLSTATE_HEADER
