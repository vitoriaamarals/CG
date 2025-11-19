#include <vart/scene.h>
#include <vart/transform.h>
#include <vart/joint.h>
#include <vart/jointmover.h>
#include <vart/uniaxialjoint.h>
#include <vart/baseaction.h>
#include <vart/jointaction.h>
#include <vart/light.h>
#include <vart/sphere.h>
#include <vart/callback.h>
#include <vart/meshobject.h>
#include <vart/box.h>
#include <vart/linearinterpolator.h>
#include <vart/contrib/viewerglutogl.h>

#include <iostream>

using namespace std;
using namespace VART;

const unsigned int MAX_KEYS = 8; // max number of keys to keep control
enum Key { UP, DOWN, RIGHT, LEFT, I, K, J, L };
bool keyPressed[MAX_KEYS];

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

// Define the keyboard handler
class KeyboardHandler : public ViewerGlutOGL::KbHandler
{
    public:
        KeyboardHandler() {
            for (unsigned int i = 0; i < MAX_KEYS; ++i)
                keyPressed[i] = false;
        }
        virtual void OnKeyDown(int key) {
            switch (key) {
                case KEY_LEFT:
                    keyPressed[LEFT] = true;
                    break;
                case KEY_RIGHT:
                    keyPressed[RIGHT] = true;
                    break;
                case KEY_UP:
                    keyPressed[UP] = true;
                    break;
                case KEY_DOWN:
                    keyPressed[DOWN] = true;
                    break;
                case 'i':
                    keyPressed[I] = true;
                    break;
                case 'k':
                    keyPressed[K] = true;
                    break;
                case 'j':
                    keyPressed[J] = true;
                    break;
                case 'l':
                    keyPressed[L] = true;
                    break;
            }
        }
        virtual void OnKeyUp(int key) {
            switch (key) {
                case KEY_LEFT:
                    keyPressed[LEFT] = false;
                    break;
                case KEY_RIGHT:
                    keyPressed[RIGHT] = false;
                    break;
                case KEY_UP:
                    keyPressed[UP] = false;
                    break;
                case KEY_DOWN:
                    keyPressed[DOWN] = false;
                    break;
                case 'i':
                    keyPressed[I] = false;
                    break;
                case 'k':
                    keyPressed[K] = false;
                    break;
                case 'j':
                    keyPressed[J] = false;
                    break;
                case 'l':
                    keyPressed[L] = false;
                    break;
            }
        }
    private:
};

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : rotPtr (NULL), soldadoRotacaoPos(0) { }
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            bool someKeyIsPressed = false;
            if (keyPressed[UP] || keyPressed[I]) {
                translPtr->GetTranslation(&soldadoPos);
                double speed = 0.01;
                double frontX = sin(soldadoRotacaoPos) * speed;  
                double frontZ = cos(soldadoRotacaoPos) * speed;  
                
                translPtr->MakeTranslation(soldadoPos + Point4D(frontX, 0, frontZ, 0));
                rotPtr->MakeYRotation(soldadoRotacaoPos);
                someKeyIsPressed = true;
            }
            if (keyPressed[DOWN] || keyPressed[K]) {
                translPtr->GetTranslation(&soldadoPos);
                double speed = 0.01;
                double backX = -sin(soldadoRotacaoPos) * speed;  
                double backZ = -cos(soldadoRotacaoPos) * speed;  
                
                translPtr->MakeTranslation(soldadoPos + Point4D(backX, 0, backZ, 0));
                rotPtr->MakeYRotation(soldadoRotacaoPos);
                someKeyIsPressed = true;
            }
            if (keyPressed[LEFT] || keyPressed[J]) {
                soldadoRotacaoPos += 0.02;
                rotPtr->MakeYRotation(soldadoRotacaoPos);
                translPtr->GetTranslation(&soldadoPos);
                translPtr->MakeTranslation(soldadoPos);
                someKeyIsPressed = true;
            }
            if (keyPressed[RIGHT] || keyPressed[L]) {
                soldadoRotacaoPos -= 0.02;
                rotPtr->MakeYRotation(soldadoRotacaoPos);
                translPtr->GetTranslation(&soldadoPos);
                translPtr->MakeTranslation(soldadoPos);
                someKeyIsPressed = true;
            }
            if (someKeyIsPressed)
                viewerPtr->PostRedisplay();
        }
        Transform* translPtr;
        Transform* rotPtr;
        double soldadoRotacaoPos;
    protected:
    private:
        Point4D soldadoPos;
};

const char fileName[] = "marine/marine.obj";

int main(int argc, char* argv[]) {

    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)
    // create a camera (scene observer)
    VART::Camera camera(Point4D(0,0, 6), Point4D::ORIGIN(), Point4D::Y());
    //MyIHClass idleHandler;

    //ClickHandlerClass clickHandler;

    // Build up the scene -- begin =======================================================
    // scene -> base
    VART::Box base;
    base.MakeBox(-3,3,-0.1,0.1,-2,2);
    base.SetMaterial(VART::Material::DARK_PLASTIC_GRAY());
    scene.AddObject(&base);

    Transform soldadoTransl;
    soldadoTransl.MakeIdentity();
    scene.AddObject(&soldadoTransl);

    Transform soldadoRot;
    soldadoRot.MakeIdentity();
    soldadoTransl.AddChild(soldadoRot);

    Transform soldadoScale;
    soldadoScale.MakeScale(0.75, 0.75, 0.75);
    soldadoRot.AddChild(soldadoScale);

    Transform rotInicialSoldado;
    rotInicialSoldado.MakeXRotation(-(M_PI / 2));
    soldadoScale.AddChild(rotInicialSoldado);

    double soldRotacaoValor = 0;

    list<VART::MeshObject*> objects;
    list<VART::MeshObject*>::iterator it;

    // Load a object from a ".obj" file. Note that the object refers to a material
    // library, that refers to two other textures.
    if( VART::MeshObject::ReadFromOBJ(fileName, &objects)) {
        // Here, we add the object data to a transform, setup the data and the transform.
        for(it = objects.begin(); it != objects.end(); ++it) {
            (*it)->autoDelete = true;
            rotInicialSoldado.AddChild( **it );
        }
    } else
        cout << "Could not read '" << fileName << "' for object data." << std::endl;

   // scene.AddObject( &soldadoTrans );
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    // Set up the viewer
    viewer.SetTitle("V-ART pick example");
    viewer.SetScene(scene); // attach the scene

    KeyboardHandler kbh; // create a keyboard handler
    viewer.SetKbHandler(&kbh); // attach the keyboard handler
    MyIHClass idh;
    idh.rotPtr = &soldadoRot;
    idh.soldadoRotacaoPos = soldRotacaoValor;
    idh.rotPtr = &soldadoRot;
    idh.translPtr = &soldadoTransl;
    viewer.SetIdleHandler(&idh);

    glEnable( GL_TEXTURE_2D );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)

    return 0;
}