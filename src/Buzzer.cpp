#include <Buzzer.h>

void Buzzer::Ring()
{
    digitalWrite(BUZZER_BUS, HIGH);
    delay(500);
    digitalWrite(BUZZER_BUS, LOW);
    delay(500);
    digitalWrite(BUZZER_BUS, HIGH);
    delay(500);
    digitalWrite(BUZZER_BUS, LOW);
}

void Buzzer::Ring(int count)
{
    Ring(count, 500);
}

void Buzzer::Ring(int count, int duration)
{
    for (byte i = 0; i < count; i++)
    {
        digitalWrite(BUZZER_BUS, HIGH);
        delay(duration);
        digitalWrite(BUZZER_BUS, LOW);
        delay(500);
    }
}