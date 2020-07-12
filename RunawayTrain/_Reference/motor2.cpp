/*
    L298 ���� ����̹��� ���� ������ DC���� ����
    gcc -o �������ϸ� �ҽ��� -lwiringPi
*/

#include <stdio.h>
#include <wiringPi.h>

#define MOTOR_PLUS 4     // BCM_GPIO 23
#define MOTOR_MINUS 5     // BCM_GPIO 24

int main(void)
{
    if (wiringPiSetup() == -1)
        return 1;

    pinMode(MOTOR_PLUS, OUTPUT);    // �⺻�� input �̹Ƿ� output �ʱ�ȭ �ʿ�
    pinMode(MOTOR_MINUS, OUTPUT);

    for (;;)
    {
        digitalWrite(MOTOR_PLUS, 1);     // ������ ����
        digitalWrite(MOTOR_MINUS, 0);

        delay(1000); // ms

        digitalWrite(MOTOR_PLUS, 0);     // ���� ����
        digitalWrite(MOTOR_MINUS, 1);

        delay(1000); // ms

        digitalWrite(MOTOR_PLUS, 0);     // ���߱�
        digitalWrite(MOTOR_MINUS, 0);

        delay(1000); // ms
    }

    return;
}