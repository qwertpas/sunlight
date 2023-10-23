
void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

int count = 0;
int dir = 1;

float percent = 0.06;
float cutoff = 0.06;

int count2 = 0;

void loop() {

  count2++;

  if(count2 < 200){
    percent = 0.06;
  }else if(count2 < 400){
    percent = 0.15;
  }else if(count2 < 600){
    percent = 0.30;
  }else{
    count2 = 0;
  }
  
   count += dir;
  if(dir == 1 && count > int(255*(percent - cutoff))){
    dir = -1;
  }else if(dir == -1 && count < 1){
    dir = 1;
  }


//  int duty1 = 255-int(255*cutoff)-count;
//  int duty2 = 255-int(255*percent)+count;

  int duty1 = 255-int(255*percent);
  int duty2 = 255-int(255*percent);
  

  analogWrite(2, count);
  analogWrite(4, duty1);
  analogWrite(5, duty2);

  delay(30);
  

  Serial.print(count);
  Serial.print(", ");
  Serial.print(duty1);
  Serial.print(", ");
  Serial.println(duty2);
  Serial.print(", ");
  Serial.println(percent);
  
}
