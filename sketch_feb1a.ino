// // #include <WiFi.h>
// // #include <HTTPClient.h>
// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>
// #include <BluetoothSerial.h>
// #include <DHT.h>

// #define DHTPin  4       //DHT11连接4引脚
// #define DHTType DHT11    //使用DHT11传感器


// // const char* ssid = "李";
// // const char* password = "xflj61225";
// // const char* serverUrl = "YourTencentCloudServerURL";
// // const int serverPort = 80;

// //声明蓝牙服务器、蓝牙特征和连接状态的变量。
// BLEServer *pServer = NULL;
// BLECharacteristic *pTxCharacteristic;
// bool deviceConnected = false;
// bool oldDeviceConnected = false;
// //为处理蓝牙数据定义缓冲区和字符串变量。
// char BLEbuf[32] = {0};//蓝牙数据定义缓冲区
// String data = "";     //字符串变量

// DHT dht(DHTPin,DHTType);

// // 定义收发服务的UUID（唯一标识）
// #define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" 
// #define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"// RX串口标识
// #define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"// TX串口标识

// /*******************************************************************************************************************
// * 功能 ：设置数据帧格式
// * 方法 ：---------------------------------------------
//          |帧 头  |设备号|控制位|数据长度|#数据1#数据2#|
//          |L Z H  |  0  |  0  |   4    |#    #     #|        
//         ----------------------------------------------
//         数据长度 ：2位温度值+2位湿度值=4位
// * 时间 ： 2024年2月10日
// * 作者 ： 李泽豪
// ********************************************************************************************************************/
// char message[32]={'L','Z','H','0','0','4'};

// /*******************************************************************************************************************
// * 功能 ：定义服务器回调类（MyServerCallbacks）以处理连接和断开连接事件
// * 方法 ： onConnect    在设备连接到服务器时被调用。     在这里，它将 deviceConnected 标志设置为 true，表示设备已连接
// * 方法 ： onDisconnect 在设备从服务器断开连接时被调用。 在这里，它将 deviceConnected 标志设置为 false，表示设备已断开连接
// * 时间 ： 2024年2月10日
// * 作者 ： 李泽豪
// ********************************************************************************************************************/
// class MyServerCallbacks: public BLEServerCallbacks {
//   void onConnect(BLEServer* pServer) {
//    deviceConnected = true;
//   };

//   void onDisconnect(BLEServer* pServer) {
//    deviceConnected = false;
//   }
// };

// /******************************************************************************************************************
// * 功能 ：定义特征回调类以处理RX特征上接收到的数据

// * 方法 ：onWrite   在特征值被写入时被调用。         在这里，它获取写入的数据，并将其打印到串行监视器（Serial Monitor）中
// *        pCharacteristic->getValue() 用于获取特征值的数据，返回一个 std::string 类型

// *        将 rxValue 转换为 C 风格字符串（c_str()），并将其存储在全局变量 data 中。这个字符串可以在后续的代码中使用。
// * 时间 ： 2024年2月10日
// * 作者 ： 李泽豪
// *******************************************************************************************************************/
// class MyCallbacks: public BLECharacteristicCallbacks {
//   void onWrite(BLECharacteristic *pCharacteristic) {
//     std::string rxValue = pCharacteristic->getValue();//发送数据
//      if (rxValue.length() > 0) 
//      {
//       Serial.print("Received Value: ");
//       for (int i = 0; i < rxValue.length(); i++)
//       {
//         Serial.print(rxValue[i]);
//       }
//       Serial.println();
//      }
//    }
// };

// void setup(){
//   Serial.begin(115200);
//   BLEDevice::init("Smart diapers");          // 初始化蓝牙设备
//   dht.begin();

//   // 为蓝牙设备创建服务器
//   pServer = BLEDevice::createServer();
//   pServer->setCallbacks(new MyServerCallbacks());             //设置回调
//   BLEService *pService = pServer->createService(SERVICE_UUID);// 基于SERVICE_UUID来创建一个服务

//   //创建特征值，向蓝牙中心设备发送通知。这个特征值的 UUID 是 CHARACTERISTIC_UUID_TX，并且设置了PROPERTY_NOTIFY属性，表示可以通过通知方式发送数据。
//   pTxCharacteristic = pService->createCharacteristic          
//   (
//     CHARACTERISTIC_UUID_TX,
//     BLECharacteristic::PROPERTY_NOTIFY
//   );                               
//   pTxCharacteristic->addDescriptor(new BLE2902());   //为特征值添加一个描述符，使用了 BLE2902 描述符，它是一个通知配置描述符，用于配置特征值是否支持通知。
//   BLECharacteristic * pRxCharacteristic = pService->createCharacteristic//创建另一个特征值，接收从蓝牙中心设备写入的数据。特征值的 UUID 是 CHARACTERISTIC_UUID_RX，并且设置了 PROPERTY_WRITE 属性，表示支持写入。
//   (
//     CHARACTERISTIC_UUID_RX,
//     BLECharacteristic::PROPERTY_WRITE
//   );
//   pRxCharacteristic->setCallbacks(new MyCallbacks());//为写入特征值添加回调函数，以便在有数据写入时执行相应的操作。这里使用了之前定义的 MyCallbacks 类。

//   pService->start();                 // 开启服务
//   pServer->getAdvertising()->start();//开始广播
//   Serial.println("Waiting a client connection to notify...");    
//   Serial.println();
// }

// void loop(){
//    int humidity = dht.readHumidity();          //湿度
//    int temperature = dht.readTemperature();    //温度
//   //  Serial.print("湿度: ");
//   //  Serial.println(humidity);
//   //  Serial.print("温度: ");
//   //  Serial.println(temperature);
//   //  delay(1000);

//   //转字符串测试
//   String Shumidity = String(humidity);
//   String Stemperature = String(temperature);
//   String Sflag = "#";
//   String WJmessage = String(message);
//   WJmessage.concat(Sflag);
//   WJmessage.concat(Stemperature);
//   WJmessage.concat(Sflag);
//   WJmessage.concat(Shumidity); 
//   WJmessage.concat(Sflag);//帧头+设备号+控制位+数据长度+数据
//   Serial.println(WJmessage);
//   delay(1000);

//   if (deviceConnected==1&data.length()>0) 
//    {
//         memset(BLEbuf, 0, 32);             //将 BLEbuf 数组的前32个字节设置为0，这是为了确保我们的数据缓冲区处于初始状态。
//         memcpy(BLEbuf, data.c_str(), 32);  //将从 data 字符串获取的数据复制到 BLEbuf 数组中，我们将字符串数据转移到一个以 null 结尾的字节数组中，以便在后续的 Bluetooth 通信中使用
//         Serial.println(BLEbuf); 
      
//        // 组合字符串并设置为特征值
//        //String message = "光线强度: " + String(lightValue);  //汉字会乱码
//        //String message  = String(lightValue);
//        Serial.print("Before setValue: ");
//        Serial.println(WJmessage);
//        pTxCharacteristic->setValue(WJmessage.c_str());
//        Serial.print("After setValue: ");
//        Serial.println(WJmessage);

//        pTxCharacteristic->notify();
//        data = "";  // 返回数据后进行清空，否则一直发送 data
//     }

//     // 没有新连接时
//     if (!deviceConnected && oldDeviceConnected) {
//         // 给蓝牙堆栈准备数据的时间
//         delay(500); 
//         pServer->startAdvertising(); 
//         // 重新开始广播
//         Serial.println("start advertising");
//         oldDeviceConnected = deviceConnected;
//     }
//     // 正在连接时
//     if (deviceConnected && !oldDeviceConnected) {
//         // 正在连接时进行的操作
//         oldDeviceConnected = deviceConnected;
//    }
// }

// //AHT3001
// /*
//   传感器：SDA(1)  SCL(2)  GND(3)  VCC(4)
//   转接板： 绿       蓝      黄      紫
// */
// // #include <Wire.h>
// // #include <Adafruit_AHTX0.h>

// // Adafruit_AHTX0 aht;

// // void setup() {
// //   Serial.begin(9600);
// //   while (!Serial) {
// //     delay(10);
// //   }

// //   if (!aht.begin()) 
// //   {
// //     while (1){
// //       Serial.println("Could not find AHTX0 sensor, check wiring!");
// //       delay(1000);
// //     }
    
// //   }
// //   Serial.println("AHTX0 sensor found");
// // }

// // void loop() {
// //   sensors_event_t humidity, temp;
// //   aht.getEvent(&humidity, &temp); 
// //   Serial.print(F("Humidity: ")); 
// //   Serial.print(humidity.relative_humidity);
// //   Serial.println(F("%"));
// //   Serial.print(F("Temperature: ")); 
// //   Serial.print(temp.temperature);
// //   Serial.println(F("°C"));
// //   delay(1000);
// // }


// #include <DHT.h>

// #define DHTPIN 4    // DHT11连接到ESP32的GPIO引脚
// #define DHTTYPE DHT11 // 使用DHT11传感器

// DHT dht(DHTPIN, DHTTYPE);

// void setup() {
//   Serial.begin(115200);
//   dht.begin();
// }

// void loop() {
//   delay(2000);  // 每2秒读取一次传感器数据

//   float humidity = dht.readHumidity();
//   float temperature = dht.readTemperature();

//   if (isnan(humidity) || isnan(temperature)) {
//     Serial.println("Failed to read from DHT sensor!");
//     return;
//   }

//   Serial.print("湿度: ");
//   Serial.print(humidity);
//   Serial.print("% 温度: ");
//   Serial.print(temperature);
//   Serial.println("°C");
// }

//温湿度信息发送到手机蓝牙
#include <DHT.h>
#include <BluetoothSerial.h>

#define DHTPIN 4    // DHT11连接到ESP32的GPIO引脚
#define DHTTYPE DHT11 // 使用DHT11传感器



DHT dht(DHTPIN, DHTTYPE);
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT"); // 设置蓝牙设备名称
  dht.begin();
}

void loop() {
  humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.print(humidity);
  Serial.println(temperature);

  SerialBT.print(humidity);
  SerialBT.println(temperature);

  delay(1000);
}
