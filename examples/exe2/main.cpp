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
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D::ORIGIN(),VART::Point4D::Y());

    // Create some objects
    VART::Arrow arrowX(2);
    VART::Arrow arrowY(VART::Point4D::ORIGIN(), VART::Point4D::Y()*2);
    VART::Arrow arrowZ(VART::Point4D::ORIGIN(), VART::Point4D::Z()*2);

    // Initialize scene objects
    arrowX.SetMaterial(VART::Material::PLASTIC_RED());
    arrowY.SetMaterial(VART::Material::PLASTIC_GREEN());
    arrowZ.SetMaterial(VART::Material::PLASTIC_BLUE());

    // Build the scene graph
    scene.AddObject(&arrowX);
    scene.AddObject(&arrowY);
    scene.AddObject(&arrowZ);

    Sphere solSphere(1);
    solSphere.SetMaterial(VART::Material::PLASTIC_RED());

    Sphere terra(0.4);
    terra.SetMaterial(VART::Material::PLASTIC_BLUE());

    Sphere lua(0.17);
    lua.SetMaterial(VART::Material::DARK_PLASTIC_GRAY());

       Transform solRot;
    solRot.MakeIdentity();     // Sol gira aqui
    solRot.AddChild(solSphere);

    // Terra
    Transform terraRot;        // Terra gira sobre si
    terraRot.MakeIdentity();

    Transform terraTransl;     // Terra orbita o Sol
    terraTransl.MakeTranslation(3,0,0);
    terraTransl.AddChild(terra);

    terraRot.AddChild(terraTransl);   // giro → translação → esfera

    // Lua
    Transform luaRot;          // Lua orbita a Terra
    luaRot.MakeIdentity();

    Transform luaTransl;       // posição da lua
    luaTransl.MakeTranslation(0.7,0,0);
    luaTransl.AddChild(lua);

    luaRot.AddChild(luaTransl);
    terraRot.AddChild(luaRot); // Lua presa à Terra

    // --- Monta o Sistema Solar ---
    solRot.AddChild(terraRot);

    scene.AddObject(&solRot);


    // Set up the Idle Handler
    MyIHClass idleHandler;
    idleHandler.terraRot = &terraRot;
    idleHandler.solRot = &solRot;
    //idleHandler.luaRot = &luaRot;

    // Add lights and cameras to the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    viewer.SetIdleHandler(&idleHandler); 

    // Set up the viewer
    viewer.SetTitle("V-ART arrow example");
    viewer.SetScene(scene); // attach the scene

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop) and never return
    return 0;
}
