#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include "AnimalManager.h"

void outputAnimalData(const Animal& animal);

struct Capture {
    std::istringstream input;
    std::ostringstream output;
    std::streambuf* oldInput;
    std::streambuf* oldOutput;
    explicit Capture(const std::string& text) : input(text) {
        std::cin.clear(); oldInput=std::cin.rdbuf(input.rdbuf()); oldOutput=std::cout.rdbuf(output.rdbuf());
    }
    ~Capture(){std::cin.rdbuf(oldInput);std::cout.rdbuf(oldOutput);std::cin.clear();}
};

std::string json(const std::string& value){
    std::string out="\"";
    for(unsigned char c:value){switch(c){
      case '\\':out+="\\\\";break;case '"':out+="\\\"";break;
      case '\n':out+="\\n";break;case '\r':out+="\\r";break;case '\t':out+="\\t";break;
      default:out+=c;}}
    return out+'"';
}

Animal makeAnimal(int id,const std::string& name,const std::string& species,const std::string& gender="Самець"){
    Animal a;
    a.nameOfAnimal=name;a.typeOfAnimal=species;a.animalIdentificationNumber=id;a.genderOfAnimal=gender;
    a.ageOfAnimal=8;a.enclosureID=12;a.lastFeeding="12/11/26 10:30";
    a.healthStatus="Здоровий";a.lastVeterinarCheckDate="11/11/26";
    a.treatmentPlan="Спостереження";a.veterinaryExaminationResult="Без зауважень";
    return a;
}

std::string state(const Animal&a){
    return json(a.nameOfAnimal)+json(a.typeOfAnimal)+std::to_string(a.animalIdentificationNumber)+json(a.genderOfAnimal)
      +std::to_string(a.ageOfAnimal)+json(a.healthStatus)+std::to_string(a.enclosureID)+json(a.lastFeeding)
      +json(a.lastVeterinarCheckDate)+json(a.treatmentPlan)+json(a.veterinaryExaminationResult);
}
std::string state(const std::vector<Animal>&a){std::string s;for(const auto&v:a)s+="["+state(v)+"]";return s;}
std::string displayFields(const Animal&a){
    std::vector<std::string> fields={std::to_string(a.animalIdentificationNumber),a.nameOfAnimal,a.typeOfAnimal,
        a.genderOfAnimal,std::to_string(a.ageOfAnimal),std::to_string(a.enclosureID),a.lastFeeding};
    std::string s="[";for(size_t i=0;i<fields.size();++i){if(i)s+=",";s+=json(fields[i]);}return s+"]";
}

struct Result {std::string id,kind,output,expected;bool unchanged;};
int main(){
    const Animal shortOne=makeAnimal(10000001,"Лео","Лев");
    const Animal shortTwo=makeAnimal(10000002,"Мія","Тигр","Самка");
    const Animal longOne=makeAnimal(10000001,"Абдул-Аль-Сумафер","Південноафриканський жираф");
    const Animal longTwo=makeAnimal(10000002,"Абдул-Аль-Сумафер","Південноафриканський жираф","Самка");
    std::vector<Result> results;
    auto list=[&](std::string id,std::vector<Animal>a){
        const auto before=state(a);std::string expected="[";
        for(size_t i=0;i<a.size();++i){if(i)expected+=",";expected+=displayFields(a[i]);}expected+="]";
        Capture c("");showAnimalList(a);results.push_back({id,"list",c.output.str(),expected,before==state(a)});
    };
    list("L01",{shortOne,shortTwo});
    list("L02",{longOne,shortTwo});
    list("L03",{shortOne,longTwo});
    list("L04",{});
    {
        Animal a=longOne;const auto before=state(a);Capture c("");outputAnimalData(a);
        results.push_back({"O01","single",c.output.str(),"["+displayFields(a)+"]",before==state(a)});
    }
    {
        auto a=std::vector<Animal>{longOne,shortTwo};const auto before=state(a);
        Capture c("10000001\n");searchAnimalByIdentificationNumber(a);
        results.push_back({"S01","search_found",c.output.str(),"["+displayFields(longOne)+"]",before==state(a)});
    }
    {
        auto a=std::vector<Animal>{shortOne,shortTwo};const auto before=state(a);
        Capture c("10000003\n");searchAnimalByIdentificationNumber(a);
        results.push_back({"S02","search_absent",c.output.str(),"[]",before==state(a)});
    }
    {
        std::vector<Animal>a;const auto before=state(a);Capture c("");searchAnimalByIdentificationNumber(a);
        results.push_back({"S03","search_empty",c.output.str(),"[]",before==state(a)});
    }
    std::cout<<"[\n";
    for(size_t i=0;i<results.size();++i){auto&r=results[i];
        std::cout<<"{\"id\":"<<json(r.id)<<",\"kind\":"<<json(r.kind)<<",\"output\":"<<json(r.output)
          <<",\"expected_fields\":"<<r.expected<<",\"unchanged\":"<<(r.unchanged?"true":"false")<<"}";
        if(i+1<results.size())std::cout<<",";
        std::cout<<"\n";
    }
    std::cout<<"]\n";
}
