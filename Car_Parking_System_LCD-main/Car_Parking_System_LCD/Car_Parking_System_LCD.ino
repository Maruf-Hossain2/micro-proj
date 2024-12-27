#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo myservo;

#define ir_enter 2
#define ir_back  4
#define ir_car1 5
#define ir_car2 6
#define ir_car3 7
#define ir_car4 8

int S1=0, S2=0, S3=0, S4=0;
int flag1=0, flag2=0;
int slot = 4;  

// Separate timer variables for each slot
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;
const long interval = 2000; // 5 seconds
int timer1 = 0, timer2 = 0, timer3 = 0, timer4 = 0;
bool timer1Active = false, timer2Active = false, timer3Active = false, timer4Active = false;

void setup(){
    Serial.begin(9600);
    pinMode(ir_car1, INPUT);
    pinMode(ir_car2, INPUT);
    pinMode(ir_car3, INPUT);
    pinMode(ir_car4, INPUT);
    pinMode(ir_enter, INPUT);
    pinMode(ir_back, INPUT);
     
    myservo.attach(3);
    myservo.write(90);
    lcd.begin(20, 4);  
    lcd.setCursor (0,1);
    lcd.print("    Car  parking  ");
    lcd.setCursor (0,2);
    lcd.print("       System     ");
    delay (2000);
    lcd.clear();  
    Read_Sensor();
    int total = S1+S2+S3+S4;
    slot = slot-total;
}

void loop(){
    unsigned long currentMillis = millis();
    
    // Separate timer logic for each slot
    if(S1 == 1) {
        if(!timer1Active) {
            timer1Active = true;
            previousMillis1 = currentMillis;
        }
        if(currentMillis - previousMillis1 >= interval) {
            timer1++;
            previousMillis1 = currentMillis;
        }
    } else {
        timer1Active = false;
        timer1 = 0;
    }

    if(S2 == 1) {
        if(!timer2Active) {
            timer2Active = true;
            previousMillis2 = currentMillis;
        }
        if(currentMillis - previousMillis2 >= interval) {
            timer2++;
            previousMillis2 = currentMillis;
        }
    } else {
        timer2Active = false;
        timer2 = 0;
    }

    if(S3 == 1) {
        if(!timer3Active) {
            timer3Active = true;
            previousMillis3 = currentMillis;
        }
        if(currentMillis - previousMillis3 >= interval) {
            timer3++;
            previousMillis3 = currentMillis;
        }
    } else {
        timer3Active = false;
        timer3 = 0;
    }

    if(S4 == 1) {
        if(!timer4Active) {
            timer4Active = true;
            previousMillis4 = currentMillis;
        }
        if(currentMillis - previousMillis4 >= interval) {
            timer4++;
            previousMillis4 = currentMillis;
        }
    } else {
        timer4Active = false;
        timer4 = 0;
    }

    Read_Sensor();
    lcd.setCursor (0,0);
    lcd.print("   Have Slot: ");
    lcd.print(slot);
    lcd.print("    ");  

    lcd.setCursor (0,1);
    if(S1==1){
        lcd.print("S1:Fill(");
        lcd.print(timer1);
        lcd.print(")");
    }
    else{
        lcd.print("S1:Empty  ");
    }

    lcd.setCursor (10,1);
    if(S2==1){
        lcd.print("S2:Fill(");
        lcd.print(timer2);
        lcd.print(")");
    }
    else{
        lcd.print("S2:Empty  ");
    }

    lcd.setCursor (0,2);
    if(S3==1){
        lcd.print("S3:Fill(");
        lcd.print(timer3);
        lcd.print(")");
    }
    else{
        lcd.print("S3:Empty  ");
    }

    lcd.setCursor (10,2);
    if(S4==1){
        lcd.print("S4:Fill(");
        lcd.print(timer4);
        lcd.print(")");
    }
    else{
        lcd.print("S4:Empty  ");
    }

    if(digitalRead (ir_enter) == 0 && flag1==0){
        if(slot>0){
            flag1=1;
            if(flag2==0){
                myservo.write(180); 
                slot = slot-1;
            }
        }
        else{
            lcd.setCursor (0,0);
            lcd.print(" Sorry Parking Full ");  
            delay(1500);
        }  
    }

    if(digitalRead (ir_back) == 0 && flag2==0){
        flag2=1;
        if(flag1==0){
            myservo.write(180); 
            slot = slot+1;
        }
    }

    if(flag1==1 && flag2==1){
        delay (1000);
        myservo.write(90);
        flag1=0, flag2=0;
    }

    delay(1);
}

void Read_Sensor(){
    S1=0, S2=0, S3=0, S4=0;
    if(digitalRead(ir_car1) == 0){S1=1;}
    if(digitalRead(ir_car2) == 0){S2=1;}
    if(digitalRead(ir_car3) == 0){S3=1;}
    if(digitalRead(ir_car4) == 0){S4=1;}
}


