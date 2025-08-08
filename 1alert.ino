#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>

// ----------- 硬件引脚定义 -----------
#define RED_LED1    27
#define BLUE_LED1   26
#define RED_LED2    25
#define BLUE_LED2   17
#define BUZZER      16
#define TRIG_PIN    18
#define ECHO_PIN    19
#define DHT_PIN     5

// ----------- 其他设置 -----------
#define DHTTYPE DHT22
const int dangerThreshold = 200;  // cm

// ----------- 实例化对象 -----------
DHT dht(DHT_PIN, DHTTYPE);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

// ----------- 状态变量 -----------
float temperature = 0;
float humidity = 0;
float distance = 0;

unsigned long prevToneTime = 0;
unsigned long prevLightTime = 0;
int buzzerFreq = 1000;
bool toneUp = true;
bool toggleLight = false;

void setup() {
  Serial.begin(9600);

  // 设置引脚模式
  pinMode(RED_LED1, OUTPUT);
  pinMode(BLUE_LED1, OUTPUT);
  pinMode(RED_LED2, OUTPUT);
  pinMode(BLUE_LED2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // 初始化模块
  dht.begin();
  u8g2.begin();
}

void loop() {
  measureDistance();
  readTemperatureHumidity();

  bool isDanger = (distance > 0 && distance < dangerThreshold);

  if (isDanger) {
    updateBuzzer();
    updateLights();
  } else {
    stopAlert();
  }

  displayInfo(temperature, humidity, distance, isDanger);
  delay(100);
}

// ========== 读取超声波 ==========
void measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  // 计算声速，单位 m/s
  float v_sound = 331.4 + 0.6 * temperature + 0.0124 * humidity;

  // 距离 = 声速 × 时间 / 2，转换单位为 cm
  distance = (v_sound * (duration / 1000000.0)) / 2.0 * 100.0;

  Serial.print("距离: ");
  Serial.print(distance);
  Serial.println(" cm");
}

// ========== 读取温湿度 ==========
void readTemperatureHumidity() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
}

// ========== 控制蜂鸣器 ==========
void updateBuzzer() {
  unsigned long now = millis();
  if (now - prevToneTime >= 30) {
    prevToneTime = now;

    tone(BUZZER, buzzerFreq);
    if (toneUp) {
      buzzerFreq += 60;
      if (buzzerFreq >= 1900) toneUp = false;
    } else {
      buzzerFreq -= 60;
      if (buzzerFreq <= 1000) toneUp = true;
    }
  }
}

// ========== 控制灯光 ==========
void updateLights() {
  unsigned long now = millis();
  if (now - prevLightTime >= 100) {
    prevLightTime = now;
    toggleLight = !toggleLight;

    if (toggleLight) {
      digitalWrite(RED_LED1, HIGH);
      digitalWrite(BLUE_LED2, HIGH);
      digitalWrite(RED_LED2, LOW);
      digitalWrite(BLUE_LED1, LOW);
    } else {
      digitalWrite(RED_LED1, LOW);
      digitalWrite(BLUE_LED2, LOW);
      digitalWrite(RED_LED2, HIGH);
      digitalWrite(BLUE_LED1, HIGH);
    }
  }
}

// ========== 停止报警 ==========
void stopAlert() {
  noTone(BUZZER);
  digitalWrite(RED_LED1, LOW);
  digitalWrite(BLUE_LED1, LOW);
  digitalWrite(RED_LED2, LOW);
  digitalWrite(BLUE_LED2, LOW);
}

// ========== 显示信息到 OLED ==========
void displayInfo(float temp, float humi, float dist, bool danger) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy12_t_gb2312b); // 支持中文的12号字体
  u8g2.enableUTF8Print(); // 启用UTF-8打印支持

  // y坐标是基线，字体高12px，间距保持16px一行
  char buf[32];

  // 温度
  snprintf(buf, sizeof(buf), "温度: %.1f℃", temp);
  u8g2.drawUTF8(0, 15, buf);

  // 湿度
  snprintf(buf, sizeof(buf), "湿度: %.1f%%", humi);
  u8g2.drawUTF8(0, 30, buf);

  // 距离
  snprintf(buf, sizeof(buf), "距离: %.0fcm", dist);
  u8g2.drawUTF8(0, 45, buf);

  // 状态
  if (danger) {
    u8g2.drawUTF8(0, 60, "状态: 报警");
  } else {
    u8g2.drawUTF8(0, 60, "状态: 正常");
  }

  u8g2.sendBuffer();
}

