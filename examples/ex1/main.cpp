#include <vart/scene.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/point4d.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace VART;

MeshObject criarCubo(Material mat) {
    MeshObject obj;

    Point4D p1(-1.0, -1.0, 1.0);   
    Point4D p2(1.0, -1.0, 1.0); 
    Point4D p3(1.0, 1.0, 1.0);  
    Point4D p4(-1.0, 1.0, 1.0);
    Point4D p5 (-1.0, -1.0, -1.0);
    Point4D p6 (1.0, -1.0, -1.0);
    Point4D p7 (1.0, 1.0, -1.0);
    Point4D p8 (-1.0, 1.0, -1.0);

    vector<Point4D> vertices;

    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    vertices.push_back(p4);
    vertices.push_back(p5);
    vertices.push_back(p6);
    vertices.push_back(p7);
    vertices.push_back(p8);
    
    obj.SetVertices(vertices);

    //Cria o mesh
    Mesh mesh1, mesh2, mesh3, mesh4, mesh5, mesh6;
    //Define o tipo de mesh
    mesh1.type = Mesh::MeshType::POLYGON;
    mesh1.indexVec.push_back(0);
    mesh1.indexVec.push_back(1);
    mesh1.indexVec.push_back(2);
    mesh1.indexVec.push_back(3);
    mesh1.material = Color::CYAN();

    mesh2.type = Mesh::MeshType::POLYGON;
    mesh2.indexVec.push_back(4);
    mesh2.indexVec.push_back(5);
    mesh2.indexVec.push_back(1);
    mesh2.indexVec.push_back(0);
    mesh2.material = Color::MAGENTA();

    mesh3.type = Mesh::MeshType::POLYGON;
    mesh3.indexVec.push_back(4); 
    mesh3.indexVec.push_back(7); 
    mesh3.indexVec.push_back(6); 
    mesh3.indexVec.push_back(5); 
    mesh3.material = Color::YELLOW();

    mesh4.type = Mesh::MeshType::POLYGON;
    mesh4.indexVec.push_back(3); 
    mesh4.indexVec.push_back(2); 
    mesh4.indexVec.push_back(6); 
    mesh4.indexVec.push_back(7); 
    mesh4.material = Color::GREEN();

    mesh5.type = Mesh::MeshType::POLYGON;
    mesh5.indexVec.push_back(1); 
    mesh5.indexVec.push_back(5); 
    mesh5.indexVec.push_back(6); 
    mesh5.indexVec.push_back(2); 
    mesh5.material = Color::BLUE();

    mesh6.type = Mesh::MeshType::POLYGON;
    mesh6.indexVec.push_back(0); 
    mesh6.indexVec.push_back(4); 
    mesh6.indexVec.push_back(7); 
    mesh6.indexVec.push_back(3); 
    mesh6.material = Color::RED();
    
    //Adiciona no meshObject        
    obj.AddMesh(mesh1);    
    obj.AddMesh(mesh2);
    obj.AddMesh(mesh3);
    obj.AddMesh(mesh4);
    obj.AddMesh(mesh5);
    obj.AddMesh(mesh6);

    //Adiciona no meshObject        
    obj.ComputeVertexNormals();

    obj.SetMaterial(mat);
    return obj;
}

int main (int argc, char* argv[]) {

    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT

    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D::ORIGIN(),VART::Point4D::Y());


    MeshObject obj = criarCubo(Material::LIGHT_PLASTIC_GRAY());
    //MeshObject obj2 = criarCubo(Material::PLASTIC_BLUE());

    scene.AddObject(&obj);
    //scene.AddObject(&obj);

    /*Transform trans1;
    trans1.MakeTranslation(-1.5, -1.5, 0);
    scene.AddObject(&trans1);
    trans1.AddChild(obj);

    Transform trans2;
    trans2.MakeTranslation(1.5, 0, 0);
    scene.AddObject(&trans2);
    trans2.AddChild(obj2);*/
    
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.MakeCameraViewAll();

    // Set up the viewer
    viewer.SetTitle("CUBO");
    viewer.SetScene(scene); // attach the scene

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;

}