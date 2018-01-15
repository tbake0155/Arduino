

class Message{
 private:
   float value;
   String type;

 public:
   Message(){value=0.0;type="No Type";}
   Message(float Value, String Type)
   {
     value = Value;
     type = Type;
   }
   ~Message(){}
   float Value(){return value;}
   void Value(float Value){value=Value;}
   String Type(){return type;}
   void Type(String Type){type=Type;}
};