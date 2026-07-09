#include <Buzzer.h>

void Buzzer::Ring()
{
#ifdef PASSIVE_BUZZER
    tone(BUZZER_BUS, 1000);
    delay(500);
    noTone(BUZZER_BUS);
    delay(500);
    tone(BUZZER_BUS, 1000);
    delay(500);
    noTone(BUZZER_BUS);
#else
    digitalWrite(BUZZER_BUS, HIGH);
    delay(500);
    digitalWrite(BUZZER_BUS, LOW);
    delay(500);
    digitalWrite(BUZZER_BUS, HIGH);
    delay(500);
    digitalWrite(BUZZER_BUS, LOW);
#endif  // PASSIVE_BUZZER
}

void Buzzer::Ring(int count)
{
    Ring(count, 500);
}

void Buzzer::Ring(int count, int duration)
{
    for (byte i = 0; i < count; i++)
    {
#ifdef PASSIVE_BUZZER
        tone(BUZZER_BUS, 1000);
        delay(duration);
        noTone(BUZZER_BUS);
        delay(500);
#else
        digitalWrite(BUZZER_BUS, HIGH);
        delay(duration);
        digitalWrite(BUZZER_BUS, LOW);
        delay(500);
#endif  // PASSIVE_BUZZER
    }
}