#include <pebble.h>
#include "main.h"
//the keys for the app message stuff
#define KEY_LOCATION 0
#define KEY_FORECAST1 1
#define KEY_FORECAST2 2
#define KEY_FORECAST3 3
        
static Window *s_main_window;
static TextLayer *location_text_layer;
static TextLayer *forecast_text_layer;
static char forecast_buffers[3][150];
static int curr_forecast = 1;
static BitmapLayer *horiz_line; 
ActionBarLayer *action_bar;

int main() {
        init();
        app_event_loop();
        deinit();
}

static void main_window_load(Window *window) {      
        window_set_fullscreen(window, false);	

        horiz_line = bitmap_layer_create(GRect(0, 30, 144, 2));
        bitmap_layer_set_background_color(horiz_line, GColorWhite);

        location_text_layer = text_layer_create(GRect(0, 0, 144, 168));
        text_layer_set_background_color(location_text_layer, GColorBlack);
        text_layer_set_text_color(location_text_layer, GColorWhite);
        text_layer_set_font(location_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
        text_layer_set_text_alignment(location_text_layer, GTextAlignmentCenter);   

        forecast_text_layer = text_layer_create(GRect(10, 48, 114, 120));
        text_layer_set_background_color(forecast_text_layer, GColorBlack);
        text_layer_set_text_color(forecast_text_layer, GColorWhite);
        text_layer_set_font(forecast_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
        text_layer_set_text_alignment(forecast_text_layer, GTextAlignmentLeft);        

        action_bar = action_bar_layer_create();
        // Associate the action bar with the window:
        action_bar_layer_add_to_window(action_bar, window);
        // Set the click config provider:
        action_bar_layer_set_click_config_provider(action_bar, click_config_provider);
        
        layer_add_child(window_get_root_layer(window), text_layer_get_layer(location_text_layer));
        layer_add_child(window_get_root_layer(window), text_layer_get_layer(forecast_text_layer));
        layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(horiz_line));
}

static void main_window_unload(Window *window) {
        text_layer_destroy(location_text_layer);
        text_layer_destroy(forecast_text_layer);
        bitmap_layer_destroy(horiz_line);

}

static void init() {
        // Create main Window element and assign to pointer
        s_main_window = window_create();
        // Set handlers to manage the elements inside the Window
        window_set_window_handlers(s_main_window, (WindowHandlers) {
                .load = main_window_load,
                .unload = main_window_unload
        });
        window_set_click_config_provider(s_main_window, click_config_provider);
        window_stack_push(s_main_window, true);
        // set up all of the app message handlers
        app_message_register_inbox_received(inbox_received_callback);
        app_message_register_inbox_dropped(inbox_dropped_callback);
        app_message_register_outbox_failed(outbox_failed_callback);
        app_message_register_outbox_sent(outbox_sent_callback);
        app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
        update_weather();
}

static void deinit() {
        window_destroy(s_main_window);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
        // Read first item
        APP_LOG(APP_LOG_LEVEL_ERROR, "inbox received callback");
        static char location_buffer[32];       
        Tuple *t = dict_read_first(iterator);
        while(t != NULL) {
                switch(t->key) {
                        case KEY_LOCATION:
                                snprintf(location_buffer, sizeof(location_buffer), "%s", t->value->cstring);
                                APP_LOG(APP_LOG_LEVEL_ERROR, "Set location buffer! %s", location_buffer);
                                break;
                        case KEY_FORECAST1:
                                snprintf(forecast_buffers[0], sizeof(forecast_buffers[0]), "%s", t->value->cstring);
                                APP_LOG(APP_LOG_LEVEL_ERROR, "Set forecast buffer! %s", forecast_buffers[0]);
                                break;
                        case KEY_FORECAST2:
                                snprintf(forecast_buffers[1], sizeof(forecast_buffers[1]), "%s", t->value->cstring);
                                APP_LOG(APP_LOG_LEVEL_ERROR, "Set forecast buffer! %s", forecast_buffers[1]);
                                break;
                        case KEY_FORECAST3:
                                snprintf(forecast_buffers[2], sizeof(forecast_buffers[2]), "%s", t->value->cstring);
                                APP_LOG(APP_LOG_LEVEL_ERROR, "Set forecast buffer! %s", forecast_buffers[2]);
                                break;                
                        default:
                                APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
                                break;
                }
                t = dict_read_next(iterator);
        }
        curr_forecast = 1;
        text_layer_set_text(forecast_text_layer, forecast_buffers[0]);
        text_layer_set_text_alignment(forecast_text_layer, GTextAlignmentLeft);
        int location_len = strlen(location_buffer);
        if(location_len > 15){
                location_buffer[11] = '.';
                location_buffer[12] = ',';
                location_buffer[13] = ' ';
                location_buffer[14] = location_buffer[location_len - 2];
                location_buffer[15] = location_buffer[location_len - 1];
                location_buffer[16] = '\0';
        }
        text_layer_set_text(location_text_layer, location_buffer);
}

static void click_config_provider(void *context)
{
        window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
        window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
        window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context)
{
        switch(curr_forecast){
                case KEY_FORECAST1:
                        return;
                        break;
                case KEY_FORECAST2:
                        text_layer_set_text(forecast_text_layer, forecast_buffers[0]);
                        curr_forecast--;
                        break;
                case KEY_FORECAST3:
                        text_layer_set_text(forecast_text_layer, forecast_buffers[1]);
                        curr_forecast--;
                        break;
        }          
}
 
static void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
        switch(curr_forecast){
                case KEY_FORECAST1:
                        text_layer_set_text(forecast_text_layer, forecast_buffers[1]);
                        curr_forecast++;
                        break;
                case KEY_FORECAST2:
                        text_layer_set_text(forecast_text_layer, forecast_buffers[2]);
                        curr_forecast++;
                        break;
                case KEY_FORECAST3:
                        return;
                        break;
        }       
}
 
static void select_click_handler(ClickRecognizerRef recognizer, void *context)
{
        text_layer_set_text_alignment(forecast_text_layer, GTextAlignmentCenter);
        text_layer_set_text(forecast_text_layer, "refreshing...");
        DictionaryIterator *iter;       
        app_message_outbox_begin(&iter);
        // Add a key-value pair
        dict_write_uint8(iter, 0, 0);
        // Send the message!
        app_message_outbox_send();
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
        APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void update_weather() {
        text_layer_set_text_alignment(forecast_text_layer, GTextAlignmentCenter);
        static char weather_buffer[] = "Loading...";
        text_layer_set_text(forecast_text_layer, weather_buffer);
}
