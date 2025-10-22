#include <vart/scene.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/point4d.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;
using namespace VART;

MeshObject objectReader(ifstream &arq){
    //Cria as cordenadas de um ponto
    double X;
    double Y;
    double Z;
    int points;
    //Lê a quantidade de pontos
    arq >> points;
    //Cria um vector com os pontos
    vector<Point4D> vetor;
    vetor.reserve(points);
    for(int i = 0;i < points;++i){
        arq >> X;
        arq >> Y;
        arq >> Z;
        vetor.push_back(Point4D(X, Y, Z));
    }
    MeshObject meshObject;
    meshObject.SetVertices(vetor);

    //Captura as faces do objeto
    string line;
    string numero;
    while(getline(arq, line)){
        stringstream ss(line);
        //Cria o mesh
        Mesh mesh;
        //Define o tipo de mesh
        mesh.type = Mesh::MeshType::POLYGON;
        int vertice;
        while (ss >> vertice and vertice != -1)
            mesh.indexVec.push_back(vertice);
        
        //Define a cor da face
        mesh.material = Color::RANDOM();
        //Adiciona no meshObject
        meshObject.AddMesh(mesh);
        //Reinicia o mesh
        mesh.indexVec.clear();
    }
    //Calcula os vértices
    meshObject.ComputeVertexNormals();
    return meshObject;
}

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT

    if (argc < 2) {
        cerr << "Fatal error: No filename given.\nUsage: " << argv[0] << " filename\n";
        return 1;
    }

    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0));
    
    //Abre o arquivo
    ifstream arq(argv[1]);
    if(!arq.is_open()){
        cerr << "Não foi possível abrir o arquivo " << argv[1] << endl;
        return 1;
    }
    //Cria o MeshObject
    MeshObject meshObject(objectReader(arq));
    arq.close();

    //Adiciona o objeto na cena
    scene.AddObject(&meshObject);
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.MakeCameraViewAll();

    // Set up the viewer
    viewer.SetTitle("V-ART OBJ Viewer");
    viewer.SetScene(scene); // attach the scene

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
