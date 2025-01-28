/*
 * Project ZX2D10GE01R-V4848 for Arduino
 * Description: The base project that can be used in the Arduino environment using the ZX2D10GE01R-V4848 device
 * Author: Eric Nam
 * Date: 03-18-2023
 */

// ** Prerequisites **
// ESP32 Arduino 2.0.7 based on ESP-IDF 4.4.4
// https://github.com/espressif/arduino-esp32

// LVGL version 8.3.5
// https://github.com/lvgl/lvgl

// GFX Library for Arduino 1.3.2
// https://github.com/moononournation/Arduino_GFX

// ZX2D10GE01R-V4848 for ESP-IDF
// https://github.com/wireless-tag-com/ZX2D10GE01R-V4848

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino_GFX_Library.h>
#include "button.hpp"
#include "mt8901.hpp"
#include <WiFi.h>
#include <HARestAPI.h>

#include "ui.h"
//Home assistant connector libraries
#include <ArduinoHA.h>
//#include <MQTTPubSubClient.h>

//OTA libraries
#include <ElegantOTA.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <Button2.h>
#include <ESP32Encoder.h>

#define ECO_O(y) (y > 0) ? -1 : 1
#define ECO_STEP(x) x ? ECO_O(x) : 0
#define GFX_BL 38
//static uint8_t cnt_5 = -1;

//variables for adjusting the volume
String entity = "media_player.echo_studio"; //device to control
int btn_hold_time = 0;
bool is_button_held = false;
bool toggle_volume_mode = false;
int previousEncoderValue = 0; // Initialize the previous encoder value

int degreesPerStep = 7; // Set the degrees for each step


WiFiClient client;
//MQTTPubSubClient mqtt;
HARestAPI ha(client);

//Details from Home Assistant


const char* ha_ip = "homeassistant.local";
uint16_t ha_port = 8123; // Could be 443 is using SSL
const char* ha_pwd = "xxxxxxx";  //long-lived password. On HA, Profile > Long-Lived Access Tokens > Create Token



HADevice device("myVolumeKnob");
//HAMqtt mqtt(client, device);

//CONNECT TO WIFI
const char* ssid = "xxxxx";
const char* password = "xxxxxx";

// Static IP configuration
IPAddress staticIP(192, 168, 1, 100); // ESP32 static IP
IPAddress gateway(192, 168, 1, 254);    // IP Address of your network gateway (router)
IPAddress subnet(255, 255, 255, 0);   // Subnet mask


////OTA Server/////
WebServer server(80);
unsigned long ota_progress_millis = 0;

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}


/////////


Arduino_DataBus *bus = new Arduino_SWSPI(
  GFX_NOT_DEFINED /* DC */, 21 /* CS */,
  47 /* SCK */, 41 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
  39 /* DE */, 48 /* VSYNC */, 40 /* HSYNC */, 45 /* PCLK */,
  10 /* R0 */, 16 /* R1 */, 9 /* R2 */, 15 /* R3 */, 46 /* R4 */,
  8 /* G0 */, 13 /* G1 */, 18 /* G2 */, 12 /* G3 */, 11 /* G4 */, 17 /* G5 */,
  47 /* B0 */, 41 /* B1 */, 0 /* B2 */, 42 /* B3 */, 14 /* B4 */,
  1 /* hsync_polarity */, 10 /* hsync_front_porch */, 10 /* hsync_pulse_width */, 10 /* hsync_back_porch */,
  1 /* vsync_polarity */, 14 /* vsync_front_porch */, 2 /* vsync_pulse_width */, 12 /* vsync_back_porch */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
  480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
  bus, GFX_NOT_DEFINED /* RST */, st7701_type7_init_operations, sizeof(st7701_type7_init_operations));

static button_t *g_btn;
//_lv_obj_t *lv_g_btn = (_lv_obj_t *)g_btn;

static const uint16_t screenWidth  = 480;
static const uint16_t screenHeight = 480;
//static uint32_t screenWidth; //const & added =480 for ui
//static uint32_t screenHeight; //const &added =480 for ui
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10 ]; // changed for ui originally was *disp_draw_buf;
//static lv_color_t *disp_draw_buf;
static lv_group_t *lv_group;
//static lv_obj_t *lv_btn_1;
//static lv_obj_t *lv_btn_2;
static lv_obj_t *lv_btn_3;
static lv_obj_t *lv_btn_4;
static lv_obj_t *lv_btn_5;


TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance added for ui */


//thisif statement section added for ui
/* Serial debugging */
#if LV_USE_LOG != 0  
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif


// "myButtonA" and "myButtonB" are unique IDs of buttons. You should define you own ID.
//HAButton ButtonA("lv_btn_1"); //probably can delete
//HAButton ButtonB("lv_btn_2"); //probably can delete


/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

//in the original code
#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

//added for ui code
  tft.startWrite();
  tft.setAddrWindow( area->x1, area->y1, w, h );
  tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
  tft.endWrite();

//in the original code
  lv_disp_flush_ready(disp);
}

void encoder_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
  static int16_t cont_last = 0;
  int16_t cont_now = mt8901_get_count();
  data->enc_diff = ECO_STEP(cont_now - cont_last);
  cont_last = cont_now;
  if (button_isPressed(g_btn)) {
    data->state = LV_INDEV_STATE_PR;
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void setup() {
  Serial.begin(115200);
  device.enableSharedAvailability();
  device.enableLastWill();

  //WIFI CODE////
  WiFi.begin(ssid, password);
  Serial.print("connecting to wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" connected!");


  if(!WiFi.config(staticIP, gateway, subnet)) {
    Serial.println("Failed to configure Static IP");
  } else {
    Serial.println("Static IP configured!");
  }

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  //////

  ///OTA Server
  server.on("/", []() {
  server.send(200, "text/plain", "Hi! I am ESP32.");
  });

  ElegantOTA.begin(&server);    // Start ElegantOTA
  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);
/////////////

  server.begin();
  Serial.println("HTTP server started");


  //ha server
  ha.setHAServer(ha_ip, ha_port);
  ha.setHAPassword(ha_pwd);


  //THAT PROJECT CODE
  gfx->begin();
  gfx->fillScreen(BLACK);

  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);

//added for ui
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println( LVGL_Arduino );
  Serial.println( "I am LVGL_Arduino" );

//original code
  lv_init();

//added for ui
#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

 tft.begin();          /* TFT init */
    tft.setRotation( 3 ); /* Landscape orientation, flipped */

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );
    Serial.println( "tft Initialised" );

//original code

  // button pins
  g_btn = button_attch(3, 0, 10); 


  // Magnetic Encoder
  mt8901_init(5, 6);

//in original code
  //screenWidth = gfx->width();
  //screenHeight = gfx->height();

  // Must use PSRAM In original code
//  disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 32, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

//  if (!disp_draw_buf) {
//    Serial.println("LVGL disp_draw_buf allocate failed!");
//  } else {
//    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * 32);

    /* Initialize the display */
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    Serial.println( "Display Initialised" );

    /* Initialize the input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = encoder_read;
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    lv_indev_drv_register(&indev_drv);
    Serial.println( "Input Driver Initialised" );

    init_lv_group();
    Serial.println( "lv_group Initialised" );

    //ui code
    ui_init();
    Serial.println( "UI initialised" );


    //ui_SongControls_screen_init();
    //Serial.println( "Song control screen initialised" );

//    lv_example_get_started_1();
//    lv_image_btn_events();
//    Serial.println( "Buttons Initialised" );


  //}

    //OTA
    ElegantOTA.begin(&server);
    
}

void loop() {
  lv_timer_handler(); /* let the GUI do its work */

  if (toggle_volume_mode == true) {
    int encoderChange = mt8901_get_count();
      // Check if the change in encoder value corresponds to a 7-degree increment
    if ((encoderChange - previousEncoderValue) >= degreesPerStep) {
        ha.sendCustomHAData("/api/services/media_player/volume_down", "{\"entity_id\":\"" + entity + "\"}");
        // Update the previous encoder value
        previousEncoderValue = previousEncoderValue + 7;
    }
    if ((encoderChange - previousEncoderValue) <= -degreesPerStep) {
        ha.sendCustomHAData("/api/services/media_player/volume_up", "{\"entity_id\":\"" + entity + "\"}");
        // Update the previous encoder value
        previousEncoderValue = previousEncoderValue - 7;
    }
      // Add a delay to control the frequency of readings (adjust as needed)
     delay(100); // Delay for 100 milliseconds before the next check
  }  


  server.handleClient();
  ElegantOTA.loop();
  delay(5);
}

void init_lv_group() {
  lv_group = lv_group_create();
  lv_group_set_default(lv_group);

  lv_indev_t *cur_drv = NULL;
  for (;;) {
    cur_drv = lv_indev_get_next(cur_drv);
    if (!cur_drv) {
      break;
    }

    if (cur_drv->driver->type == LV_INDEV_TYPE_ENCODER) {
      lv_indev_set_group(cur_drv, lv_group);
    }
  }
}


// Function to handle button press -now in external file
//static void btn_event_cb(lv_event_t * e) {
 // }


/**
 * Create a button with a label and react on click event.
 */
//void lv_image_btn_events(void) {

 // lv_obj_add_event_cb(ui_ButtonPlayPause, btn_event_cb, LV_EVENT_ALL, NULL);
 // lv_obj_add_event_cb(ui_ButtonBack, btn_event_cb, LV_EVENT_ALL, NULL);
 // lv_obj_add_event_cb(ui_ButtonNext, btn_event_cb, LV_EVENT_ALL, NULL);

//}
//void lv_example_get_started_1(void) {

  //BUTTON THREE
//  lv_btn_3 = lv_btn_create(lv_scr_act()); /*Add a button the current screen*/
//  lv_obj_set_style_bg_color(lv_btn_3, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN); /*Set its main color*/

//  lv_obj_set_size(lv_btn_3, 80, 80); /*Set its size*/
//  lv_obj_align(lv_btn_3, LV_ALIGN_CENTER, -80, 0); /*Set its position*/
  //lv_obj_add_event_cb(ui_ButtonBack, btn_event_cb, LV_EVENT_ALL, NULL); /*Assign a callback to the button*/ //previously called lv_btn_3

//  lv_obj_t * label3 = lv_label_create(lv_btn_3); /*Add a label to the button*/
//  lv_label_set_text(label3, "Previous"); /*Set the labels text*/
//  lv_obj_center(label3);

  //BUTTON FOUR
//  lv_btn_4 = lv_btn_create(lv_scr_act()); /*Add a button the current screen*/
//  lv_obj_set_style_bg_color(lv_btn_4, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN); /*Set its main color*/

//  lv_obj_set_size(lv_btn_4, 80, 80); /*Set its size*/
//  lv_obj_align(lv_btn_4, LV_ALIGN_CENTER, 80, 0); /*Set its position*/
 // lv_obj_add_event_cb(ui_ButtonPlayPause, btn_event_cb, LV_EVENT_ALL, NULL); /*Assign a callback to the button*///previously called lv_btn_4

//  lv_obj_t * label4 = lv_label_create(lv_btn_4); /*Add a label to the button*/
//  lv_label_set_text(label4, "Next"); /*Set the labels text*/
//  lv_obj_center(label4);

  //BUTTON FIVE
//  lv_btn_5 = lv_btn_create(lv_scr_act()); /*Add a button the current screen*/
//  lv_obj_set_style_bg_color(lv_btn_5, lv_palette_main(LV_PALETTE_DEEP_ORANGE), LV_PART_MAIN); /*Set its main color*/

//  lv_obj_set_size(lv_btn_5, 80, 80); /*Set its size*/
//  lv_obj_align(lv_btn_5, LV_ALIGN_CENTER, 0, 0); /*Set its position*/
//  lv_obj_add_event_cb(lv_btn_5, btn_event_cb, LV_EVENT_ALL, NULL); /*Assign a callback to the button*/

//  lv_obj_t * label5 = lv_label_create(lv_btn_5); /*Add a label to the button*/
//  lv_label_set_text(label5, "Play/Pause"); /*Set the labels text*/
//  lv_obj_center(label5);

//}


void btn_event_cb(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * btn = lv_event_get_target(e);






  if (code == LV_EVENT_PRESSED) {
    btn_hold_time = millis(); // Record the time when the button is pressed
    Serial.print("Button hold time: ");
    Serial.println(btn_hold_time);
    is_button_held = false; // Reset the flag for held status
  } else if (code == LV_EVENT_RELEASED) {
    uint32_t release_time = millis();
    if ((release_time - btn_hold_time) >= 1200) { // Check if the button was held for at least 1.2 seconds
      is_button_held = true; // Set the flag indicating the button was held
      if(toggle_volume_mode == false){
          toggle_volume_mode = true;
          previousEncoderValue = mt8901_get_count();
          }
      else {
          toggle_volume_mode = false;
          } 
      
      // Toggle the mode
      Serial.print("Volume mode is: ");
      Serial.println(toggle_volume_mode);
    } 
  //Serial.print("Button held is: ");
  //Serial.println(is_button_held);
  //if (is_button_held) {
  // Change rotary encoder behavior based on toggle_volume_mode
  
    // Change volume
    // Handle volume change based on encoder movement
  } if (toggle_volume_mode == false){
      //lv_scr_load(ui_SongControls);
      //exit_loop = true; // Set the flag to exit the loop
      if (code == LV_EVENT_CLICKED && is_button_held == false) {
        // Control play/pause or track selection

        if (btn == ui_ButtonBack) { //previously called lv_btn_3
          //Serial.println("lv_btn_3 pressed\n"); // Print when lv_btn_3 is pressed
        //static uint8_t cnt_3 = 0;
          ha.sendCustomHAData("/api/services/media_player/media_previous_track", "{\"entity_id\":\"" + entity + "\"}");

        } else if (btn == ui_ButtonNext) { //previously called lv_btn_4
        //static uint8_t cnt_4 = 0;
          ha.sendCustomHAData("/api/services/media_player/media_next_track", "{\"entity_id\":\"" + entity + "\"}");
        //  Serial.println("lv_btn_4 pressed\n"); // Print when lv_btn_4 is pressed

        } else if (btn == ui_ButtonPlayPause) {
          ha.sendCustomHAData("/api/services/media_player/media_play_pause", "{\"entity_id\":\"" + entity + "\"}");
          //ha.sendCustomHAData("/api/services/media_player/media_play_pause", "{\"entity_id\":\"media_player.adam_s_echo_studio\"}");
        //  Serial.println("lv_btn_5 pressed\n"); // Print when lv_btn_5 is pressed
       }
     }
  }
 // else if (toggle_volume_mode == true) {    
 //    lv_scr_load(ui_VolumeKnob);
}
