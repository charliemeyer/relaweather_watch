#pragma once

//---------------------------------------------//
//        relaweather watch app                //
//        by: Charlie Meyer                    //
//        December 2014                        //
//---------------------------------------------//
        
//loads all the app elements and UI
static void main_window_load(Window *window);
//unloads all the app elements and UI 
static void main_window_unload(Window *window);
//functions to handle all of the appmessage stuff
static void inbox_received_callback(DictionaryIterator *iterator, void *context);
static void inbox_dropped_callback(AppMessageResult reason, void *context);
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
static void outbox_sent_callback(DictionaryIterator *iterator, void *context);
//the logic to get the updated forecast
static void update_weather();
//button handlers
static void up_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_click_handler(ClickRecognizerRef recognizer, void *context);
static void select_click_handler(ClickRecognizerRef recognizer, void *context);
//put the UI elements on da heap
//make the event handlers for app_event_loop 
//and push the main window to the window stack 
static void init();
//free everything
static void deinit();