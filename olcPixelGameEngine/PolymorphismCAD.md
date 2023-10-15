<h1> Practic polymorphism by creating CAD application</h1>
<details>
<summary> <h2>The idea & design</h2></summary>
  
### Create class A as a base class to encapsulate the common funcionality between 2 derived classed B & C.
```c 
  class A{
  public: 
      A(){}
      void DoSomething();
  };
  
  class B :public A
  {};
  class C: public A
  {};
```
### By hinting to the compiler that B & C are derived class from A, we can create a vector that stores only elements of type A.
```c
    vector<A> vec;
```
### But the problem is when wo call DoSomething() method, it's going to call the method of class A. By storing the pointer to where A mgiht riside in the memory, we can actually create a new object of class B & C. 
```c
  A* ob1 = new A();
  A* ob2 = new B();
  A* ob3= new C();
  vector<A*> vecObjects = {ob1, ob2, ob3};
```
### What we implying here is if B & C override a methos in class A, then if we call such as method, it will call the appropriate method related to that class.
- When we call DoSomething() on the vector, regardless of whether  that object type was B or C, it would still call  DoSomething() method related to class A.
--> We need to tell compiler that DoSomething() is a candidtae to be overridden by derived class.
  #### "virtual" keyword: tell that this method must be overridden.
  ```c
  virtual void DoSomething()=0;
  ```
  ---> Now we can't create any object of class A because there is no implementation of DoSomething() method.
### Class structer
#### Struct to represent node
```c
struct sNode{
  int x, y;
  sShape* parent;
}
```
#### Class hierachy:
sShape <br>
->Draw()=0; <br>
->vector<sNode> vecNodes;<br>
|--> Line ->Draw() override;<br>
|--> Box ->Draw() override;<br>
|--> Circle ->Draw() override;<br>
|--> Curve ->Draw() override; <br>

</details>

<details>
<summary> <h2> Panning & Zooming</h2></summary>
  
-Spanning by hold dowm the mouse wheel and moving the mouse cursor.

```c
   if (GetMouse(2).bPressed)
            vStartPan = vMouse;
        if (GetMouse(2).bHeld)
        {
            vOffset -= (vMouse - vStartPan) / fScale;
            vStartPan = vMouse;
        }
  ```
- Scaling by scrolling the wheel(GetMouseWheel()>0) but more complicated.
  - First, store mouse position before zooming:
  ```c 
      olc::vf2d vMouseBeforeZoom;
      ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseBeforeZoom);
  ```
  - Once the scale value has been changed, we need to calculate where is the mouse again.
  ```c
    olc::vf2d vMouseAfterZoom;
    ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseAfterZoom);

    // Update the offset vector
    vOffset += (vMouseBeforeZoom - vMouseAfterZoom);
  ```
==> That's how we're zooming relative to the mouse's position.

</details>

<details>
<summary> <h2> World-Screen Transform</h2></summary>
  
  ### When update position, transform to real world pisition:
    
  ```c
    void ScreenToWorld(int nScreenX, int nScreenY, olc::vf2d& v)
    {
        v.x = (float)(nScreenX) / fScale + vOffset.x;
        v.y = (float)(nScreenY) / fScale + vOffset.y;
    }
  ```
  
 ### When Draw into screen, transform from world to screen:
    ```c
    void WorldToScreen(const olc::vf2d& v, int& nScreenX, int& nScreenY)
      {
          nScreenX = (int)((v.x - vOffset.x) * fScale);
          nScreenY = (int)((v.y - vOffset.y) * fScale);
      }

  ```
</details>

<details>
<summary> <h2> The cursor </h2></summary>
- Update selected node position
- Move the node around(Hit node function)
- To draw accurately, cursor should snap to particular node.
</details>

<details>
<summary> <h2> Draw Screeen </h2></summary>
- First, clear screen
- Second, initalize some variable to store start-end point, top-left, bottom-right of screen.
- Don't forget to transfrom from world to screenbefore draw. We need to draw: grid dots, axises, cursor, cursor position...
</details>

