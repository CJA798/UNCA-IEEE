void setup(){
  Map botLoc;
  botLoc.setMovedBot(12, 30, 90);
  botLoc.setColumn();
  botLoc.setMovedBot(40, 20, 0);
  int* dest = botLoc.getColumn();
  Serial.begin(9600);
  Serial.print(dest[0]);
  Serial.print("\t\t");
  Serial.print(dest[1]);
  
}

void loop(){

}