#define OLC_PGE_APPLICATION
#include"olcPixelGameEngine.h"


class PorlymorphismCAD: public olc::PixelGameEngine
{
public:
    PorlymorphismCAD()
    {   
        sAppName = "Practice Polymorphism CAD";
    }


private: 
    olc::vf2d vOffset= {0.0f, 0.0f};
    olc::vf2d vStartPan= {0.0f, 0.0f};
    float fScale =10.0f;
    float fGrid= 1.0f;

    olc::vf2d vCursor= {0.0f, 0.0f};

    // Convert coordsfrom world space to screen space.
    void WorldToScreen(const olc::vf2d &v, int &nScreenX, int &nScreenY)
    {
        nScreenX= (int)((v.x- vOffset.x)*fScale);
        nScreenY =(int)((v.y- vOffset.y)*fScale);
    }

    // Convert coors from green to world space is exactly reverted
    void ScreenToWorld(int nScreenX, int nScreenY,  olc::vf2d &v)
    {
        v.x= (float)(nScreenX)/fScale +vOffset.x;
        v.y =(float)(nScreenY)/fScale+ vOffset.y;
    }


protected:
    bool OnUserCreate() override
    {
        // Set the default offset to the middle of the screen(center of screen is point (0,0))  
        vOffset = { (float)(-ScreenWidth() / 2) / fScale, (float)(-ScreenHeight() / 2) / fScale };
        

        return true;
    }


    bool OnUserUpdate(float fElapsedTime) override
    {

        // Get current mouse position
        olc::vf2d vMouse= {(float)GetMouseX(), (float)GetMouseY()}; 

        /*Once get mouse coorsd, enable:
            + Zooming by scrolling the wheel.
            + Panning by holding down the mouse wheel & moving the cursor.*/

        // Panning==============================================================================
        if(GetMouse(2).bPressed)
            vStartPan= vMouse;
        if(GetMouse(2).bHeld)
        {
            vOffset -= (vMouse - vStartPan) / fScale;
            vStartPan= vMouse;
        }

        // Zooming is more complicated===========================================================
        /*Zooming relative to the mouse's cursor!!!*/
        olc::vf2d vMouseBeforeZoom;
        ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseBeforeZoom);

        if(GetKey(olc::Key::Q).bHeld || GetMouseWheel()>0)
            fScale *= 1.1f;
        if(GetKey(olc::Key::A).bHeld || GetMouseWheel()<0)
            fScale *= 0.9f;

        // Once scale has been changed, we need to calculate where is the mouse again
        olc::vf2d vMouseAfterZoom;
        ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseAfterZoom);

        // Update the offset vector
        vOffset += (vMouseBeforeZoom - vMouseAfterZoom);
        
        // The Cursor ========================================================
        /* To draw thing accurately and cleanly, it helps the cursor can snap to a particular location(nearest unit value)*/
        
        vCursor.x = floorf((vMouseAfterZoom.x+0.5f)*fGrid);
        vCursor.y = floorf((vMouseAfterZoom.y+0.5f)*fGrid);
    

        // Clear the screen
        Clear(olc::VERY_DARK_BLUE);
        int sx, sy, ex, ey;

        // Get visible world
        olc::vf2d vWorldTopLeft, vWorldBottomRight ; 
        ScreenToWorld(0, 0, vWorldTopLeft);
        ScreenToWorld(ScreenWidth(), ScreenHeight(), vWorldBottomRight);

        // Do some overlap around the edges of the screen to make sure when we're drawing lines, we don't see them clipping.
        vWorldTopLeft.x = floor(vWorldTopLeft.x);
        vWorldTopLeft.y= floor(vWorldTopLeft.y);
        vWorldBottomRight.x= ceil(vWorldBottomRight.x);
        vWorldBottomRight.y= ceil(vWorldBottomRight.y);

        // Draw grid dots.
        /*Becaus i know the extent(pham vi) of my visible world, i can simply iterate along each axis stepping in my grid size.*/
        /*Because we're in world space, so fGrid= 1.0f*/
        for(float x= vWorldTopLeft.x; x<vWorldBottomRight.x; x+= fGrid)
            for(float y= vWorldTopLeft.y; y<vWorldBottomRight.y;y+=fGrid)
            {
                /*Change world space coords into screen space coords.*/
                WorldToScreen({x,y},sx, sy);

                // Draw grid dots.
                Draw({sx, sy}, olc::BLUE);
            }

        // Draw axises - dash line 0xF0F0F0F0
        WorldToScreen({0.0f, vWorldTopLeft.y}, sx, sy);
        WorldToScreen({0.0f, vWorldBottomRight.y}, ex,ey);
        DrawLine(sx, sy, ex, ey, olc::WHITE, 0xF0F0F0F0);
        WorldToScreen({vWorldTopLeft.x, 0.0f}, sx, sy);
        WorldToScreen({vWorldBottomRight.x, 0.0f}, ex,ey);
        DrawLine(sx, sy, ex, ey, olc::WHITE, 0xF0F0F0F0);

        // Draw the cursor
        WorldToScreen(vCursor, sx, sy);
        FillCircle({sx, sy}, 3, olc::RED);

        // Print the position of mouse cursor
        DrawString({10,10}, "The mouse cursor: " + std::to_string(vCursor.x) +", "+ std::to_string(vCursor.y), olc::GREEN,2);
       
        return true;
    }

};

int main()
{
    PorlymorphismCAD app;
    if(app.Construct(800, 480,2,2))
        app.Start();

    return 0;
}
