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
    VART::Camera camera(VART::Point4D(0,3,10),
                        VART::Point4D(0, 2, 0),
                        VART::Point4D::Y());

    Sphere baseNeve(2.0);
    baseNeve.SetMaterial(VART::Material::PLASTIC_WHITE());

    Sphere meioNeve(1.5);
    meioNeve.SetMaterial(VART::Material::PLASTIC_WHITE());

    Sphere cabecaNeve(1.0);
    cabecaNeve.SetMaterial(VART::Material::PLASTIC_WHITE());

    Transform posBase;
    posBase.MakeTranslation(0, 0, 0);

    Transform posMeio;
    posMeio.MakeTranslation(0, 2.0 + 1.5, 0);

    Transform posCabeca;
    posCabeca.MakeTranslation(0, (1.5 + 1.0), 0);

    scene.AddObject(&posBase);
    posBase.AddChild(posMeio);
    posMeio.AddChild(posCabeca);

    posBase.AddChild(baseNeve);
    posMeio.AddChild(meioNeve);
    posCabeca.AddChild(cabecaNeve);

    Box bracoDireito;
    bracoDireito.MakeBox(0, 0.2, -1.5, 1.5, -0.2, 0.2);
    bracoDireito.SetMaterial(VART::Material::DARK_PLASTIC_GRAY());

    Box bracoEsquerdo;
    bracoEsquerdo.MakeBox(-0.2, 0, -1.5, 1.5, -0.2, 0.2);
    bracoEsquerdo.SetMaterial(VART::Material::DARK_PLASTIC_GRAY());

    Transform posBracoDireito;
    posBracoDireito.MakeTranslation(1.0, 1.5, 0);
    Transform posBracoEsquerdo;
    posBracoEsquerdo.MakeTranslation(-1.0, 1.5, 0);

    /*Cone chapeu(2.0, 1.5);
    chapeu.SetMaterial(VART::Material::PLASTIC_GREEN());
    chapeu.SetHeight(2.0);

    Transform posChapeu;
    posChapeu.MakeTranslation(0, 1.0 + 2.0, 0);
    posCabeca.AddChild(posChapeu);
    posChapeu.AddChild(chapeu);*/


    posMeio.AddChild(posBracoDireito);
    posMeio.AddChild(posBracoEsquerdo);

    posBracoDireito.AddChild(bracoDireito);
    posBracoEsquerdo.AddChild(bracoEsquerdo);

    Sphere olhoDireito(0.1);
    olhoDireito.SetMaterial(VART::Material::PLASTIC_BLACK());

    Sphere olhoEsquerdo(0.1);
    olhoEsquerdo.SetMaterial(VART::Material::PLASTIC_BLACK());

    Transform posOlhoDireito;
    posOlhoDireito.MakeTranslation(0.35, 0.2, 0.9);

    Transform posOlhoEsquerdo;
    posOlhoEsquerdo.MakeTranslation(-0.35, 0.2, 0.9);   
    
    posCabeca.AddChild(posOlhoDireito);
    posCabeca.AddChild(posOlhoEsquerdo);

    posOlhoDireito.AddChild(olhoDireito);
    posOlhoEsquerdo.AddChild(olhoEsquerdo);

    Cone nariz(0.5, 0.25);
    nariz.SetMaterial(VART::Material::PLASTIC_RED());
    Transform posNariz;
    posNariz.MakeTranslation(0, 0, 1.0);
    posCabeca.AddChild(posNariz);
    posNariz.AddChild(nariz);

    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.MakeCameraViewAll();

    // Set up the viewer
    viewer.SetTitle("BONECO DE NEVE");
    viewer.SetScene(scene); // attach the scene

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;

}