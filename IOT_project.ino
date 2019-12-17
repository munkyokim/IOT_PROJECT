/**
1. 리모컨 주솟값은 tinkercad기준. 주솟값을 얻는 예제를 이용하여 주솟값을 먼저
  얻은 후에 해당 주솟값으로 소스코드 수정 필요. (주석 참고)
2. analogWrite가 동작하지 않음.
  irrecv.enableIRIn()함수를 빼면 동작하는 것으로 보아 리모컨 때문인 듯.
    digitalWrite을 이용하여 모터 작동까지는 구현하였으나, 세기조절은 구현 X
3. humi value를 integer라고 가정
4. 습도값 (50%, 20%)는 임의로 상황을 가정한 것
5. 인터럽트를 고려하지 않았음. 따라서 의도하지 않은 상황이 발생할 수 있음.
  예를 들면, 리모컨으로 ON을 했는데 이어 습도가 20% 이하여서 바로 꺼지는 등 
6. IR PART까지는 테스트 완료. HUMI PART는 테스트하지 못함.
  tinkercad에 DHT11이 없기때문에 관련 테스트 진행하지 못하고 가상코딩.
  -> 테스트 필요
**/

#include <IRremote.h>
#include <dht11.h>

dht11 DHT11;

int pin_DHT11 = 6;      //dht11 pin
int RECV_PIN = 11;      //recv pin
int PWM1 = 3;           //pwm         
int isacting = 0;       //check the motor is acting

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup () {
  Serial.begin(115200);
  
  pinMode(PWM1,OUTPUT); 
  //pinMode(En,OUTPUT);
  
  irrecv.enableIRIn();
  
  digitalWrite(PWM1, LOW);
}

void loop () {
  //digitalWrite(En, HIGH);
  
  // IR PART START //
  
  if(irrecv.decode(&results)){
    if(results.value == 0xFFC23D){  //maybe change this results value
      Serial.println("PRESSED POWER BUTTON");
      if(isacting == 0){      //motor is not acting -> turn on
        digitalWrite(PWM1, HIGH);
        //analogWrite(PWM1, 128);
        isacting = 1;
        Serial.println("Turn on the motor");
      }
      else{             //motor is acting -> turn off
        digitalWrite(PWM1, LOW);
        //analogWrite(PWM1, 0);
        isacting = 0;
        Serial.println("Turn off the motor");
      }
      delay(5000);
    }
    else{
      Serial.println("PRESSED OTHER BUTTONS");
    }
    irrecv.resume();
  }
  
  // IR PART END //
  
  // HUMI PART START //
  
  int chk = DHT11.read(pin_DHT11);
  int humi;   //assume humi value is integer...
  
  switch (chk){
    //normal operation
    case DHTLIB_OK:
      humi = DHT11.humidity;
      Serial.print("Humidity : ");
      Serial.print(humi);
      Serial.println("%");
      if(humi >= 65){     //습도 60% 이상 -> turn on
          digitalWrite(PWM1, HIGH);
        }
      else if(humi <= 50){  //습도 20% 이하 -> turn off
          digitalWrite(PWM1, LOW);
        }
    break;
    //error message
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Checksum Error");
    break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("Timeout Error");
    break;
    default:
      Serial.println("Unknown Error");
    break;
  }
  
  // HUMI PART END //
  
  delay(1000);
  
  //digitalWrite(En, LOW);
}
