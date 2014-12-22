#include <pebble.h>
#include "main.h"

static Window *s_main_window;
static TextLayer *forecast_text_layer;

int main() {
        init();
        app_event_loop();
        deinit();
}

static void main_window_load(Window *window) {      
        window_set_fullscreen(window, false);	

        forecast_text_layer = text_layer_create(GRect(0, 120, 144, 48));
        text_layer_set_background_color(forecast_text_layer, GColorBlack);
        text_layer_set_text_color(forecast_text_layer, GColorWhite);
        text_layer_set_font(forecast_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
        text_layer_set_text_alignment(forecast_text_layer, GTextAlignmentCenter);        

        layer_add_child(window_get_root_layer(window), text_layer_get_layer(forecast_text_layer));
}

static void main_window_unload(Window *window) {
        text_layer_destroy(forecast_text_layer);
}

static void init() {
        // Create main Window element and assign to pointer
        s_main_window = window_create();
        // Set handlers to manage the elements inside the Window
        window_set_window_handlers(s_main_window, (WindowHandlers) {
                .load = main_window_load,
                .unload = main_window_unload
        });
        window_stack_push(s_main_window, true);
        update_weather();
}


static void update_weather() {
        // create buffers for the time, date, weekday
        static char weather_buffer[] = "00:00";

        text_layer_set_text(forecast_text_layer, weather_buffer);
}

static void deinit() {
        window_destroy(s_main_window);
}

