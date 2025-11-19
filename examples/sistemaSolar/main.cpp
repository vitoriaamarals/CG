// main.cpp - from V-ART template application

// This example shows how to use the Arrow class.

// Changelog
// Oct 19, 2012 - Bruno de Oliveira Schneider
// - Removed the unused keyboard handler.
// - Added more arrows.
// Sep 25, 2008 - Bruno de Oliveira Schneider
// - Application created.

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>
#include <cmath>

#include <iostream>

using namespace std;
using namespace VART;

// Define the click handler
class ClickHandlerClass : public VART::MouseControl::ClickHandler
{
    public:
        ClickHandlerClass() {
        }
        virtual ~ClickHandlerClass() {};
        virtual void OnClick() {
            if (mouseCtrlPtr->LastClickIsDown()) {
            }
        }
};

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : solRad(0), terraRad(0) {}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            solRad += 0.0005;
            solRot->MakeYRotation(solRad);
            terraRad += 0.001;
            terraRot->MakeYRotation(terraRad);
            viewerPtr->PostRedisplay();
        }
    //protected:
        Transform* solRot;
        Transform* terraRot;
        Transform* luaRot;
    private:
        float solRad;
        float terraRad;
};

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)
    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,20),VART::Point4D::ORIGIN(),VART::Point4D::Y());

    // Create some objects
    VART::Arrow arrowX(50);
    VART::Arrow arrowY(VART::Point4D::ORIGIN(), VART::Point4D::Y()*50);
    VART::Arrow arrowZ(VART::Point4D::ORIGIN(), VART::Point4D::Z()*50);

    // Initialize scene objects
    arrowX.SetMaterial(VART::Material::PLASTIC_RED());
    arrowY.SetMaterial(VART::Material::PLASTIC_GREEN());
    arrowZ.SetMaterial(VART::Material::PLASTIC_BLUE());

    Sphere sol(30);
    Texture* texturaSol = new Texture;
    texturaSol->LoadFromFile("sol.jpeg");
    Material materialSol(*texturaSol);
    sol.SetMaterial(materialSol);

    Sphere terra(10);
    Texture* texturaTerra = new Texture;
    texturaTerra->LoadFromFile("terra.jpeg");
    Material materialTerra(*texturaTerra);
    terra.SetMaterial(materialTerra);

    Sphere lua(4);
    Texture* texturaLua = new Texture;
    texturaLua->LoadFromFile("lua.jpeg");
    Material materialLua(*texturaLua);
    lua.SetMaterial(materialLua);

    Transform rotTerraInit;
    rotTerraInit.MakeXRotation(-M_PI_2);
    rotTerraInit.AddChild(terra);

    Transform rotTerra;
    rotTerra.MakeIdentity();
    rotTerra.AddChild(rotTerraInit);

    Transform transTerra;
    transTerra.MakeTranslation(90,0,0);
    
    Transform rotLua;
    rotLua.MakeIdentity();

    Transform transLua;
    transLua.MakeTranslation(20, 0, 0); // 20 unidades em relação à Terra
    transLua.AddChild(lua);

    transTerra.AddChild(rotTerra);
    rotTerra.AddChild(transLua);
    transLua.AddChild(lua);

    Transform rotSol;
    rotSol.MakeIdentity();
    rotSol.AddChild(sol);
    rotSol.AddChild(transTerra);


    // Build the scene graph
    scene.AddObject(&arrowX);
    scene.AddObject(&arrowY);
    scene.AddObject(&arrowZ);
    scene.AddObject(&rotSol);

    // Add lights and cameras to the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.MakeCameraViewAll();

    MyIHClass idleHandler;
    idleHandler.solRot = &rotSol;
    idleHandler.terraRot = &rotTerra;

    // Set up the viewer
    viewer.SetTitle("V-ART arrow example");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idleHandler); 

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop) and never return
    return 0;
}
