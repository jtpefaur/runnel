#include "painterterrain.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "shaders/shaderutils.h"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "openglutils.h"

#include "drainageAlgorithms/peuckerdrainagenetwork.h"
#include "drainageAlgorithms/gradientdrainagehalfpointnetwork.h"
#include "drainageAlgorithms/gradientdrainagecallaghanmark.h"
#include "waterMovementAlgorithms/buildtreecallaghan.h"
#include "waterMovementAlgorithms/buildtreepeucker.h"
#include "patternsAlgorithms/algorithmpatron.h"
#include "patternsAlgorithms/zhangguilbertalgorithm.h"

PainterTerrain::PainterTerrain(){
    ter = (Terrain*)0;
    min_angle = -10.0f;
    max_angle = 10.0f;
    width_line = 0.05f;
    linewater = 0.003f;
    delta_water = 1/100; //0.07
    exag_z = 1.0f;
    this->initColorBuffer();
}

PainterTerrain::~PainterTerrain() {
    delete shader_terrain;
    delete shader_angle_edge;
    delete shader_callaghan;
    delete shader_drainage;
    delete shader_gradient;
    delete shader_patron;
    delete shader_patron2;
    delete shader_water;
}

void PainterTerrain::changeAttrConf(std::string name, glm::vec3 value){
    color_conf[name] = value;
    this->changeConf();
}
bool PainterTerrain::checkExists(std::string file)
{
    std::ifstream file_to_check (file.c_str());
    if(file_to_check.is_open())
      return true;
    return false;
    file_to_check.close();
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

std::unordered_map<std::string, glm::vec3> PainterTerrain::confColor(){
    return color_conf;
}

void PainterTerrain::initGL()
{
    GLenum err = glewInit();
    if (GLEW_OK != err){
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
}

void PainterTerrain::setTerrain(Terrain* t)
{
    ter = t;
    PeuckerDrainageNetwork::calculateGrid(ter);
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
    shader_drainage = new ShaderDrainage();
    shader_angle_edge = new ShaderAngleEdge();
    shader_callaghan = new ShaderCallaghan();
    shader_gradient = new ShaderGradient();
    shader_patron = new ShaderPatron();
    shader_patron2 = new ShaderPatron();
    shader_water = new ShaderTerrain();

    std::vector<glm::vec3> vertex_position_terrain = ter->getVectorPoints();
    shader_terrain->fillPositionBuffer(vertex_position_terrain);


        std::vector<glm::vec3> normal_triangle = ter->getVectorNormals();
        std::vector<glm::vec3> angle_value_edge = ter->calculateNeightbourByEdges();
        std::vector<glm::vec3> vertex_height = ter->calculateHeigtArray();

        std::vector<glm::vec3> vertex_position_gradient = ter->getGradientDirectionVector();
        std::vector<glm::vec3> vertex_position_gradient_color = ter->vector_gradient_color;
      //  std::cout << "vertex_position_gradient_color " << vertex_position_gradient_color.size() << std::endl;



        std::vector<glm::vec3> vertex_color_drainage_peucker = ter->getDrainageColor();
        std::vector<glm::vec3> vertex_position_drainage_peucker = ter->getPointsEdgeDrainage();





















    GradientDrainageCallaghanMark gdcm =  GradientDrainageCallaghanMark();

    gdcm.runAlgorithm(ter, delta_water );
    ter->getMoreWaterPoint();
    std::vector<float>& count_water = ter->count_water;
 //   std::cout << "count water" << count_water.size() << std::endl;
    std::vector<glm::vec3>& water_position = ter->position_water_points;
  //  std::cout << "water position " << water_position.size() << std::endl;

    //  buildTreeCallaghan ip (ter->struct_point, ter->max_value_water);
      BuildTreePeucker ip(ter->struct_point, ter);
      std::vector<arbol*> arbolines = ip.reviewPoints();

      ZhangGuilbertAlgorithm la;
      AlgorithmPatron& patterns = la;
      patterns.setData(ter, arbolines);
      std::vector<std::string> names_type = patterns.runAlgoritm();
    std::vector<std::string> names_type2;
    names_type2.clear();


    shader_callaghan->fillPositionBuffer(water_position,count_water);
    shader_gradient->fillPositionBuffer(normal_triangle);
    shader_angle_edge->fillPositionBuffer(vertex_position_terrain, angle_value_edge, vertex_height);

    shader_patron->fillPositionBuffer(arbolines, names_type2);
    shader_patron2->fillPositionBuffer(arbolines, names_type);


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

    if( this->GLWidget::normal_view ){//N
        this->drawNormalTerrain();
    }
    if (this->GLWidget::angle_edge){ //A
        this->drawAngleEdge();
    }

    if(this->GLWidget::drainage_callaghan){//C
        this->drawDrainageCallaghan();
    }
    if(this->GLWidget::drainage_view){//D
        this->drawDrainagePeucker();
    }
    if(this->GLWidget::gradient_view){//G
        this->drawGradientVector();
    }

    if(this->GLWidget::arbolito_mode){
        this->drawArbolitos();
    }
    if(this->GLWidget::arbolito_mode_type){
        this->drawArbolitosType();
    }

    this->drawGradientPoint();


    OpenGLUtils::printOpenGLError();
}

void PainterTerrain::drawArbolitos(){
    shader_patron->render(matrix_final,exag_z);
}


void PainterTerrain::drawArbolitosType(){
    shader_patron2->render(matrix_final, exag_z);
}



void PainterTerrain::drawDrainagePeucker(){
    std::vector<glm::vec3> vertex_color_drainage_peucker = ter->getDrainageColor();
    std::vector<glm::vec3> vertex_position_drainage_peucker = ter->getPointsEdgeDrainage();
    shader_drainage->fillPositionBuffer(vertex_position_drainage_peucker, vertex_color_drainage_peucker);
    shader_drainage->render(matrix_final, exag_z, color_conf["shader_drainage_color_peucker"]);
}



void PainterTerrain::drawDrainageCallaghan(){
    shader_callaghan->render(matrix_final, exag_z, linewater,ter->max_value_water, color_conf["shader_callaghan_color"]);
}


void PainterTerrain::drawGradientVector(){
    std::vector<glm::vec3> vertex_position_gradient = ter->getGradientDirectionVector();
    std::vector<glm::vec3> vertex_position_gradient_color = ter->vector_gradient_color;
    shader_drainage->fillPositionBuffer(vertex_position_gradient, vertex_position_gradient_color);
    shader_drainage->render(matrix_final, exag_z, color_conf["shader_drainage_color"]);
}


void PainterTerrain::drawGradientPoint(){
    shader_gradient->render(matrix_final,exag_z,color_conf["color_gradient_path"]);
}

void PainterTerrain::drawAngleEdge(){
    shader_angle_edge->render(matrix_final, exag_z, min_angle, max_angle, width_line, color_conf["shader_edge_color_min"],  color_conf["shader_edge_color_max"]);
}





void PainterTerrain::drawNormalTerrain(){
    shader_gradient->render(matrix_final, exag_z, color_conf["normal_color"]);
}


void PainterTerrain::mouseDoubleClickEvent(QMouseEvent * event){
    this->ObtainPositionFromView(event->x(), event->y(),this->GLWidget::model_view_matrix, ortho_matrix);
}


void PainterTerrain::ObtainPositionFromView(int x, int y, glm::mat4 view, glm::mat4 projection){
    int window_width = this->width();
    int window_height = this->height();
  //  std::cout << "Windows size obtain: "<<window_height<<", "<<window_width<<std::endl;

    GLfloat depth;
    glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
 //   std::cout << "Clicked on pixel "<< x << " " << y << " depth " << depth << std::endl;

    glm::vec4 viewport = glm::vec4(0, 0, window_width, window_height);
    glm::vec3 wincoord = glm::vec3(x, window_height - y - 1, depth);
    glm::vec3 objcoord = glm::unProject(wincoord, view, projection, viewport);

  //  std::cout << "Doble click en (" << x << ", " << y <<", " << depth << ") -> (" << objcoord.x << ", " << objcoord.y << ", " << objcoord.z << ")" << std::endl;
   // this->obtainTriangle(objcoord);
    this->obtainCaidaWater(objcoord);
}



void PainterTerrain::obtainTriangle(glm::vec3 coords){
    float minimum_distance = glm::distance(ter->struct_triangle[0]->incentro,coords);
    runnel::Triangle* triangle_minimum_distance = ter->struct_triangle[0];
    for(runnel::Triangle* trian : ter->struct_triangle){
        float distance = glm::distance(trian->incentro, coords);
        if( distance < minimum_distance){
            triangle_minimum_distance = trian;
            minimum_distance = distance;
        }

    }
    std::vector<glm::vec3> points_gradient;
    GradientDrainageHalfPointNetwork::calculateNextTriangle(triangle_minimum_distance, ter->struct_point, points_gradient);

    shader_water->fillPositionBuffer(points_gradient);
    shader_water->render(matrix_final,exag_z, ter->max_bounding.z, color_conf["color_gradient_path"]);
    this->GLWidget::updateGL();
}

void PainterTerrain::changeValueLineWidth(int number){
    width_line = number/1000.0f;
    this->GLWidget::updateGL();
}

void PainterTerrain::changeValueMaxAngle(QString string_number){
    max_angle = string_number.toFloat();
    this->GLWidget::updateGL();
}

void PainterTerrain::changeValueMinAngle(QString string_number){
    min_angle = string_number.toFloat();
    this->GLWidget::updateGL();
}


void PainterTerrain::obtainCaidaWater(glm::vec3 coords){
    float minimum_distance = glm::distance(glm::vec2(ter->struct_point[0]->coord),glm::vec2(coords));
    runnel::Point* pto_minimum = ter->struct_point[0];
    for(runnel::Point*pto : ter->struct_point){
        float dist = glm::distance(glm::vec2(pto->coord), glm::vec2(coords));
        if(dist < minimum_distance){
            pto_minimum = pto;
            minimum_distance = dist;

        }
    }
    std::vector<glm::vec3> points_gradient;
    new_gota = {};
    std::unordered_map<int, int> id_used;
    this->getPath(pto_minimum, id_used);

    points_gradient = new_gota;
    shader_water->fillPositionBuffer(points_gradient);
    shader_water->render(matrix_final,exag_z, ter->max_bounding.z, color_conf["color_gradient_path"]);

    this->GLWidget::updateGL();
}

void PainterTerrain::getPath(runnel::Point* pto, std::unordered_map<int, int>& id_used){

    std::vector<int> position_neightbour;
    position_neightbour.push_back(pto->ident - ter->width);
    position_neightbour.push_back(pto->ident + ter->width);
    if(pto->ident%ter->width > 0){
        position_neightbour.push_back(pto->ident + ter->width - 1);
        position_neightbour.push_back(pto->ident - ter->width - 1);
        position_neightbour.push_back(pto->ident - 1);
    }
    if(pto->ident%ter->width < ter->width-1){
        position_neightbour.push_back(pto->ident + 1);
        position_neightbour.push_back(pto->ident - ter->width + 1);
        position_neightbour.push_back(pto->ident + ter->width + 1);
    }
  //  std::cout << "getPath " << std::endl;
    float max_z = -1;
    int id_max = -1;
    for (int id : position_neightbour){
        if(id >= 0 && id < ter->struct_point.size()){
            if( ter->struct_point[id]->coord.z <= (pto->coord.z + delta_water )){
                if(id_used[id] == 0){

                    float dist_z = (pto->coord.z + delta_water ) - ter->struct_point[id]->coord.z;
                    if( max_z < dist_z){
                        max_z = dist_z;
                        id_max = id;
                    }
                }else{
                 //   std::cout << "fue usado " << std::endl;
                }
            }
        }
    }
    if(id_max != -1 && id_used[id_max] == 0){
        id_used[id_max] = 1;
        new_gota.push_back(pto->coord);
        new_gota.push_back(ter->struct_point[id_max]->coord);
        this->getPath(ter->struct_point[id_max], id_used);
    }else{
        std::cout << "valor del pto " << id_used[id_max] << " id del punto " << id_max << std::endl;
    }

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
    this->GLWidget::updateGL();

}

void PainterTerrain::changeExag(int number){
    exag_z = number*1.0f;
}

void PainterTerrain::changeToRiver(){
    this->GLWidget::angle_edge = !this->GLWidget::angle_edge;
    this->GLWidget::updateGL();
}

void PainterTerrain::changeLineWater(QString number){
    linewater = number.toFloat();
    this->GLWidget::updateGL();
}

void PainterTerrain::changeDeltaWater(double number){
    delta_water = (float)number/100.0f;
    std::cout << "numero de conf para el delta de agua " << number << std::endl;
}

