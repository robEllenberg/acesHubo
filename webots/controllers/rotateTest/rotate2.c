#include <webots/robot.h>
#include <webots/servo.h>
#include <webots/touch_sensor.h>
#include <webots/emitter.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    wb_robot_init();
    int control_step = 50;
    
    static WbDeviceTag hole1, hole2;
    hole1 = wb_robot_get_device("hole1");
    hole2 = wb_robot_get_device("hole2");
    
    wb_servo_enable_position(hole1, control_step);
    wb_servo_enable_position(hole2, control_step);

    wb_robot_step(50 * control_step);
    
    double next_position1 = wb_servo_get_position(hole1)+1;
    double next_position2 = wb_servo_get_position(hole2)+1;
    
    for (;;) { 
        wb_servo_set_position(hole1, next_position1);
        wb_servo_set_position(hole2, next_position2);
        wb_robot_step(control_step);         /* run one step */
    }

    return 0;
}
