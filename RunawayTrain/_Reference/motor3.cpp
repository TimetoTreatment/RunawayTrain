//����A ��Ʈ��
int IN1 = 7;
int IN2 = 6;

//����B ��Ʈ��
int IN3 = 5;
int IN4 = 4;

void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void loop() {
    bothMotorStart();
    delay(3000);
    turnLeft();
    delay(3000);
    turnRight();
    delay(3000);
    motorA_Rotation();
    delay(3000);
    motorB_Rotation();
    delay(3000);
    motorA_Reverse();
    delay(3000);
    motorB_Reverse();
    delay(3000);
    stopAllMotor();
    delay(3000);
}

//����A,B ��ȸ��
void bothMotorStart()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

//����A,B Stop
void stopAllMotor()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

//����A ��ȸ��, ����B ��ȸ��
void turnLeft()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

//����A ��ȸ��, ����B ��ȸ��
void turnRight()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

//����A ��ȸ��, ����B Stop
void motorA_Rotation()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

//����A Stop, ����B ��ȸ��
void motorB_Rotation()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

//����A ��ȸ��, ����B Stop
void motorA_Reverse()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

//����A Stop, ����B ��ȸ��
void motorB_Reverse()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}


��ó: https://deneb21.tistory.com/277 [Do It Yourself!]