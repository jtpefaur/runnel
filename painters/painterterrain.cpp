#include "painterterrain.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "painters/openglutils.h"
#include "lib/glm/gtc/matrix_transform.hpp"



PainterTerrain::PainterTerrain():
    GLWidget()
{
    ter = (Terrain*)0;
    exag_z = 1.0f;
    this->initColorBuffer();
    drainage_algorithm = 0;
    pattern_algorithm = 0;
    water_algorithm = 0;
    build_network = 0;
    shader_terrain = 0;
    glew_initialized = false;
}

PainterTerrain::~PainterTerrain() {
    if(shader_terrain)
        delete shader_terrain;
}


std::unordered_map<std::string, glm::vec3> PainterTerrain::confColor(){
    return color_conf;
}

void PainterTerrain::initGL()
{
    if(glew_initialized)
        return;
    GLenum err = glewInit();
    if (GLEW_OK != err){
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    emit glewIsReady();
    glew_initialized = true;
}

void PainterTerrain::setTerrain(Terrain* t)
{
    ter = t;
    PainterTerrain::InitializeVertexBuffer();

    ortho_matrix = glm::ortho<GLfloat>(-1.0f, 1.0f, -1.0f, 1.0f, -10.f, 10.f);
    glm::mat4 matrix = glm::mat4();
    float max_sigma = std::max(ter->sigma.x, ter->sigma.y);
    max_sigma = std::max(max_sigma, ter->sigma.z);
    matrix = glm::scale(matrix, glm::vec3(1,1,1)/max_sigma);
    matrix = t->matrix_esc * matrix;
    matrix = glm::translate(matrix, -ter->media);
    this->GLWidget::model_matrix = matrix;
}

void PainterTerrain::InitializeVertexBuffer()
{
    shader_terrain = new ShaderTerrain();
    std::vector<glm::vec3> vertex_position_terrain = ter->getVectorPoints();
    shader_terrain->fillPositionBuffer(vertex_position_terrain);
}

void PainterTerrain::paintGL(){
    this->GLWidget::paintGL();
    if(!ter){
        std::cout << "No hay terreno para pintar"<<std::endl;
        return;
    }
    this->GLWidget::model_view_matrix = glm::translate(glm::mat4(),glm::vec3(0.0f,0.0f, -5.0f))*this->GLWidget::matrix_glwidget*this->GLWidget::model_matrix;
    matrix_final = ortho_matrix*this->GLWidget::model_view_matrix;

    shader_terrain->render(matrix_final, exag_z, ter->max_bounding.z, color_conf["shader_terrain_color"]);
    if( drainage_algorithm ){
        drainage_algorithm->render(matrix_final, exag_z, color_conf["shader_callaghan_color"]);
    }
    if( build_network ){
        build_network->render(matrix_final, exag_z, color_conf["shader_callaghan_color"]);
    }

    OpenGLUtils::printOpenGLError();
}

void PainterTerrain::mouseDoubleClickEvent(QMouseEvent * event){
    this->ObtainPositionFromView(event->x(), event->y(),this->GLWidget::model_view_matrix, ortho_matrix);
}


void PainterTerrain::ObtainPositionFromView(int x, int y, glm::mat4 view, glm::mat4 projection){
    int window_width = this->width();
    int window_height = this->height();
    GLfloat depth;
    glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glm::vec4 viewport = glm::vec4(0, 0, window_width, window_height);
    glm::vec3 wincoord = glm::vec3(x, window_height - y - 1, depth);
    glm::vec3 objcoord = glm::unProject(wincoord, view, projection, viewport);
}

void PainterTerrain::changeModelTerrain(int index){
    switch (index) {
        case MALLA:
            this->GLWidget::drawing_mode = this->GLWidget::WIREFRAME;
            break;
        case POLIGONO:
            this->GLWidget::drawing_mode = this->GLWidget::FILL_POLYGONS;
            break;
        case POINTS:
            this->GLWidget::drawing_mode = this->GLWidget::NUBE_PUNTOS;
            break;
        default:
            break;
    }


}

void PainterTerrain::changeExag(int number){
    exag_z = number*1.0f;
}

void PainterTerrain::setDrainageAlgorithm(DrainageAlgorithms *da){
    drainage_algorithm = da;
}

void PainterTerrain::setPatternAlgorithm(AlgorithmPatron *da){
    pattern_algorithm = da;
}

void PainterTerrain::setWaterAlgorithm(PathWaterAlgorithm *da){
    water_algorithm = da;
}

void PainterTerrain::setNetworkAlgorithm(BuildNetwork* alg){
    build_network = alg;
}

// TODO: Change the other file color configuration
bool PainterTerrain::checkExists(std::string file)
{
    std::ifstream file_to_check (file.c_str());
    if(file_to_check.is_open())
      return true;
    return false;
}



//TODO, Change to when change attr only this is overwritten
void PainterTerrain::changeConf(){
    std::ofstream conf_file;
    conf_file.open ("conf_runnel.txt");
    if (conf_file.is_open()){
        for ( auto it = color_conf.begin(); it != color_conf.end(); ++it )
           conf_file << it->first << " " << it->second.x << " " << it->second.y << " " << it->second.z << "\n";
    }

    conf_file.close();
}

void PainterTerrain::initColorBuffer(){
    if(checkExists("conf_runnel.txt")){
        std::ifstream conf_file;
        std::string line;
        std::string name;
        float x; float y; float z;
        conf_file.open ("conf_runnel.txt");
        while ( std::getline (conf_file, line) ){
            std::istringstream  iss(line);
            iss >> name;
            iss >> x;
            iss >> y;
            iss >> z;
            color_conf[name] = glm::vec3(x,y,z);
        }
        conf_file.close();
    }else{
        color_conf["shader_edge_color_min"] = glm::vec3(0,1,0);
        color_conf["shader_edge_color_max"] = glm::vec3(1,0,0);
        color_conf["shader_callaghan_color"] = glm::vec3(0,0.5,1);
        color_conf["shader_point_color"] = glm::vec3(0,0,0.8);
        color_conf["normal_color"] = glm::vec3(0.0f, 0.0f, 0.0f);
        color_conf["shader_terrain_color"] = glm::vec3(1.0f,1.0f,1.0f);
        color_conf["color_gradient_path"] = glm::vec3(0.0f,0.3f,0.0f);
        color_conf["shader_drainage_color"] = glm::vec3(0.0f,0.3f,0.0f);
        color_conf["shader_drainage_color_peucker"] = glm::vec3(0.0f,0.3f,0.0f);
    }
}

void PainterTerrain::changeAttrConf(std::string name, glm::vec3 value){
    color_conf[name] = value;
    this->changeConf();
}
