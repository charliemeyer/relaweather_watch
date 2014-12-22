#include <pebble.h>
#include "main.h"
#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_week_layer;
static BitmapLayer *s_timeborder_layer;
static GBitmap *s_timeborder_bitmap;
    
int main() {
        init();
        app_event_loop();
        deinit();
}

static void time_handler(struct tm *tick_time, TimeUnits units_changed){
        update_time();
}

static void main_window_load(Window *window) {      
        window_set_fullscreen(window, true);	

        s_timeborder_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TIMEBORDER);
        s_timeborder_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
        bitmap_layer_set_bitmap(s_timeborder_layer, s_timeborder_bitmap);

        s_week_layer = text_layer_create(GRect(0, 8, 144, 48));
        text_layer_set_background_color(s_week_layer, GColorBlack);
        text_layer_set_text_color(s_week_layer, GColorWhite);
        text_layer_set_font(s_week_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
        text_layer_set_text_alignment(s_week_layer, GTextAlignmentCenter);
        
        s_time_layer = text_layer_create(GRect(0, 56, 144, 56));
        text_layer_set_background_color(s_time_layer, GColorClear);
        text_layer_set_text_color(s_time_layer, GColorBlack);
        text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
        text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
        
        s_date_layer = text_layer_create(GRect(0, 120, 144, 48));
        text_layer_set_background_color(s_date_layer, GColorBlack);
        text_layer_set_text_color(s_date_layer, GColorWhite);
        text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
        text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);        

        // Add them as child layers to the Window's root layer
        layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_timeborder_layer));
        layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
        layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
        layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_week_layer));

}

static void main_window_unload(Window *window) {
        text_layer_destroy(s_time_layer);
        text_layer_destroy(s_date_layer);
        text_layer_destroy(s_week_layer);
        bitmap_layer_destroy(s_timeborder_layer);
}

static void init() {
        // Create main Window element and assign to pointer
        s_main_window = window_create();
        tick_timer_service_subscribe(MINUTE_UNIT, time_handler);
        // Set handlers to manage the elements inside the Window
        window_set_window_handlers(s_main_window, (WindowHandlers) {
                .load = main_window_load,
                .unload = main_window_unload
        });
        window_stack_push(s_main_window, true);
        update_time();
}

static void update_time() {
        // get a structure that's the current time
        time_t temp = time(NULL); 
        struct tm *current_time = localtime(&temp);

        // create buffers for the time, date, weekday
        static char time_buffer[] = "00:00";
        static char date_buffer[] = "may 01";
        static char week_buffer[] = "wednesday";

        // write the time to the buffers using the current time
        if(clock_is_24h_style() == true) { 
                strftime(time_buffer, sizeof("00:00"), "%H:%M", current_time);
        } else {
                strftime(time_buffer, sizeof("00:00"), "%I:%M", current_time);
        }
        
        strftime(date_buffer, sizeof("may 01"), "%b %e", current_time);
        strftime(week_buffer, sizeof("wednesday"), "%A", current_time);
        
        if(time_buffer[0] == '0'){
                for(int i = 1; i < 6; i++){ //get rid of that leading 0
                        time_buffer[i - 1] = time_buffer[i];      
                }
        } 
        week_buffer[0] += ('a' - 'A');
        date_buffer[0] += ('a' - 'A');

        text_layer_set_text(s_time_layer, time_buffer);
        text_layer_set_text(s_date_layer, date_buffer);
        text_layer_set_text(s_week_layer, week_buffer);
}

static void deinit() {
        window_destroy(s_main_window);
}
