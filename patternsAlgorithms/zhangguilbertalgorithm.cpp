#include "zhangguilbertalgorithm.h"
#include <iostream>

#define PI 3.14159265

ZhangGuilbertAlgorithm::ZhangGuilbertAlgorithm():
    AlgorithmPatron()
{
    shader = 0;
}

ZhangGuilbertAlgorithm::~ZhangGuilbertAlgorithm(){
    if(shader){
        delete shader;
    }
}

void ZhangGuilbertAlgorithm::run(Terrain *ter, std::vector<arbol*> &drainage_tree){
    terreno = ter;
    drainage_trees = drainage_tree;
    std::vector<std::string> result = this->runAlgoritm();
    shader->fillPositionBuffer(drainage_trees, result);
}

void ZhangGuilbertAlgorithm::render(glm::mat4 matrix, float exag_z){
    if(shader){
        shader->render(matrix, exag_z);
    }
}

void ZhangGuilbertAlgorithm::glewReady(){
    shader = new ShaderPatron();
}

QString ZhangGuilbertAlgorithm::getName(){
    return QString("Zhang Guilbert");
}

QWidget* ZhangGuilbertAlgorithm::getConf(){
    return &conf;
}


std::vector<std::string> ZhangGuilbertAlgorithm::runAlgoritm(){
    std::vector<std::string> result;
    float delta = 10;
    float sinuodelta = 0.20;
    std::cout << "cantidad de redes " << this->drainage_trees.size() << std::endl;
    for(arbol* ar: this->drainage_trees){
        if(ar->hijos.size() == 0){
            result.push_back("");
            continue;
        }
        float angle_union = this->angleUnion(ar);
        float sinuo = this->sinuosity(ar);
        float lenght_ratio = this->lenghtRatio(ar);
        float catchment_elong = this->catchmentElongation(ar);
        std::cout << "=============="<<std::endl;

        std::cout << "angle_union " << angle_union << std::endl;
        std::cout << "sinuo " << sinuo << std::endl;
        std::cout << "lenght_ratio " << lenght_ratio << std::endl;
        std::cout << "catchment_elong " << catchment_elong << std::endl;
        std::string value = "";
        if( angle_union < (90 + delta) && angle_union > (90 - delta)){
            if(sinuo > (1 - sinuodelta)){
                value = "rectangular";
            }else{
                if(sinuo < (0 + sinuodelta)){
                    if(lenght_ratio < (1 - 0.2)){
                        if(catchment_elong > 1.1){
                            value = "trellis";
                        }
                    }
                }
            }
        }

         if(catchment_elong > (1+sinuodelta) && lenght_ratio > (1-delta) && sinuo > (1-sinuodelta) && sinuo < (1+sinuodelta)){
             if(angle_union < 160-delta){
                 value = "parallel";
             }
         }

         if(angle_union < (90 - delta) && catchment_elong < 1+ sinuodelta){

            value = "dendritic";
         }
         std::cout << "tipo de red " << value << std::endl;
        result.push_back(value);

    }

    return result;

}


float ZhangGuilbertAlgorithm::angleUnion(arbol* ar){
   float cant_angles = 0;
   float angle = 0;
   while(ar->hijos.size() > 0){
      // std::cout << "cant hijos " << ar->hijos.size() << std::endl;
       arbol* main_path = this->selectPrincipalStream(ar);
       if(ar->hijos.size() > 1){

           for(arbol* child : ar->hijos){
               bool is_same = child->pto == main_path->pto;
             //  std::cout << "es igual " << is_same << std::endl;
               if(!is_same){
                   float a = glm::distance(ar->pto->coord, main_path->pto->coord);
                   float b = glm::distance(ar->pto->coord, child->pto->coord);
                   float c = glm::distance(main_path->pto->coord, child->pto->coord);
                   float value = (a*a + b*b - c*c)/(2*a*b);
                   angle += std::acos(value)* 180.0 / PI;
                   cant_angles++;
               }
           }
       }
       ar = main_path;
   }
   if(cant_angles == 0){
       cant_angles = 1;
   }
   return angle/cant_angles;
}


float ZhangGuilbertAlgorithm::sinuosity(arbol* ar){
    glm::vec3 first_point = ar->pto->coord;
    glm::vec3 last_point = glm::vec3(0,0,0);
    float dist = 0;
    while(ar->hijos.size()> 0){
        arbol* main_path = this->selectPrincipalStream(ar);
        last_point = main_path->pto->coord;
        dist += glm::distance(main_path->pto->coord, ar->pto->coord);
        ar = main_path;
    }
    float dist_pf = glm::distance(first_point, last_point);
    return dist/dist_pf;
}


float ZhangGuilbertAlgorithm::lenghtRatio(arbol* ar){
    float sum_dist_main = 0;
    float cant_fluente = 0;
    float prom_afluente = 0;
    float sum_dist_fluente = 0;
    if (ar->hijos.size() == 0) return -1;
    while (ar->hijos.size()>0) {
        arbol* main_path = this->selectPrincipalStream(ar);
        if(ar->hijos.size()> 1){
            for(arbol* child: ar->hijos){
                if(main_path->pto!=child->pto){
                    cant_fluente++;
                    sum_dist_fluente += glm::distance(child->pto->coord, ar->pto->coord) + this->lenghtFluente(child);
                }
            }
        }
        sum_dist_main+= glm::distance(main_path->pto->coord, ar->pto->coord);
        ar = main_path;
    }
    if(cant_fluente == 0){
        cant_fluente = 1;
    }
    prom_afluente = sum_dist_fluente/cant_fluente;
    return prom_afluente/sum_dist_main;
}


float ZhangGuilbertAlgorithm::catchmentElongation(arbol* ar){
    float max_float = std::numeric_limits<float>::max();
    float min_float = -std::numeric_limits<float>::max();
    float q_min =  max_float;
    float q_max =  min_float;
    float p_min =  max_float;
    float p_max =  min_float;

    std::vector<glm::vec3> puntos;
    glm::vec3 min_values = glm::vec3(max_float,max_float,max_float);
    glm::vec3 max_values = glm::vec3(min_float,min_float,min_float);

    puntos.push_back(ar->pto->coord);
    this->getPoints(ar, puntos);
    //direction
    glm::vec3 final_point = this->getFinalPoint(ar);

    float square_unit_y = this->vectorEqualDirection(glm::vec2(ar->pto->coord), glm::vec2(final_point), glm::vec2(0,0), glm::vec2(0,1));
    float square_unit_x = this->vectorEqualDirection(glm::vec2(ar->pto->coord), glm::vec2(final_point), glm::vec2(0,0), glm::vec2(1,0));
    float rect_unit_x = this->vectorEqualDirection(glm::vec2(ar->pto->coord), glm::vec2(final_point), glm::vec2(0,0), glm::vec2(1,1));
    float rect_unit_y = this->vectorEqualDirection(glm::vec2(ar->pto->coord), glm::vec2(final_point), glm::vec2(0,0), glm::vec2(-1,1));

    for(glm::vec3 data: puntos){
        min_values = glm::min(data, min_values);
        max_values = glm::max(data, max_values);
        float value_p = data.x + data.y;
        float value_q = data.x - data.y;
        if(value_p < p_min){
            p_min = value_p;
        }
        if(value_p > p_max){
            p_max = value_p;
        }
        if(value_q < q_min){
            q_min = value_q;
        }
        if(value_q > q_max){
            q_max = value_q;
        }
    }
    glm::vec2 p1 = glm::vec2((p_min+q_max)/2, (p_min-q_max)/2);
    glm::vec2 p2 = glm::vec2((p_max+q_max)/2, (p_max-q_max)/2);
    glm::vec2 p4 = glm::vec2((p_min+q_min)/2, (p_min-q_min)/2);
    float e2 = glm::distance(p2, p1);
    float e1 = glm::distance(p4, p1);
    float square_y = max_values.y - min_values.y;
    float square_x = max_values.x - min_values.x;
    float rec_value = 0;
    float square_value = 0;
    if(rect_unit_x > rect_unit_y){
        if (e1 == 0) e1 = 1;
        rec_value = e2/e1;
    }else{
        if (e2 == 0) e2 = 1;
        rec_value = e1/e2;
    }
    if(square_unit_x > square_unit_y){
        if (square_y == 0) square_y = 1;
        square_value = square_x/square_y;
    }else{
        if (square_x == 0) square_x = 1;
        square_value = square_y/square_x;
    }
    float final_value = this->chooseBestValue(square_value, rec_value);
    return final_value;
}

float ZhangGuilbertAlgorithm::chooseBestValue(float sq, float re){
    float sq_dif = 1 - sq;
    if (sq_dif < 0){
        sq_dif = -sq_dif;
    }
    float re_dif = 1 - re;
    if(re_dif < 0){
        re_dif = -re_dif;
    }
    float mayor_dif = std::max(re_dif, sq_dif);
    if( re_dif == mayor_dif){
        return re;
    }else{
        return sq;
    }
}

glm::vec3 ZhangGuilbertAlgorithm::getFinalPoint(arbol *ar){
    glm::vec3 final = glm::vec3();
    while (ar->hijos.size()>0) {
        arbol* main_path = this->selectPrincipalStream(ar);
        final = main_path->pto->coord;
        ar = main_path;
    }
    return final;
}
float ZhangGuilbertAlgorithm::vectorEqualDirection(glm::vec2 ini, glm::vec2 fin, glm::vec2 ini_com, glm::vec2 fin_com){
    glm::vec2 vec1 = glm::normalize(fin-ini);
    glm::vec2 vec2 = glm::normalize(fin_com - ini_com);
    return glm::dot(vec1, vec2);
}

void ZhangGuilbertAlgorithm::getPoints(arbol* ar, std::vector<glm::vec3>& pointss){
    for(arbol* child: ar->hijos ){
        pointss.push_back(child->pto->coord);
        this->getPoints(child, pointss);
    }
    return;
}

float ZhangGuilbertAlgorithm::lenghtFluente(arbol* padre){
    float distancia = 0;
    while(padre->hijos.size()> 0){
        arbol* child = this->selectPrincipalStream(padre);
        distancia+= glm::distance(padre->pto->coord, child->pto->coord);
        padre = child;
    }
    return distancia;
}


arbol* ZhangGuilbertAlgorithm::selectPrincipalStream(arbol* father){
    int number = father->number_strahler_horton;
    std::vector<arbol*> child_path;
    for(arbol* child : father->hijos){
        if( child->number_strahler_horton == number){
            child_path.push_back(child);
        }
    }
    if (child_path.size() == 0){
        for(arbol* child : father->hijos){
            if( child->number_strahler_horton == (number - 1)){
                child_path.push_back(child);
            }
        }
    }
    arbol* main_path = 0;
    if(child_path.size() == 1){
         main_path = child_path[0];
    }else{
       if(child_path.size() > 1){
           float dist_major = 0;
           for(arbol* chil: child_path){
               float dis = glm::distance(chil->pto->coord, father->pto->coord);
               if(dist_major < dis){
                   main_path = chil;
                   dist_major = dis;
               }
           }
       }else{
           std::cout << "Error with getMainArbol " << std::endl;
       }
    }
    return main_path;
}

std::vector<glm::vec3> ZhangGuilbertAlgorithm::getPathTree(){
    std::vector<glm::vec3> path;
    path.clear();
    for( arbol* ar: this->drainage_trees){
        std::vector<glm::vec3> path_edge;
        ar->getArbolEdges(path_edge);
        path.insert( path.end(), path_edge.begin(), path_edge.end() );
        path_edge.clear();
    }
    return path;
}

