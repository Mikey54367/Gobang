#include <iostream>
#include <jsoncpp/json/json.h>
#include <sstream>
std::string Serialize()
{
    //1.create Json::Value object
    Json::Value root;
    root["name"] = "Mike";
    root["age"] = 18;
    root["score"].append(90);
    root["score"].append(90.5);
    root["score"].append(88.0);
    //2.create Json::WriterBuilder
    Json::StreamWriterBuilder swb;
    //3.create Json::StreamWriter
    Json::StreamWriter* pw =swb.newStreamWriter();
    //4.use writer to serialize
    std::stringstream ss;
    int ret=pw->write(root,&ss);
    if(ret!=0)
    {
        std::cerr<<"serialize failed"<<std::endl;
        return "";
    }
    //5.show result
    delete pw;
    return ss.str();
}
void Deserialze(const std::string& str)
{
    //1.create readerBuilder
    Json::CharReaderBuilder crb;
    //2.create reader
    Json::CharReader* crp =crb.newCharReader();
    //3. create Json::Value object
    Json::Value root;
    //4.deserialize
    std::string err;
    bool ret=crp->parse(str.c_str(),str.c_str()+str.size(),&root,&err);
    if(ret==false)
    {
        std::cout<<"Deserialze failed: "<<err<<std::endl;
        return;
    }
    //5.Extract data
    std::cout<<"name: "<<root["name"].asString()<<std::endl;
    std::cout<<"age: "<<root["age"].asInt()<<std::endl;
    int sz=root["score"].size();
    for(int i=0;i<sz;++i)
    {
        std::cout<<"score: "<<root["score"][i].asDouble()<<std::endl; 
    }
    delete crp;
}
int main()
{
    std::string str=Serialize();
    Deserialze(str);
    return 0;
}