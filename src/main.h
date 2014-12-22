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
//the logic to get the updated forecast
static void update_weather();
//put the UI elements on da heap
//make the event handlers for app_event_loop 
//and push the main window to the window stack 
static void init();
//free everything
static void deinit();