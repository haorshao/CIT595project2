#include <pebble.h>
static Window *window;
static TextLayer *hello_layer;
static char msg[100];
static int count_select = 0;
static int count_up = 0;
static int count_down = 0;
static bool isStandBy = false;
static bool isStandingBy = false;
static int count_standby = 0;
static bool isAlert = false;
static int alert_count = 0;
// static char msgup[100];

void out_sent_handler(DictionaryIterator *sent, void *context) { 
  // outgoing message was delivered -- do nothing
}
void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
// outgoing message failed
  text_layer_set_text(hello_layer, "Error out!"); 
}

void in_received_handler(DictionaryIterator *received, void *context) {
// incoming message received 
// looks for key #0 in the incoming message 
  
  int key = 0;
//   char* empty = "empty";
  for(key = 0; key <= 23; key++){
    Tuple *text_tuple = dict_find(received, key); 
    if (text_tuple) {
      if (text_tuple->value) {
    // put it in this global variable 
        if(!isStandingBy || (key == 10 || key == 11)){
          strcpy(msg, text_tuple->value->cstring);
          text_layer_set_text(hello_layer, msg);  
        }
      }
    }  
  }
}
void in_dropped_handler(AppMessageResult reason, void *context) {
// incoming message dropped
  text_layer_set_text(hello_layer, "Error in!");
}
  
/* This is called when the select button is clicked */
void select_click_handler(ClickRecognizerRef recognizer, void *context) {
//text_layer_set_text(hello_layer, "Selected!"); 
  
  DictionaryIterator *iter; 
  app_message_outbox_begin(&iter);
  int key = 0;
  if(isStandBy){
//     count_standby = 0;
    key = count_standby % 2 + 10;
    count_standby++;
  }else{
    if(isAlert){
      key = --alert_count % 6 + 18;
    }else{
      key = count_select % 4;//0->curTemp, 1->averageTemp, 2->min, 3->max    
    }
  }
 
  if(key == 11){
    isStandBy = false;
    isStandingBy = false;
  }else if(key == 10){
    isStandingBy = true;
  }
  // send the message "hello?" to the phone, using key #0 
  Tuplet value = TupletCString(key, "temp"); 
  dict_write_tuplet(iter, &value); 
  app_message_outbox_send();
  count_select++;
}

void up_click_handler(ClickRecognizerRef recognizer, void *context){
//   text_layer_set_text(hello_layer, "up");
  DictionaryIterator *iter; 
  app_message_outbox_begin(&iter);
  int key = 0;
  if(!isAlert){
    key = count_up % 2 + 5;//even number -> fah, oddnumber -> cel;  
  }else{
    key = alert_count % 6 + 12;
    alert_count++;
  }
  
  // send the message "hello?" to the phone, using key #0 
  Tuplet value = TupletCString(key, "f or c?"); 
  dict_write_tuplet(iter, &value); 
  app_message_outbox_send();
  count_up++;
}
void down_click_handler(ClickRecognizerRef recognizer, void *context){
  DictionaryIterator *iter; 
  app_message_outbox_begin(&iter);
  int key = count_down % 3 + 7;
  if(!isStandingBy){
    if(key == 7){
      isStandBy = true;
    }else{
      isStandBy = false;
      if(key == 8){
        isAlert = true;
      }else{
        isAlert = false;
      }
    }    
  }

  // send the message "arduino?" to the phone, using key #4 
  Tuplet value = TupletCString(key, "arduino?"); 
  dict_write_tuplet(iter, &value); 
  app_message_outbox_send();
  count_down++;
}
  
/* this registers the appropriate function to the appropriate button */ 
void config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window); GRect bounds = layer_get_bounds(window_layer);
  hello_layer = text_layer_create((GRect) { .origin = { 0, 72 },
  .size = { bounds.size.w, 20 } }); text_layer_set_text(hello_layer, "Pineapplecake!!"); 
  text_layer_set_text_alignment(hello_layer, GTextAlignmentCenter); layer_add_child(window_layer, text_layer_get_layer(hello_layer));
}
static void window_unload(Window *window) { 
  text_layer_destroy(hello_layer);
}
static void init(void) {
  window = window_create(); window_set_window_handlers(window, (WindowHandlers) {
  .load = window_load,
  .unload = window_unload, });
  // need this for adding the listener 
  window_set_click_config_provider(window, config_provider);
    
  // for registering AppMessage handlers 
  app_message_register_inbox_received(in_received_handler); 
  app_message_register_inbox_dropped(in_dropped_handler); 
  app_message_register_outbox_sent(out_sent_handler); 
  app_message_register_outbox_failed(out_failed_handler); 
  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64; 
  app_message_open(inbound_size, outbound_size);  
    
  const bool animated = true;
  window_stack_push(window, animated); 
}
static void deinit(void) { 
  window_destroy(window);
}
int main(void) { 
  init();
  app_event_loop();
  deinit(); 
}

