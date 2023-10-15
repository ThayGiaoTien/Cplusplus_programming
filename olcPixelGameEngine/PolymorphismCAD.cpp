#define OLC_PGE_APPLICATION
#include"olcPixelGameEngine.h"
#include<vector>

class sShape; // Prototype

struct sNode
{
    olc::vf2d pos;
    sShape* parent;
};

class sShape
{
private:
    
    int nMaxNodes;
    olc::Pixel col;
    // Create tranform fucntion because struct can't access to DrawToWorld/ortherwise utility of PGE

public:
    static float fWorldScale;
    static olc::vf2d vWorldOffset;
    // Convert coordinates from World Space --> Screen Space
    void WorldToScreen(const olc::vf2d& v, int& nScreenX, int& nScreenY)
    {
        nScreenX = (int)((v.x - vWorldOffset.x) * fWorldScale);
        nScreenY = (int)((v.y - vWorldOffset.y) * fWorldScale);
    }

public:
    std::vector<sNode> vecNodes;
    sShape()
    {
        col = olc::GREEN;
        SetMaxNode(0);
    }
    void SetMaxNode(int nodes)
    {
        nMaxNodes = nodes;
    }
    int GetMaxNodes()
    {
        return nMaxNodes;
    }
    olc::Pixel GetCol()
    {
        return col;
    }
    void SetCol(olc::Pixel colour)
    {
        col = colour;
    }
 
    virtual void DrawSelf(olc::PixelGameEngine* gfx) = 0;
    sNode* GetNextNode(const olc::vf2d& p)
    {
        if (vecNodes.size() >= nMaxNodes) return nullptr;
  
        // create new node and push to vecNodes.
        sNode newNode;
        newNode.parent = this;  
        newNode.pos = p;
        vecNodes.push_back(newNode);
        
        return &vecNodes[vecNodes.size() - 1];
    }
    void DrawNodes(olc::PixelGameEngine* gfx)
    {
        // Take the world positions
        // Transform into screen
        // Draw.
        for (auto &n : vecNodes)
        {
            int sx, sy;
            WorldToScreen(n.pos, sx, sy);
            gfx->FillCircle( sx, sy , 2, GetCol());
        }
    }
};

// We must define 2 static value of sShape struct. These value will be used for all derived struct.
float sShape::fWorldScale = 1.0f;
olc::vf2d sShape::vWorldOffset = {0,0 };

class sLine : public sShape
{
public:
    sLine()
    {
        SetMaxNode(2);
        vecNodes.reserve(GetMaxNodes());
        std::cout << "Created line\n ";
    }
    // Override DrawSelf fucntion
    void DrawSelf(olc::PixelGameEngine* gfx) override
    {
        int sx, sy, ex, ey;
        WorldToScreen(vecNodes[0].pos, sx, sy);
        WorldToScreen(vecNodes[1].pos, ex, ey);
        gfx->DrawLine(sx, sy, ex, ey, GetCol() );
    }
};





class PorlymorphismCAD : public olc::PixelGameEngine
{
public:
    PorlymorphismCAD()
    {
        sAppName = "Practice Polymorphism CAD";
    }


private:
    olc::vf2d vOffset = { 0.0f, 0.0f };
    olc::vf2d vStartPan = { 0.0f, 0.0f };
    float fScale = 10.0f;
    float fGrid = 1.0f;
    std::list <sShape*> listShapes;
    sShape* tempShape = nullptr;
    sNode* selectedNode = nullptr;
    

    olc::vf2d vCursor = { 0.0f, 0.0f };

    // Convert coordsfrom world space to screen space.
    void WorldToScreen(const olc::vf2d& v, int& nScreenX, int& nScreenY)
    {
        nScreenX = (int)((v.x - vOffset.x) * fScale);
        nScreenY = (int)((v.y - vOffset.y) * fScale);
    }

    // Convert coors from green to world space is exactly reverted
    void ScreenToWorld(int nScreenX, int nScreenY, olc::vf2d& v)
    {
        v.x = (float)(nScreenX) / fScale + vOffset.x;
        v.y = (float)(nScreenY) / fScale + vOffset.y;
    }


protected:
    bool OnUserCreate() override
    {
        // Set the default offset to the middle of the screen(center of screen is point (0,0))  
        vOffset = { (float)(-ScreenWidth() / 2) / fScale, (float)(-ScreenHeight() / 2) / fScale };

        sLine* n = new sLine();
        // create new node and push to vecNodes.
        sNode newNode;
        newNode.parent = n;
        newNode.pos = { 0,0 };
        n->vecNodes.push_back(newNode);
        sNode newNode2;
        newNode2.parent = n;
        newNode2.pos = { 10,0 };
        n->vecNodes.push_back(newNode2);
        listShapes.push_back(n);
        return true;
    }


    bool OnUserUpdate(float fElapsedTime) override
    {


        // Get current mouse position
        olc::vf2d vMouse = { (float)GetMouseX(), (float)GetMouseY() };

        /*Once get mouse coorsd, enable:
            + Zooming by scrolling the wheel.
            + Panning by holding down the mouse wheel & moving the cursor.*/

        // Panning==============================================================================
        if (GetMouse(2).bPressed)
            vStartPan = vMouse;
        if (GetMouse(2).bHeld)
        {
            vOffset -= (vMouse - vStartPan) / fScale;
            vStartPan = vMouse;
        }

        // Zooming is more complicated===========================================================
        /*Zooming relative to the mouse's cursor!!!*/
        olc::vf2d vMouseBeforeZoom;
        ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseBeforeZoom);

        if (GetKey(olc::Key::Q).bHeld || GetMouseWheel() > 0)
            fScale *= 1.1f;
        if (GetKey(olc::Key::A).bHeld || GetMouseWheel() < 0)
            fScale *= 0.9f;

        // Once scale has been changed, we need to calculate where is the mouse again
        olc::vf2d vMouseAfterZoom;
        ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseAfterZoom);

        // Update the offset vector
        vOffset += (vMouseBeforeZoom - vMouseAfterZoom);

        // The Cursor ========================================================
        /* To draw thing accurately and cleanly, it helps the cursor can snap to a particular location(nearest unit value)*/

        vCursor.x = floorf((vMouseAfterZoom.x + 0.5f) * fGrid);
        vCursor.y = floorf((vMouseAfterZoom.y + 0.5f) * fGrid);

        // Draw new line
        if (GetKey(olc::Key::L).bPressed)
        {
            tempShape = new sLine();
            selectedNode=tempShape->GetNextNode(vCursor);
            selectedNode=tempShape->GetNextNode(vCursor); // Set the temporary second node. Then we'll change it 
            // when L key is released.
        }

        // If a node is selected, make it follow the mouse cursor
        // by updating its position
        if (selectedNode != nullptr)
        {
            selectedNode->pos = vCursor;
        }
        if (tempShape != nullptr)
        {
            std::cout << tempShape->vecNodes[0].pos.x << "-" << tempShape->vecNodes[0].pos.y << std::endl;
            std::cout << tempShape->vecNodes[1].pos.x << "-" << tempShape->vecNodes[1].pos.y << std::endl;
        }
        // When user released mouse, place new shape into vecShapes.
        if (GetMouse(0).bReleased)
        {
            if (tempShape != nullptr)
            {
                selectedNode = tempShape->GetNextNode(vCursor);
                if (selectedNode == nullptr) // now vecNodes is full.
                {
                    tempShape->SetCol(olc::WHITE);
                    listShapes.push_back(tempShape);
                    std::cout << "Pushed!\n";
                    std::cout << tempShape->vecNodes[0].pos.x << "-" << tempShape->vecNodes[0].pos.y << std::endl;
                    std::cout << tempShape->vecNodes[1].pos.x << "-" << tempShape->vecNodes[1].pos.y << std::endl;
                    tempShape = nullptr;
                }
               
            }
            else
            {
                selectedNode = nullptr;
            }
        }
        

        // Clear the screen
        Clear(olc::VERY_DARK_BLUE);
        int sx, sy, ex, ey;

        // Get visible world
        olc::vf2d vWorldTopLeft, vWorldBottomRight;
        ScreenToWorld(0, 0, vWorldTopLeft);
        ScreenToWorld(ScreenWidth(), ScreenHeight(), vWorldBottomRight);

        // Do some overlap around the edges of the screen to make sure when we're drawing lines, we don't see them clipping.
        vWorldTopLeft.x = floor(vWorldTopLeft.x);
        vWorldTopLeft.y = floor(vWorldTopLeft.y);
        vWorldBottomRight.x = ceil(vWorldBottomRight.x);
        vWorldBottomRight.y = ceil(vWorldBottomRight.y);

        // Draw grid dots.
        /*Becaus i know the extent(pham vi) of my visible world, i can simply iterate along each axis stepping in my grid size.*/
        /*Because we're in world space, so fGrid= 1.0f*/
        for (float x = vWorldTopLeft.x; x < vWorldBottomRight.x; x += fGrid)
            for (float y = vWorldTopLeft.y; y < vWorldBottomRight.y; y += fGrid)
            {
                /*Change world space coords into screen space coords.*/
                WorldToScreen({ x,y }, sx, sy);

                // Draw grid dots.
                Draw({ sx, sy }, olc::BLUE);
            }

        // Draw axises - dash line 0xF0F0F0F0
        WorldToScreen({ 0.0f, vWorldTopLeft.y }, sx, sy);
        WorldToScreen({ 0.0f, vWorldBottomRight.y }, ex, ey);
        DrawLine(sx, sy, ex, ey, olc::WHITE, 0xF0F0F0F0);
        WorldToScreen({ vWorldTopLeft.x, 0.0f }, sx, sy);
        WorldToScreen({ vWorldBottomRight.x, 0.0f }, ex, ey);
        DrawLine(sx, sy, ex, ey, olc::WHITE, 0xF0F0F0F0);


        // Draw shapes
        // Draw All Existing Shapes
       
        for (auto& shape : listShapes)
        {
            shape->DrawSelf(this);
            shape->DrawNodes(this);
        }
        // Draw shape currently being defined
        if (tempShape != nullptr)
        {
            tempShape->DrawSelf(this);
            tempShape->DrawNodes(this);
        }

        // Draw the cursor
        WorldToScreen(vCursor, sx, sy);
        FillCircle({ sx, sy }, 3, olc::YELLOw);

        // Print the position of mouse cursor
        DrawString({ 10,10 }, "The mouse cursor: " + std::to_string(vCursor.x) + ", " + std::to_string(vCursor.y), olc::GREEN, 2);

        return true;
    }

};

int main()
{
    PorlymorphismCAD app;
    if (app.Construct(800, 480, 1, 1, false))
        app.Start();

    return 0;
}
