  #include <Servo.h>

  //Pins for the players
  int ledPin[] = {4,5,6};
  int ledPin2[] = {10,11,12};

  //Pins for the buttons
  int playerOneButton = 2;
  int playerTwoButton = 3;

  //These ints represent the player's scores, which start at 0 for the beginning of the game
  int playerOneScore = 0;
  int playerTwoScore = 0;

  //These ints represent the pins connected to the White LED, potentiometer and piezo respectively
  int whiteLED = 8;
  int modeValue = 0;
  int difficultyPin = A0;
  int speakerPin = 9;
  int servoPin = 7;
  //Servo position is set to mid-point for the start
  int servoPos = 90;

  //This is the general delay between LEDs lighting up
  int delayTime = 1000;
  // time delay between lights on/off
  volatile int randNumber;
  Servo gameServo;

  //Booleans for hit/miss are set as false to ensure the scoring system starts at zero-zero
  volatile bool hitPlayerOne = false;
  volatile bool hitPlayerTwo = false;
  volatile bool missPlayerOne = false;
  volatile bool missPlayerTwo = false;

  //setup interrupt, button input and LED outputs
  void setup() {
    attachInterrupt(0, playerOneInput, FALLING); // specify interrupt routine
    attachInterrupt(1, playerTwoInput, FALLING);
    Serial.begin(9600);
    for (int i=0; i<3; i++){
      pinMode(ledPin[i], OUTPUT);
      pinMode(ledPin2[i], OUTPUT);
    }
    pinMode(playerOneButton, INPUT);
    pinMode(playerTwoButton, INPUT);
    pinMode(whiteLED, OUTPUT);
    pinMode(speakerPin, OUTPUT);
    gameServo.attach(7);
    gameServo.write(90);
  }

  //run main program loop
  void loop() {
    randNumber = random(3); // select a random number
    modeValue = analogRead(difficultyPin);
    delayTime = modeValue;
    digitalWrite(ledPin[randNumber], HIGH); // light the LED with this number
    delay(delayTime);
    digitalWrite(ledPin[randNumber], LOW);
    delay(delayTime);
    if(digitalRead(whiteLED)==HIGH){
      digitalWrite(whiteLED, LOW);
    }  //if whiteLED on = turn it off

    digitalWrite(ledPin2[randNumber], HIGH); // light the LED with this number
    delay(delayTime);
    digitalWrite(ledPin2[randNumber], LOW);
    delay(delayTime);

    //These booleans increment/decrement the score when met
    //This also ensures that bouncing of buttons doesn't result in repeated increments of the score
    if(hitPlayerOne == true){
      playHitTone();
      playerOneScore = playerOneScore + 1;
      hitPlayerOne = false;
    }
    if(missPlayerOne == true){
      playMissTone();
      playerTwoScore = playerOneScore - 1;
      missPlayerOne = false;
    }
    if(hitPlayerTwo == true){
      playHitTone();
      playerTwoScore = playerTwoScore + 1;
      hitPlayerTwo = false;
    }
    if(missPlayerTwo == true){
      playMissTone();
      playerTwoScore = playerTwoScore - 1;
      missPlayerTwo = false;
    }
    servoRotate(); 

    Serial.print("Player 1: ");
    Serial.println(playerOneScore);
    Serial.print("Player 2: ");
    Serial.println(playerTwoScore);

    while(playerOneScore>=5 || playerTwoScore>=5) //The while loop causes all LEDs to light up, when at least one of the players reaches a score of 5
    {
      for(int x=0; x < 3; x++){
        digitalWrite(ledPin[x], HIGH);
        digitalWrite(ledPin2[x], HIGH);
        digitalWrite(whiteLED, HIGH);
      }
    }
  }

  //This function is made to point the servo to the leading player
  //If player 1 is leading, rotate 45 degrees anticlockwise and vice versa
  void servoRotate() {
    if(playerOneScore > playerTwoScore) {
      servoPos = 45;  
      gameServo.write(servoPos);
    }
    else if (playerOneScore < playerTwoScore) {
      servoPos = 135;
      gameServo.write(servoPos);  
    }
    else {
      servoPos = 90;
      gameServo.write(servoPos);  
    }
  }

  // plays the sound for when the button is correctly pressed
  void playHitTone(){
    tone(speakerPin, 1000);
    // plays sound for 100 ms
    delay(100);
    noTone(speakerPin);
  }

  // plays the sound for when the button is incorrectly pressed
  void playMissTone(){
    tone(speakerPin, 500);
    // plays sound for 100 ms
    delay(100);
    noTone(speakerPin);
  }

  void playerOneInput() {
    if(digitalRead(ledPin[randNumber])==HIGH)
    {
      digitalWrite(whiteLED, HIGH);
      //To ensure only 
      hitPlayerOne = true; 
      hitPlayerTwo = false;

    }
    else
    {
      digitalWrite(whiteLED, LOW);
      missPlayerOne = true;
      missPlayerTwo = false;
    }
  }

  void playerTwoInput() {
    if(digitalRead(ledPin2[randNumber])==HIGH)
    {
      digitalWrite(whiteLED, HIGH); 
    }
    else
      hitPlayerTwo = true;
    hitPlayerOne = false; 
    {
      digitalWrite(whiteLED, LOW);
      missPlayerTwo = true;
      missPlayerOne = false;
    }
  }
