// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: VolumeKnobGUI

#ifndef _VOLUMEKNOBGUI_UI_H
#define _VOLUMEKNOBGUI_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "components/ui_comp.h"
#include "components/ui_comp_hook.h"
#include "ui_events.h"






// SCREEN: ui_SongControls
void ui_SongControls_screen_init(void);
extern lv_obj_t * ui_SongControls;
extern lv_obj_t * ui_MusicArtwork;
extern lv_obj_t * ui_Artist;
extern lv_obj_t * ui_SongName;
extern lv_obj_t * ui_Source;
extern lv_obj_t * ui_LowerPanel;
extern lv_obj_t * ui_SongProgressBar;
void ui_event_ButtonPlayPause(lv_event_t * e);
extern lv_obj_t * ui_ButtonPlayPause;
void ui_event_ButtonBack(lv_event_t * e);
extern lv_obj_t * ui_ButtonBack;
extern lv_obj_t * ui_ButtonShuffle;
void ui_event_ButtonNext(lv_event_t * e);
extern lv_obj_t * ui_ButtonNext;
extern lv_obj_t * ui_ButtonRepeat;
extern lv_obj_t * ui_DisplaySource;
// SCREEN: ui_VolumeKnob
void ui_VolumeKnob_screen_init(void);
extern lv_obj_t * ui_VolumeKnob;
extern lv_obj_t * ui_Arc1;
extern lv_obj_t * ui_VolumeCount;
extern lv_obj_t * ui_Image1;
extern lv_obj_t * ui____initial_actions0;

LV_IMG_DECLARE(ui_img_play_png);    // assets\Play.png
LV_IMG_DECLARE(ui_img_pause_png);    // assets\Pause.png
LV_IMG_DECLARE(ui_img_back_png);    // assets\Back.png
LV_IMG_DECLARE(ui_img_shuffle_png);    // assets\Shuffle.png
LV_IMG_DECLARE(ui_img_236185509);    // assets\Shuffle-On.png
LV_IMG_DECLARE(ui_img_forward_png);    // assets\Forward.png
LV_IMG_DECLARE(ui_img_1640048024);    // assets\Repeat-On-One.png
LV_IMG_DECLARE(ui_img_852668713);    // assets\Repeat-On.png
LV_IMG_DECLARE(ui_img_sourcespotify_png);    // assets\SourceSpotify.png
LV_IMG_DECLARE(ui_img_272761352);    // assets\Dial-1-Full.png
LV_IMG_DECLARE(ui_img_671809700);    // assets\Dial-1-Full-Lit.png
LV_IMG_DECLARE(ui_img_indicator_png);    // assets\Indicator.png
LV_IMG_DECLARE(ui_img_sourcespotifylarge_png);    // assets\SourceSpotifyLarge.png
LV_IMG_DECLARE(ui_img_183454340);    // assets\Dial-1.png

LV_FONT_DECLARE(ui_font_monsterrat_bold_148);
LV_FONT_DECLARE(ui_font_monsterrat_semibold_14);
LV_FONT_DECLARE(ui_font_monsterrat_semibold_18);
LV_FONT_DECLARE(ui_font_monsterrat_semibold_34);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
