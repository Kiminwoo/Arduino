#include "DHT.h"
#include <PMsensor.h>
#include <SoftwareSerial.h>

// 필요한 라이브러리들을 include한 부분 ---------------------------------------


//#define sensitivity  0.1  //먼지 센서의 민감도 수치
// sensitivity 의 값을 지워준 이유는 전압값을 구하고 싶은 것이 아니라 , 실질적인 초미세먼지의 값을 구하고 싶었다.


#define DHTPIN 13  // 온습도센서 핀 번호
#define DHTTYPE DHT11 // DHTTYPE 온습도 라이브러리 선언


float Vo_value =0;
float dustDensity =0;
// --------------- 차례대로 , 전압값 변수 , 실제 먼지값 


const int RX = 3;
const int TX = 2;
SoftwareSerial BTSerial(TX,RX);
// ------------ const를 선엄함으로써 정확한 정수 선언 , 그리고 0,1 통신이 아니라 블루투스 통신을 위해 2,3으로 주었다. 
// ------------ 스케치 업로드를 할 때 , 오류가 났었기 때문에 이렇게 해 준 것이다.
// ------------ BTSerial을 선언할때 , (TX,RX) 순으로 선언해야 한다.


PMsensor PM;
// 차례대로 , PM , dht 라이브러리 선언인데 , 이 부분은 선언은 해주었지만 실질적으로 사용되진 않는다 .
// 왜냐하면 , 처음에는 sensitivity를 사용함으로써 , Data 값이 잘 왔나 확인해 주는 작업을 해 주었는데 , 
// 어느정도 프로젝트를 진행함으로써 코드가 완성되어갔다.
// 마지막 부분에 미세먼지 실질적인 값을 구할 때 , 이 부분을 사용해서 예외처리를 할 때 정확한 값이 나오지 않았기 때문이다.

DHT dht(DHTPIN, DHTTYPE);




void setup() 
{
  Serial.begin(9600);
  BTSerial.begin(9600);
}
// 위 부분은 Serial 통신과 , Software통신을 9600으로 선언한다는 얘기



void loop() {
   
  int temp = dht.readTemperature();
  int humi = dht.readHumidity();
  // 위 부분은 dht : 온습도 센서에서 데이터 값을 가져오는 부분이다 . 이 부분을 int로써 소숫점 자리를 없애 주었다.



  
  float Vo_value = analogRead(10); 
  // 바로 미세먼지양을 구하는것이 아니라 , 전압값을 구한다.
  // 이때 나타나는 값은 아날로그 핀을 거쳐서 나오는 전압을 0 ~ 1023을 나타냄.
  // 고로 진짜 미세먼지 값이 아님 , 진짜 V(전압)값을 구해줘야함
  // 10번 핀에 미세먼지 out이 연결되어 있다.
  
  dustDensity =  0.17*(Vo_value*0.0049)-0.1;
  // 실질 적인 미세먼지값 
   

  
  if (isnan(humi) || isnan(temp) || isnan(dustDensity))  
  {
    Serial.println("Failed to read Data!");
    return;
  }

  // 위 부분은 온습도와 미세먼지의 값을 제대로 가져왔는지 판별하는 예외처리 부분 
  

  /*float data = 0;
  int err = PMsensorErrSuccess;
  if ((err = PM.read(&data, true, sensitivity)) != PMsensorErrSuccess) {
    Serial.print("data Error = ");
    Serial.println(err);
    delay(4000);
    return;
  }
  */
  // 서두에서 말했던 , Sensor확인 여부 부분인데 , 이상하게 이 부분을 돌려주게 되면 , 미세먼지의 값이 전압값도 아닌, 이상한 값으로 출력
  // sensitivity 부분을 제외해 주면 되긴 하는데 , 이상하게 아두이노에서는 값이 계속 달라집니다. 

  Serial.print("온도:");
  Serial.println(temp);
  Serial.print("습도:");
  Serial.println(humi);
  Serial.print("미세먼지농도:");
  Serial.println(dustDensity);
  //위 부분은 Serial 통신에서 출력되는 값을 의미한다 . 

  
  BTSerial.print("thingspeak:key=WAYZ1YH21AM0P3BN&field1=");
  BTSerial.println(temp);
  BTSerial.println("&field2=");
  BTSerial.println(humi);
  BTSerial.println("&field3=");
  BTSerial.println(dustDensity);
  BTSerial.println("[*]");
  //위 부분은 Software 통신부분인데 , thingspeak에 값을 보내줍니다.
  
  delay(4000);
  // 널널하게 4초를 주었습니다.  
}
