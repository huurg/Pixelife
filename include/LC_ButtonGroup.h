#ifndef LC_BUTTON_GROUP_HEADER
#define LC_BUTTON_GROUP_HEADER

#include "LC_GamePanel.h"
#include "LC_Button.h"

class LC_ButtonGroup : public LC_GamePanel {
    private:
        int button_group_state = 0;
    public:
        LC_ButtonGroup(int in_mb, int in_mgp);
        bool mouseUpdate(SDL_Event* event);
        void frameUpdate();
        void windowUpdate(SDL_Event* event);
        void keyUpdate(SDL_Event* event);
        void render();
        int getButtonGroupState() const;
};

#endif // LC_BUTTON_GROUP_HEADER
