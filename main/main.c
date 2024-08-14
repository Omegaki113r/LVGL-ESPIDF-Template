#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_log.h>

#include <lvgl.h>
#define HORIZONTAL_RESOLUTION 240
#define VERTICAL_RESOLUTION 320
#define DRAW_BUF_SIZE (HORIZONTAL_RESOLUTION * VERTICAL_RESOLUTION / 10 * (LV_COLOR_DEPTH / 8))
static lv_display_t *display;
static lv_indev_t *input_device;
static uint32_t frame_buffer_1[DRAW_BUF_SIZE / 4];
static uint32_t frame_buffer_2[DRAW_BUF_SIZE / 4];
void display_flush_cb(lv_display_t *, const lv_area_t *, uint8_t *);
void input_read_cb(lv_indev_t *, lv_indev_data_t *);

void app_main(void)
{
    lv_init();

    /*!SECTION Display configuration*/
    display = lv_display_create(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION);
    if (display == NULL)
    {
        ESP_LOGE(__func__, "display create failed");
    }
    lv_display_set_flush_cb(display, display_flush_cb);
    lv_display_set_buffers(display, frame_buffer_1, frame_buffer_2, sizeof(frame_buffer_1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    /*!SECTION Display configuration*/

    /*!SECTION Input device configuration*/
    input_device = lv_indev_create();
    if (input_device == NULL)
    {
        ESP_LOGE(__func__, "input_device create failed");
    }
    lv_indev_set_type(input_device, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(input_device, input_read_cb);
    /*!SECTION Input device configuration*/

    /*!SECTION TEST*/
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello ESPIDF, I'm LVGL!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    /*!SECTION TEST*/

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(20));
        lv_tick_inc(20);
        lv_timer_handler();
    }
}

void display_flush_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map)
{
    uint16_t *buf16 = (uint16_t *)px_map;
    int32_t x, y;
    for (y = area->y1; y <= area->y2; y++)
    {
        for (x = area->x1; x <= area->x2; x++)
        {
            // WRITE TO DISPLAY HERE//
            buf16++;
        }
    }
    lv_display_flush_ready(display);
}

void input_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {}