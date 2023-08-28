#include <Arduino.h>
#include <U8g2lib.h>
#include <math.h>

#include "freertos/semphr.h"
#include "freertos/task.h"

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0,
											/* reset=*/U8X8_PIN_NONE,
											/* Clock=*/GPIO_NUM_5,
											/* Data=*/GPIO_NUM_4);

SemaphoreHandle_t bufferSemaph = xSemaphoreCreateBinary();

/* 显示进程, 请勿手动sendBuffer, 画面更新由该进程完成即可 */
void display(void* param) {
	Serial.println("Display Task Create!");
	while (true) {
		if (pdFALSE != xSemaphoreTake(bufferSemaph, 0)) {
			u8g2.sendBuffer();
		}
		xSemaphoreGive(bufferSemaph);
	}
}

/* 渲染进程, 画面的实际内容由该进程控制 */
void renderer(void* param) {}

void setup(void) {
	u8g2.begin();  // 选择U8G2模式
	Serial.begin(115200);

	xTaskCreate(display, "Display", 1024 * 1, NULL, 0, NULL);  // 显示进程, 拥有最低的优先级
}

/*事实上, loop运行在一个优先级为1的任务上*/
void loop(void) {
	u8g2.clearBuffer();	 // 清除内部缓冲区
	u8g2.setFont(u8g2_font_helvR08_tr);
	u8g2.drawStr(16, 9, "Hello World");
	u8g2.setDrawColor(2);
	u8g2.drawBox(0, 0, 128, 10);
	u8g2.setDrawColor(1);
	delay(1500);

	uint16_t len = 8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth();
	uint8_t* p = u8g2.getBufferPtr();
	short disappearTemp = 0;
	for (int loop = 0; loop <= 4; loop++) {
		for (int i = 0; i < len; i++) {
			p[i] = p[i] & (rand() % 0xff) >> disappearTemp;
		}
		disappearTemp += 2;
		delay(50);
	}

	delay(1500);
}