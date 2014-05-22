// Works covered by CC BY-NC-SA 4.0

#include <pebble.h>

Window* window;
TextLayer *time_layer;
TextLayer *hash_layer;
TextLayer *diff_layer;
TextLayer *block_layer;
TextLayer *doge_price_layer;
TextLayer *usdk_price_layer;
TextLayer *block_label_layer;
TextLayer *diff_label_layer;
TextLayer *hash_label_layer;

AppTimer *updateTimer;
static GBitmap *background_img;
static BitmapLayer* bg_layer;

char buffer[] = "00:00";

enum {
  DOGE_NET_HASH_KEY,
  DOGE_NET_DIFF_KEY,
  DOGE_NET_BLOCK_KEY,
  PRICE_DOGE_KEY,
  PRICE_USDK_KEY,
};

static void out_sent_handler(DictionaryIterator *sent, void *context) {
	// outgoing message was delivered
}

void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Failed to send AppMessage to Pebble");
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
	Tuple *net_hash_tuple = dict_find(iter, DOGE_NET_HASH_KEY);
	Tuple *net_diff_tuple = dict_find(iter, DOGE_NET_DIFF_KEY);
	Tuple *net_block_tuple = dict_find(iter, DOGE_NET_BLOCK_KEY);
	Tuple *price_doge_tuple = dict_find(iter, PRICE_DOGE_KEY);
	Tuple *price_usdk_tuple = dict_find(iter, PRICE_USDK_KEY);

	if (net_hash_tuple) {
    text_layer_set_text(hash_layer, net_hash_tuple->value->cstring);
  }
	if (net_diff_tuple) {
		text_layer_set_text(diff_layer, net_diff_tuple->value->cstring);
	}
	if (net_block_tuple) {
		text_layer_set_text(block_layer, net_block_tuple->value->cstring);
	}
	if (price_doge_tuple) {
		text_layer_set_text(doge_price_layer, price_doge_tuple->value->cstring);
	}
	if (price_usdk_tuple) {
		text_layer_set_text(usdk_price_layer, price_usdk_tuple->value->cstring);
	}
}

void in_dropped_handler(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "incoming message from Pebble dropped");
}

void update() {
	app_message_outbox_send();
  updateTimer=app_timer_register(300000, (AppTimerCallback) update, NULL);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  if (clock_is_24h_style()) {
    strftime(buffer, sizeof("00:00"), "%k:%M", tick_time);
  } else {
    strftime(buffer, sizeof("00:00"), "%l:%M", tick_time);
  }
  
      text_layer_set_text(time_layer, buffer);
}


void window_load(Window *window)
{
    //Background Layer
		background_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG);
    bg_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
    bitmap_layer_set_background_color(bg_layer, GColorBlack);
    bitmap_layer_set_bitmap(bg_layer, background_img);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bg_layer));

		//Time layer
    time_layer = text_layer_create(GRect(0, 20, 144, 62));
    text_layer_set_background_color(time_layer, GColorClear);
    text_layer_set_text_color(time_layer, GColorWhite);
    text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
    text_layer_set_font(time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COMIC_SANS_25)));
    layer_add_child(window_get_root_layer(window), (Layer*) time_layer);

		// hash_layer
		hash_layer = text_layer_create(GRect(75, 106, 67, 24));
		text_layer_set_text_color(hash_layer, GColorWhite);
		text_layer_set_background_color(hash_layer, GColorClear);
		text_layer_set_font(hash_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COMIC_SANS_20)));
		text_layer_set_text_alignment(hash_layer, GTextAlignmentRight);

	    // diff_layer
		diff_layer = text_layer_create(GRect(71, 88, 71, 24));
		text_layer_set_text_color(diff_layer, GColorWhite);
		text_layer_set_background_color(diff_layer, GColorClear);
		text_layer_set_font(diff_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COMIC_SANS_20)));
		text_layer_set_text_alignment(diff_layer, GTextAlignmentRight);

		// block_layer
		block_layer = text_layer_create(GRect(65, 70, 77, 24));
		text_layer_set_text_color(block_layer, GColorWhite);
		text_layer_set_background_color(block_layer, GColorClear);
		text_layer_set_font(block_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COMIC_SANS_20)));
		text_layer_set_text_alignment(block_layer, GTextAlignmentRight);

		// doge_price_layer
		doge_price_layer = text_layer_create(GRect(19, 138, 51, 24));
		text_layer_set_text_color(doge_price_layer, GColorWhite);
		text_layer_set_background_color(doge_price_layer, GColorClear);
		text_layer_set_font(doge_price_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COMIC_SANS_20)));
		text_layer_set_text_alignment(doge_price_layer, GTextAlignmentCenter);

		// usdk_price_layer
		usdk_price_layer = text_layer_create(GRect(85, 133, 57, 24));
		text_layer_set_text_color(usdk_price_layer, GColorBlack);
		text_layer_set_background_color(usdk_price_layer, GColorClear);
		text_layer_set_font(usdk_price_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COMIC_SANS_20)));
		text_layer_set_text_alignment(usdk_price_layer, GTextAlignmentCenter);

    layer_add_child(window_get_root_layer(window), (Layer*) time_layer);
		layer_add_child(window_get_root_layer(window), (Layer*) hash_layer);
		layer_add_child(window_get_root_layer(window), (Layer*) diff_layer);
		layer_add_child(window_get_root_layer(window), (Layer*) block_layer);
		layer_add_child(window_get_root_layer(window), (Layer*) doge_price_layer);
		layer_add_child(window_get_root_layer(window), (Layer*) usdk_price_layer);


    //layer for the block# label
    block_label_layer = text_layer_create(GRect(0, 70, 77, 24));
    text_layer_set_background_color(block_label_layer, GColorClear);
    text_layer_set_text_color(block_label_layer, GColorWhite);
    text_layer_set_text_alignment(block_label_layer, GTextAlignmentLeft);
    text_layer_set_font(block_label_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COMIC_SANS_20)));
    layer_add_child(window_get_root_layer(window), (Layer*) block_label_layer);

  
  
    diff_label_layer = text_layer_create(GRect(0, 88, 71, 24));
    text_layer_set_background_color(diff_label_layer, GColorClear);
    text_layer_set_text_color(diff_label_layer, GColorWhite);
    text_layer_set_text_alignment(diff_label_layer, GTextAlignmentLeft);
    text_layer_set_font(diff_label_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COMIC_SANS_20)));
    layer_add_child(window_get_root_layer(window), (Layer*) diff_label_layer);
  
  
    
    hash_label_layer = text_layer_create(GRect(0, 106, 67, 24));
    text_layer_set_background_color(hash_label_layer, GColorClear);
    text_layer_set_text_color(hash_label_layer, GColorWhite);
    text_layer_set_text_alignment(hash_label_layer, GTextAlignmentLeft);
    text_layer_set_font(hash_label_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COMIC_SANS_20)));
    layer_add_child(window_get_root_layer(window), (Layer*) hash_label_layer);
  
    //setting the labels for the numbers. 
    text_layer_set_text(block_label_layer, "Block:");
    text_layer_set_text(diff_label_layer,"Diff:");
    text_layer_set_text(hash_label_layer,"Hash:");


  
  
	//Get a time structure so that the face doesn't start blank
        struct tm *t;
        time_t temp;
        temp = time(NULL);
        t = localtime(&temp);
        
        //Manually call the tick handler when the window is loading
        
        tick_handler(t, MINUTE_UNIT);
        
}

void window_unload(Window *window)
{
    //We will safely destroy the Window's elements here!
    text_layer_destroy(time_layer);
    text_layer_destroy(block_layer);
		text_layer_destroy(diff_layer);
		text_layer_destroy(hash_layer);
		text_layer_destroy(doge_price_layer);
		text_layer_destroy(usdk_price_layer);
    bitmap_layer_destroy(bg_layer);
}

static void app_message_init(void) {
	app_message_open(128 /* inbound_size */, 2 /* outbound_size */);
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_sent(out_sent_handler);
	app_message_register_outbox_failed(out_failed_handler);
}

void init()
{
        app_message_init();
    update();
		//Initialize the app elements here!
        window = window_create();
        window_set_window_handlers(window, (WindowHandlers) {
                .load = window_load,
                .unload = window_unload,
        });

        tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) tick_handler);

		window_stack_push(window, true);
  
  //update the data
  update();
  //psleep(2000);
  updateTimer=app_timer_register(300000,(AppTimerCallback) update, NULL);
}

void deinit()
{
        //De-initialize elements here to save memory!
        tick_timer_service_unsubscribe();

        window_destroy(window);
        app_timer_cancel(updateTimer);
}

int main(void)
{
        init();
        app_event_loop();
        deinit();
}
