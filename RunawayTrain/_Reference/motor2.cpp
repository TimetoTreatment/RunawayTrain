/*
    L298 모터 드라이버를 통한 서보형 DC모터 제어
    gcc -o 실행파일명 소스명 -lwiringPi
*/

#include <stdio.h>
#include <wiringPi.h>

#define MOTOR_PLUS 4     // BCM_GPIO 23
#define MOTOR_MINUS 5     // BCM_GPIO 24

int main(void)
{
    if (wiringPiSetup() == -1)
        return 1;

    pinMode(MOTOR_PLUS, OUTPUT);    // 기본이 input 이므로 output 초기화 필요
    pinMode(MOTOR_MINUS, OUTPUT);

    for (;;)
    {
        digitalWrite(MOTOR_PLUS, 1);     // 오른쪽 돌기
        digitalWrite(MOTOR_MINUS, 0);

        delay(1000); // ms

        digitalWrite(MOTOR_PLUS, 0);     // 왼쪽 돌기
        digitalWrite(MOTOR_MINUS, 1);

        delay(1000); // ms

        digitalWrite(MOTOR_PLUS, 0);     // 멈추기
        digitalWrite(MOTOR_MINUS, 0);

        delay(1000); // ms
    }

    return;
}