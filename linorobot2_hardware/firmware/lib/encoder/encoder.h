#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "config.h" // Needed for MOTOR_MAX_RPM

class Encoder {
public:
    Encoder(uint8_t hlfb_pin)
        : hlfb_pin_(hlfb_pin), lastValidDuty_(0.0) {
        pinMode(hlfb_pin_, INPUT);
    }

    float readDutyCycle(int samples = 5) {
        float total = 0.0;
        int valid = 0;
        for (int i = 0; i < samples; i++) {
            unsigned long highTime = pulseIn(hlfb_pin_, HIGH, 4000);
            unsigned long lowTime = pulseIn(hlfb_pin_, LOW, 4000);
            unsigned long period = highTime + lowTime;

            if (period == 0) continue;

            float duty = 100.0 * highTime / period;
            if (duty > 5.0 && duty < 95.0) {
                total += duty;
                valid++;
               
            }
          //  delay(3);
        }
	
        if (valid == 0){ // this returns zero if there are no valid commands for > 2 cycles 
       		counter_++;           
      		  if (counter_ >= 5) {
      		  lastValidDuty_ = 0.0;
      		  return 0.0; 
      		  }
      	return lastValidDuty_;
      	} else {
      	counter_ = 0;
        lastValidDuty_ = total / valid;
        return lastValidDuty_;
    }
}

    float getRPM() {
        float duty = readDutyCycle();
        int raw_rpm = (duty / 100.0); //* (MOTOR_MAX_RPM * MAX_RPM_RATIO);
        return raw_rpm; //raw_rpm; // 7.0; // Gear ratio adjustment
        //return raw_rpm = 10;
    }

private:
    uint8_t hlfb_pin_;
    float lastValidDuty_;
    int counter_; 
};

#endif
