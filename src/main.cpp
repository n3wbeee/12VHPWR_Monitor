#include <Arduino.h>
#include <U8g2lib.h>
#include <math.h>
#include <map>

#include "freertos/task.h"

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0,
											/* reset=*/U8X8_PIN_NONE,
											/* Clock=*/GPIO_NUM_5,
											/* Data=*/GPIO_NUM_4);

typedef void (*funcPtr)();
std::map<String, funcPtr> pageMap;
String pageNow;	 // 不能volatile?

void homePage();
void settingPage();
void renderer(void* param);

void setup(void) {
	u8g2.begin();  // 选择U8G2模式
	Serial.begin(115200);

	// !!! 请在此处注册页面
	pageMap["home"] = homePage;
	pageMap["setting"] = settingPage;

	pageNow = "home";

	xTaskCreate(renderer, "Renderer", 1024 * 1, NULL, 0, NULL);	 // 显示进程, 拥有最低的优先级
}

/* 事实上, loop运行在一个优先级为1的任务上 */
void loop(void) {
	delay(3000);
	pageNow = "setting";
	delay(3000);
	pageNow = "home";
}

/* 页面绘制实现, 请莫在此处sendBuffer */
void homePage() {
	u8g2.clearBuffer();
	u8g2.setFont(u8g2_font_helvR08_tr);
	u8g2.drawStr(16, 9, "Hello World");
	u8g2.setDrawColor(2);
	u8g2.drawBox(0, 0, 128, 10);
	u8g2.setDrawColor(1);
};

void settingPage() {
	u8g2.clearBuffer();
	u8g2.setFont(u8g2_font_helvR08_tr);
	u8g2.drawStr(16, 24, "Hello World");
	u8g2.setDrawColor(2);
	u8g2.drawBox(0, 0, 128, 32);
	u8g2.setDrawColor(1);
};

/*渲染实现*/
void renderer(void* param) {
	Serial.println("Display Task Create!");
	String prevPage = pageNow;	// 用于判断页面是否发生变化
	while (true) {
		if (pageNow == prevPage) {	// 页面未发生变化
			prevPage = pageNow;
			pageMap[pageNow]();	 // 调用页面绘制函数
			u8g2.sendBuffer();
		} else {
			prevPage = pageNow;
			uint16_t len = 8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth();
			uint8_t* p = u8g2.getBufferPtr();
			short animeTemp = 0;
			/* 神秘的退出动画代码 */
			for (int loop = 0; loop <= 4; loop++) {
				for (int i = 0; i < len; i++) {
					p[i] = p[i] & (rand() % 0xff) >> animeTemp;
				}
				animeTemp += 2;
				u8g2.sendBuffer();
				// delay(10);
			}
			/* 神秘的进入动画代码 */
			animeTemp = 8;
			for (int loop = 0; loop <= 4; loop++) {
				pageMap[pageNow]();	 // 调用页面绘制函数
				for (int i = 0; i < len; i++) {
					p[i] = p[i] & (rand() % 0xff) >> animeTemp;
				}
				animeTemp -= 2;
				u8g2.sendBuffer();
				// delay(10);
			}
		}
	}
}