
void setup() {
    Serial.begin(115200);

    pinMode(2, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
}

float percent_a = 0.0;
float percent_b = 0.0;
// float cutoff = 0.06;

void loop() {

    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        if (input.startsWith("a")) {
            String valueString = input.substring(1);
            int value = valueString.toInt();
            if (value <= 100) {
                percent_a = value/100.0;
                Serial.print("percent_a has been set to ");
                Serial.println(percent_a);
            }
        }
        if (input.startsWith("b")) {
            String valueString = input.substring(1);
            int value = valueString.toInt();
            if (value <= 30) {
                percent_b = value/100.0;
                Serial.print("percent_b has been set to ");
                Serial.println(percent_b);
            }
        }
    }

    //  int duty1 = 255-int(255*cutoff)-count;
    //  int duty2 = 255-int(255*percent)+count;

    int duty_a = 255 - int(255 * percent_a);
    int duty_b = 255 - int(255 * percent_b);

    analogWrite(4, duty_a);
    analogWrite(5, duty_b);

    delay(30);

    Serial.print(percent_a);
    Serial.print(", ");
    Serial.println(percent_b);
}
