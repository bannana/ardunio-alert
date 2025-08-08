# ardunio-alert
the first ardunio learning repository of mine
📌 项目简介（中文）：
本项目基于 Arduino 平台，通过整合 DHT22 温湿度传感器、超声波测距模块和 0.96 寸 I2C OLED 显示屏，实时监测环境温度、湿度和前方障碍物的距离，并以中英文在屏幕上动态显示相关数据。同时，系统根据温度自动计算音速，以提高距离测量的精确度。此外，当检测到物体距离过近时，系统将触发报警状态，并在屏幕上显示“报警”或“正常”字样。该项目展示了传感器数据采集、数据处理、动态显示及人机交互的整合能力。

📌 Project Overview (English):
This project is built on the Arduino platform, integrating a DHT22 temperature & humidity sensor, an ultrasonic distance sensor, and a 0.96-inch I2C OLED display. It monitors ambient temperature, humidity, and the distance to obstacles in real-time, dynamically displaying data on-screen in both Chinese and English. The system also calculates the speed of sound based on real-time temperature to improve distance measurement accuracy. An alarm status is triggered when an object is detected too close, displaying "报警" (Alarm) or "正常" (Normal) accordingly. This project showcases skills in sensor integration, data processing, dynamic visualization, and basic human-machine interaction.

使用了esp32-wroom32-ea开发板
esp32-wroom32-ea was used in this project.
