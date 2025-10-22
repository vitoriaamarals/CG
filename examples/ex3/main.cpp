#include <vart/scene.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/point4d.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>
#include <vart/transform.h>
#include <vart/cone.h>
#include <vart/box.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace VART;

int main (int argc, char* argv[]) {

    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT

    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D::ORIGIN(),VART::Point4D::Y());

    Transform instanciaMesa;

    Box mesa;
    mesa.MakeBox(-3, 3, 0.0, 0.2, -2, 2);
    mesa.SetMaterial(VART::Material::DARK_PLASTIC_GRAY());
    instanciaMesa.AddChild(mesa);

    Box p1, p2, p3, p4;
    p1.MakeBox(0.2, 0.2, -2.0, 0.2, -0.2, 0.2);
    p1.SetMaterial(VART::Material::DARK_PLASTIC_GRAY());

    p2.MakeBox(0.2, 0.2, -2.0, 0.2, -0.2, 0.2);
    p2.SetMaterial(VART::Material::DARK_PLASTIC_GRAY());

    p3.MakeBox(0.2, 0.2, -2.0, 0.2, -0.2, 0.2);
    p3.SetMaterial(VART::Material::DARK_PLASTIC_GRAY());

    p4.MakeBox(0.2, 0.2, -2.0, 0.2, -0.2, 0.2);
    p4.SetMaterial(VART::Material::DARK_PLASTIC_GRAY());

    Transform posP1, posP2, posP3, posP4;
    posP1.MakeTranslation(-2.8, 0, 1.8);
    posP2.MakeTranslation(2.8, 0, 1.8);
    posP3.MakeTranslation(-2.8, 0, -1.8);
    posP4.MakeTranslation(2.8, 0, -1.8);
    instanciaMesa.AddChild(posP1);
    instanciaMesa.AddChild(posP2);
    instanciaMesa.AddChild(posP3);
    instanciaMesa.AddChild(posP4);

    posP1.AddChild(p1);
    posP2.AddChild(p2);
    posP3.AddChild(p3);
    posP4.AddChild(p4);

    scene.AddObject(&instanciaMesa);

    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.MakeCameraViewAll();

    // Set up the viewer
    viewer.SetTitle("MESA");
    viewer.SetScene(scene); // attach the scene

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;

}