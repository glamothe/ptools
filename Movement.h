#ifndef MOUVEMENT_H
#define MOUVEMENT_H
  

namespace PTools
{
//base class movement--------------------------------------------------------  
  class Movement 
  {
    //atribut
    protected:
    Matrix m;
        
    public:
    Movement();
    Movement(Matrix);
    ~Movement();
    
    Movement operator+ (Movement);
    
    void apply(Rigidbody& rigbody);
    
    Matrix getMatrix();
    
    void print();
    
    
  };
  
  //fonction pour les matrices, a relocaliser dans la partie ptools adapter
  Matrix matrixMultiply(Matrix,Matrix);
  Matrix inverseMatrix44 (Matrix);
  Matrix inverseTranformationMatrix(Matrix);
  Matrix inverseTranformationMatrixPlusPlus(Matrix);

//child class--------------------------------------------------------
  class Shift: public Movement //translation on x (short axis)
  {
    public:
    Shift(double);
    
  };
  
  class Slide: public Movement //translation on y (long axis)
  {
    public:
    Slide(double);
    
  };
  
  class Rise: public Movement //translation on z (perpendicular axis)
  {
    public:
    Rise(double);
    
  };
// rotation around Z, Y and then X -> order to respect

  class Twist: public Movement //rotation around z (perpendicular axis)
  {
    public:
    Twist(double);
  };

  class Roll: public Movement //rotation around y (long axis)
  {
    public:
    Roll(double);
  };

  class Tilt: public Movement //rotation around x (short axis)
  {
    public:
    Tilt(double);
  };
  
  class BDNA: public Movement 
  {
    public:
    BDNA();
  };

  class ADNA: public Movement 
  {
    public:
    ADNA();
  };
  
}//end namespace


#endif // MOUVEMENT_H 