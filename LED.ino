#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT

#include <Blinker.h>

char auth[] = "399885328cc2";
char ssid[] = "ABC";
char pswd[] = "1234567890";

bool ledState;
BlinkerButton Button1("btn-abc"); //btn-abc 名称要和app新建组件一致


// app 端按下按键即会执行该函数 回调函数
void button1_callback(const String & state) {
     
     BLINKER_LOG("get button state: ", state);
     if (state=="on") {

        digitalWrite(LED_BUILTIN, LOW);
        // 反馈开关状态
        Button1.print("on");
    } else if(state=="off"){

        digitalWrite(LED_BUILTIN, HIGH);
        // 反馈开关状态
        Button1.print("off");
    }
}
void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}

void miotPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    digitalWrite(LED_BUILTIN, LOW);
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
    ledState = true;
  }
  else if (state == BLINKER_CMD_OFF) {
    digitalWrite(LED_BUILTIN, HIGH);
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
    ledState = false;
  }
}

void miotQuery(int32_t queryCode)
{
  BLINKER_LOG("MIOT Query codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState(ledState ? "on" : "off");
      BlinkerMIOT.print();
      break;

    default :
      BlinkerMIOT.powerState(ledState ? "on" : "off");
      BlinkerMIOT.print();
      break;
  }
}
void setup()
{
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  BLINKER_DEBUG.debugAll();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Blinker.begin(auth, ssid, pswd);

  Blinker.attachData(dataRead);
  Button1.attach(button1_callback); //绑定按键执行回调函数
  BlinkerMIOT.attachPowerState(miotPowerState);
  BlinkerMIOT.attachQuery(miotQuery);
}

void loop()
{
  Blinker.run();
}
