#include <webots/robot.h>
#include <webots/servo.h>
#include <webots/touch_sensor.h>
#include <webots/emitter.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    wb_robot_init();
    WbDeviceTag joint = wb_robot_get_device("KU");
    while(1){
        float i = 0;
        for(i = 0; i < 100; i+=.04){
            //printf("in de loop\n");
            wb_servo_set_position(joint, i);
            wb_robot_step(32);   
        }
    }
}
