#include "liblvgl/widgets/buttonmatrix/lv_buttonmatrix.h"
#include "main.h"
#include <cstring>
#include "selector.h"

namespace selector {
    int auton;
    int autonCount;
    const char *btnMapRed[] = {"","","","","","","","","","",""};
    const char *btnMapBlue[] = {"","","","","","","","","","",""};

    lv_obj_t *tabView;
    lv_obj_t *redBtn;
    lv_obj_t *blueBtn;

    lv_obj_t *redBtnLabel;
    lv_obj_t *blueBtnLabel;

    static void redBtnAction(lv_event_t *e) {
        if (btnMapRed[abs(auton)][0] == '\0') { auton = 0; }
        auton = auton + 1;
        lv_label_set_text(redBtnLabel, btnMapRed[abs(auton) - 1]);
        lv_obj_remove_state(redBtn, LV_STATE_PRESSED);
    }

    static void blueBtnAction(lv_event_t *e) {
        if (btnMapBlue[abs(auton)][0] == '\0') { auton = 0; }
        auton = auton - 1;
        lv_label_set_text(blueBtnLabel, btnMapBlue[abs(auton) - 1]);
        lv_obj_remove_state(blueBtn, LV_STATE_PRESSED);
        printf("%d \n", auton);
    }

    static void skillsBtnAction(lv_event_t *e) {
        auton = 0;
    }

    int tabWatcher() {
        int activeTab = lv_tabview_get_tab_active(tabView);
        while (1) {
            int currentTab = lv_tabview_get_tab_active(tabView);
            if (currentTab != activeTab) {
                activeTab = currentTab;
                switch (activeTab) {
                    case 0:
                        if (auton == 0) auton = 1;
                        auton = abs(auton);
                        lv_label_set_text(redBtnLabel, btnMapRed[abs(auton) - 1]);
                        break;
                    case 1:
                        if (auton == 0) auton = -1;
                        auton = -abs(auton);
                        lv_label_set_text(blueBtnLabel, btnMapBlue[abs(auton) - 1]);
                        break;
                    case 2:
                        auton = 0;
                        break;
                    case 3:
                        auton = 0;
                        break;
                    default:
                        break;
                }
            }
            pros::delay(20);
        }
        return 0;
    }

    void init(int defaultAuton, const char **redAutons, const char **blueAutons) {
        int i = 0;
        do {
            memcpy(&btnMapRed[i], &redAutons[i], sizeof(const char *));
            i++;
        } while (strcmp(redAutons[i], "") != 0);
        i = 0;
        do {
            memcpy(&btnMapBlue[i], &blueAutons[i], sizeof(const char *));
            i++;
        } while (strcmp(blueAutons[i], "") != 0);

        autonCount = i;
        auton = defaultAuton;

        // lv_theme_alien_init() is removed in v9; use the default theme or another built-in
        lv_theme_t *th = lv_theme_default_init(
            lv_display_get_default(),
            lv_palette_main(LV_PALETTE_BLUE),
            lv_palette_main(LV_PALETTE_RED),
            false,
            LV_FONT_DEFAULT
        );
        lv_display_set_theme(lv_display_get_default(), th);

        // lv_tabview_create signature changed: (parent, tab_bar_position, tab_bar_size)
        tabView = lv_tabview_create(lv_screen_active());
        lv_tabview_set_tab_bar_position(tabView, LV_DIR_TOP);
        lv_tabview_set_tab_bar_size(tabView, 40);

        lv_obj_t *redTab   = lv_tabview_add_tab(tabView, "Red");
        lv_obj_t *blueTab  = lv_tabview_add_tab(tabView, "Blue");
        lv_obj_t *skillsTab = lv_tabview_add_tab(tabView, "Skills");

        if (auton < 0) {
            lv_tabview_set_active(tabView, 1, LV_ANIM_OFF);
        } else if (auton == 0) {
            lv_tabview_set_active(tabView, 2, LV_ANIM_OFF);
        }

        // Red tab button
        redBtn = lv_button_create(redTab);
        redBtnLabel = lv_label_create(redBtn);
        lv_label_set_text(redBtnLabel, btnMapRed[abs(auton) - 1]);
        lv_obj_set_size(redBtn, 450, 50);
        lv_obj_align(redBtn, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_event_cb(redBtn, redBtnAction, LV_EVENT_CLICKED, NULL);

        // Blue tab button
        blueBtn = lv_button_create(blueTab);
        blueBtnLabel = lv_label_create(blueBtn);
        lv_label_set_text(blueBtnLabel, btnMapBlue[abs(auton) - 1]);
        lv_obj_set_size(blueBtn, 450, 50);
        lv_obj_align(blueBtn, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_event_cb(blueBtn, blueBtnAction, LV_EVENT_CLICKED, NULL);

        // Skills tab button
        lv_obj_t *skillsBtn = lv_button_create(skillsTab);
        lv_obj_t *skillLabel = lv_label_create(skillsBtn);
        lv_label_set_text(skillLabel, "Skills");
        lv_obj_set_size(skillsBtn, 450, 50);
        lv_obj_align(skillsBtn, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_event_cb(skillsBtn, skillsBtnAction, LV_EVENT_CLICKED, NULL);

        pros::Task tabWatcher_task(tabWatcher);
    }
}