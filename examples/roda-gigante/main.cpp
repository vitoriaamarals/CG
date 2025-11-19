/*#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>
#include <vart/scene.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/texture.h>
#include <vart/transform.h>

using namespace VART;
using namespace std;

int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv);
    static VART::ViewerGlutOGL viewer;
    static VART::Scene scene;

    //Add a camera to a scene and configure it.
    Camera camera(VART::Point4D(0,0,20),VART::Point4D::ORIGIN(),VART::Point4D::Y());
    camera.SetDescription("camera");
    scene.AddCamera( &camera );

    Transform wheelTransl;
    wheelTransl.MakeIdentity();
    scene.AddObject( &wheelTransl );

    Transform wheelRot;
    wheelRot.MakeIdentity();
    wheelTransl.AddChild( wheelRot );

    Transform wheelScale;
    wheelScale.MakeScale(0.1, 0.1, 0.1);
    wheelRot.AddChild( wheelScale );

    Transform initialWheelRot;
    initialWheelRot.MakeYRotation(1.57); //Rotate 90 degrees
    wheelScale.AddChild( initialWheelRot );

    list<VART::MeshObject*> objData;
    list<VART::MeshObject*>::iterator it;

    // Load a object from a ".obj" file. Note that the object refers to a material
    // library, that refers to two other textures.
    if( VART::MeshObject::ReadFromOBJ("ferris-wheel.obj", &objData))
    {
        // Here, we add the object data to a transform, setup the data and the transform.
        for(it = objData.begin(); it != objData.end(); ++it){
            if ((*it)->GetDescription() == "chair") {
                for (int i = 0; i < 10; i++) {
                    Transform* ptChairTransl = new Transform();
                    ptChairTransl->MakeTranslation(Point4D(0,70*sin(i*(M_PI/5)),70*cos(i*(M_PI/5))));
                    ptChairTransl->AddChild(**it);
                    initialWheelRot.AddChild( *ptChairTransl );
                    
                }
            } else if ((*it)->GetDescription() == "wheel") {
                (*it)->autoDelete = true;
                initialWheelRot.AddChild( **it );
            } else {
                Transform* scaleTransform = new VART::Transform;
                scaleTransform->MakeScale(0.1, 0.1, 0.1);
                scaleTransform->AddChild(**it);   // o **it é o MeshObject
                scene.AddObject(scaleTransform);
            }
        }
    }
    else
        cout << "Could not read '" << "ferris-wheel.obj" << "' for object data." << endl;


    //Add a light to the scene.
    scene.AddLight( Light::BRIGHT_AMBIENT() );
    scene.DrawLightsOGL();

    //Setup the viewer.
    viewer.SetTitle("V-ART texture example");
    viewer.SetScene(scene);

    //Enable texture in OpenGL
    glEnable( GL_TEXTURE_2D );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    //Enter the main loop
    VART::ViewerGlutOGL::MainLoop();

    return 0;
}
*/
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
        MyIHClass() : chairRadianos(0), wheelRadianos(0) {}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            chairRadianos += 0.003;
            chairRotPtr->MakeXRotation(chairRadianos);

            wheelRadianos -= 0.003;
            wheelRotPtr->MakeXRotation(wheelRadianos);

            viewerPtr->PostRedisplay();
        }
    //protected:
        VART::Transform* chairRotPtr;
        VART::Transform* wheelRotPtr;
    private:
        float chairRadianos;
        float wheelRadianos;
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

    //rotação
    VART::Transform chairRotation;
    chairRotation.MakeIdentity();
    VART::Transform wheelRotation;
    wheelRotation.MakeIdentity();

    scene.AddObject(&wheelRotation);

    //leitura
    list<VART::MeshObject*> objects;
    VART::MeshObject::ReadFromOBJ("ferris-wheel.obj", &objects);
    list<VART::MeshObject*>::iterator iter = objects.begin();
    for (; iter != objects.end(); ++iter) {
        if((*iter)->GetDescription() == "chair"){
            for (int i= 0; i < 10; i++){
                VART::Transform* ptChairTranslacao = new VART::Transform;
                ptChairTranslacao->MakeTranslation(0,70*sin(i*(M_PI/5)),70*cos(i*(M_PI/5)));
                ptChairTranslacao->AddChild(chairRotation);
                wheelRotation.AddChild(*ptChairTranslacao);
            }
            chairRotation.AddChild(**iter);
        }else if((*iter)->GetDescription() == "wheel"){
            wheelRotation.AddChild(**iter);
        }
        else{
            scene.AddObject(*iter);
        }
    }

    MyIHClass idleHandler;
    idleHandler.chairRotPtr = &chairRotation;
    idleHandler.wheelRotPtr = &wheelRotation;

    // Build the scene graph
    scene.AddObject(&arrowX);
    scene.AddObject(&arrowY);
    scene.AddObject(&arrowZ);

    // Add lights and cameras to the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.MakeCameraViewAll();

    // Set up the viewer
    viewer.SetTitle("V-ART arrow example");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idleHandler); 

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop) and never return
    return 0;
}
