#include <Servo.h>    //servo kütüphanesi programa dahil ediliyor

Servo servo_motor;    //servo_motor isimli nesne oluşturuldu

//değişkenler oluşturuluyor ve ilk değer atamaları yapılıyor
int sure = 0;   
int mesafe = 0;
int hiz=250;

//Motor sürücü için pin tanımlamaları
int sag_enable = 11;    // ENABLE A 11 nolu Arduino pinine
int sag_ileri = 10;     // INPUT 1
int sag_geri = 9;       // INPUT 2
int sol_ileri = 8;      // INPUT 3
int sol_geri = 7;       // INPUT 4
int sol_enable = 6;     // ENABLE B

int echo_pin = 4;       //echo_pini 4 nolu pine
int trig_pin = 3;       //trig pini 3 nolu pine
int alev_sensor=2;      //alev sensörü 2 nolu pine
int role=12;            //role 12 nolu pine

void setup(){
  pinMode(trig_pin, OUTPUT);    //trig pini çıkış yapıldı
  pinMode(echo_pin, INPUT );    //echo pini giriş yapıldı
  pinMode(sag_ileri, OUTPUT);   //pinler çıkış olarak tanımlandı
  pinMode(sag_geri, OUTPUT);
  pinMode(sol_ileri, OUTPUT);
  pinMode(sol_geri, OUTPUT);
  pinMode(sag_enable, OUTPUT);
  pinMode(sol_enable, OUTPUT);
  pinMode(alev_sensor, INPUT);  //alev_sensor pini giriş yapıldı
  pinMode(role, OUTPUT);        //role pini çıkış yapıldı
  
  digitalWrite(role,HIGH);      //ilk olarak role konumu pasif 
  
  servo_motor.attach(5);        //servo motor sinyal pini 5 nolu pine
  servo_motor.write(90);        //servo motor ilk açısı 90 derece
      
  Serial.begin(9600);           //seri haberleşme ayarları başlatıldı
     
}

void loop(){
  //Mesafe ölçümü yapılıyor
  digitalWrite(trig_pin, HIGH);         
  delayMicroseconds(10);   
  digitalWrite(trig_pin, LOW);
  sure = pulseIn(echo_pin, HIGH);
  mesafe = (sure / 2) / 29.1;
  Serial.println(mesafe);       //mesafe seri port ekranına yazdırılıyor

  //alev sensör pini okunup değer alev_durum isimli değişkene aktarılıyor
  int alev_durum=digitalRead(alev_sensor);    

  if (mesafe < 15) {    //mesafe 15 cm den küçükse
    //motorlar kitlenerek araç durdurulur
    digitalWrite(sag_ileri, LOW); 
    digitalWrite(sag_geri, LOW);
    digitalWrite(sol_ileri, LOW);
    digitalWrite(sol_geri, LOW);
    delay(1000);
    
    // araç geriye doğru hareket ettirilir
    digitalWrite(sag_ileri, LOW);
    digitalWrite(sag_geri, HIGH);
    digitalWrite(sol_ileri, LOW);
    digitalWrite(sol_geri, HIGH);
    analogWrite(sag_enable, hiz); 
    analogWrite(sol_enable, hiz);
    delay(300);

    // motorlar kitlenerek araç durdurulur
    digitalWrite(sag_ileri, LOW); 
    digitalWrite(sag_geri, LOW);
    digitalWrite(sol_ileri, LOW);
    digitalWrite(sol_geri, LOW);
    delay(1000);
    
    //araç sola döndürülür
    digitalWrite(sag_ileri, HIGH);
    digitalWrite(sag_geri, LOW);
    digitalWrite(sol_ileri, LOW);
    digitalWrite(sol_geri, HIGH);
    analogWrite(sag_enable, hiz); 
    analogWrite(sol_enable, hiz);
    delay(300);


  }

  if(mesafe >= 15 && alev_durum==1){  //mesafe 15 cm eşit veya büyükse ve alev_durum değişkeni 1 ise
    //araç ileri gitsin
    digitalWrite(sag_ileri, HIGH);
    digitalWrite(sag_geri, LOW);
    digitalWrite(sol_ileri, HIGH);
    digitalWrite(sol_geri, LOW);
    analogWrite(sag_enable, hiz); 
    analogWrite(sol_enable, hiz);  
  }

  if(alev_durum==0){  //alev_durum değişkeni 0 ise yangın algılanmışsa
    //araç dursun
    digitalWrite(sag_ileri, LOW); 
    digitalWrite(sag_geri, LOW);
    digitalWrite(sol_ileri, LOW);
    digitalWrite(sol_geri, LOW);
    delay(1000);

    digitalWrite(role, LOW);    //role aktif olsun ve su motoru çalışsın
    
    for(int i=0; i<=180; i++){    //servo motor 0 dan 180 dereceye doğru dönsün
      servo_motor.write(i);
      delay(10);
    }

    for(int i=180; i>=0; i--){  //servo motor 180 den 0 dereceye doğru dönsün
      servo_motor.write(i);
      delay(10);
    }

    digitalWrite(role, HIGH);   //role pasif edilsin su motoru dursun
    servo_motor.write(90);
    
  }

}
