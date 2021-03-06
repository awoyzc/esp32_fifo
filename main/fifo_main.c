/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "spiram_fifo.h"
#include "spiram.h"


void fifo_read(void *pvParameters)
{
	for (;;) {
		char buff[256] = { 0 };
		spiRamFifoRead(buff, sizeof(buff));
		printf("------\t");
		printf("spiRamFifoRead\t");
		printf("------\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

	vTaskDelete(NULL);
}
void fifo_write(void *pvParameters)
{
	for (;;) {
		char buff[256] = { 0 };
		spiRamFifoWrite(buff, sizeof(buff));
		printf("++++++\t");
		printf("spiRamFifoWrite\t");
		printf("++++++\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

	vTaskDelete(NULL);
}

void fifo_task(void *pvParameters)
{
	spiRamFifoInit();

	xTaskCreate(&fifo_write, "fifo_write", 256 * 10, NULL, 4, NULL);
	xTaskCreate(&fifo_read, "fifo_read", 256 * 10, NULL, 4, NULL);
	vTaskDelete(NULL);
}

void app_main()
{
    printf("esp32 fifo test!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    fflush(stdout);

	xTaskCreate(&fifo_task, "fifo_task", 256 * 10, NULL, 4, NULL);

}
