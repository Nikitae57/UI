//
// Created by Nikita on 12.05.2019.
//

#ifndef LAB7_ENUMS_H
#define LAB7_ENUMS_H

#define SIGNALS_COUNT 2
#define STATES_COUNT 2
#define ACTIONS_COUNT 4
#define UI_ELEMENTS_COUNT 2

enum SIGNALS {
    CLICK_IMG_BTN, CLICK_STR_BTN
};

enum STATES {
    ACTIVE, INACTIVE
};

enum ACTIONS {
    RUN_STR, STOP_STR,
    RUN_IMG, STOP_IMG
};

enum UI_ELEMENTS {
    BTN_IMAGE, BTN_STRING
};

#endif //LAB7_ENUMS_H
