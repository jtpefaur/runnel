#include "patternsdata.h"

PatternsData::PatternsData()
{
}

std::vector<QString> PatternsData::names_patterns = PatternsData::fillNamesPatterns();
std::vector<QString> PatternsData::descriptions = PatternsData::fillDescriptionPatterns();
std::vector<QString> PatternsData::images = PatternsData::fillNamesImages();

std::vector<QString> PatternsData::fillNamesPatterns(){
    std::vector<QString> names;
    names.push_back("Dendrítico");
    names.push_back("Paralelo");
    names.push_back("Enrejado");
    names.push_back("Rectangular");
    return names;
}

std::vector<QString> PatternsData::fillDescriptionPatterns(){
    std::vector<QString> description;
    description.push_back("Es la forma más común de los sistema de drenaje, posee muchas corrientes que contribuyen al río principal. Su forma principal es análoga a las ramas de un árbol frondoso. El tipo de roca que desarrolla el patrón presen ta una resistencia a la erosión uniforme, siendo la roca impermeable y no porosa.");
    description.push_back("Este patrón es causado por fuertes pendientes con algunos relieves, debido a esto las corrientes son rápidas y rectas, con poco afluente de agua y una misma dirección de movimiento. También este patrón está presente en fallas grandes de las rocas.");
    description.push_back("Es similar al patrón rectangular, con la diferencia de que presenta rocas  que  difieren  en  su  resistencia  a  la  erosión  en  el  subsuelo.  También  se  puede encontrar este tipo de redes en áreas de fracturas paralelas.");
    description.push_back("Patrón que se origina en rocas que son uniformes en la resistencia a la erosión  con  2  tipos  de  dirección  del  afluente  del  agua.  Además,  estas  rocas  están cruzadas por fracturas casi perpendiculares entre sí.");
    return description;
}


std::vector<QString> PatternsData::fillNamesImages(){
    std::vector<QString> images_name;
    images_name.push_back(":images/patterns_image/dendritico.png");
    images_name.push_back(":images/patterns_image/paralelo.png");
    images_name.push_back(":images/patterns_image/enrejado.png");
    images_name.push_back(":images/patterns_image/rectangular.png");
    return images_name;
}
