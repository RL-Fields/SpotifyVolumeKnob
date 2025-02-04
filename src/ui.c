// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: VolumeKnobGUI

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
//char entity = "media_player.adam_s_echo_studio"; //device to control


 //variables for toggling in and out of volume mode
//btn_hold_time = 0;
//is_button_held = false;
//toggle_volume_mode = false;
//previousEncoderValue = 0; // Initialize the previous encoder value



// SCREEN: ui_SongControls
void ui_SongControls_screen_init(void);
lv_obj_t * ui_SongControls;
lv_obj_t * ui_MusicArtwork;
lv_obj_t * ui_Artist;
lv_obj_t * ui_SongName;
lv_obj_t * ui_Source;
lv_obj_t * ui_LowerPanel;
lv_obj_t * ui_SongProgressBar;
void ui_event_ButtonPlayPause(lv_event_t * e);
lv_obj_t * ui_ButtonPlayPause;
void ui_event_ButtonBack(lv_event_t * e);
lv_obj_t * ui_ButtonBack;
lv_obj_t * ui_ButtonShuffle;
void ui_event_ButtonNext(lv_event_t * e);
lv_obj_t * ui_ButtonNext;
lv_obj_t * ui_ButtonRepeat;
lv_obj_t * ui_DisplaySource;


// SCREEN: ui_VolumeKnob
void ui_VolumeKnob_screen_init(void);
lv_obj_t * ui_VolumeKnob;
lv_obj_t * ui_Arc1;
lv_obj_t * ui_VolumeCount;
lv_obj_t * ui_Image1;
lv_obj_t * ui____initial_actions0;
const lv_img_dsc_t * ui_imgset_1859717651[1] = {&ui_img_183454340};

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_ButtonPlayPause(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        btn_event_cb(e);
    }
}
void ui_event_ButtonBack(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        btn_event_cb(e);
    }
}
void ui_event_ButtonNext(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        (e);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    LV_EVENT_GET_COMP_CHILD = lv_event_register_id();

    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_SongControls_screen_init();
    ui_VolumeKnob_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_SongControls);
}
