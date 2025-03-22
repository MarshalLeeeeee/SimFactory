#include "TypeUtil.h"
#include "FileUtil.h"
#include "ThreadUtil.h"

std::string getTimeStampStr() {
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    return getTimeStampStr(now);
}

std::string getTimeStampStr(const std::chrono::time_point<std::chrono::system_clock> now) {
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    localtime_s(&localTime, &time);
    std::string ts;
    ts += std::to_string(localTime.tm_year + 1900);
    ts += "-";
    auto mon = std::to_string(localTime.tm_mon + 1);
    if (mon.size() == 1) ts += "0";
    ts += mon;
    ts += "-";
    auto day = std::to_string(localTime.tm_mday);
    if (day.size() == 1) ts += "0";
    ts += day;
    ts += "-";
    auto hour = std::to_string(localTime.tm_hour);
    if (hour.size() == 1) ts += "0";
    ts += hour;
    ts += "-";
    auto min = std::to_string(localTime.tm_min);
    if (min.size() == 1) ts += "0";
    ts += min;
    ts += "-";
    auto sec = std::to_string(localTime.tm_sec);
    if (sec.size() == 1) ts += "0";
    ts += sec;
    return ts;
}

bool directoryExists(const char* dir) {
    DWORD ftyp = GetFileAttributesA(dir);
    if (ftyp == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    return (ftyp & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool createDirectory(const char* dir) {
    return CreateDirectoryA(dir, NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}

std::string getVertexFilepath(std::string fileName) {
    std::string p("./res/vertex/");
    p += fileName;
    p += ".xml";
    return p;
}

std::string getModelFilepath(std::string fileName) {
    std::string p("./res/model/");
    p += fileName;
    p += ".xml";
    return p;
}

MeshMeta::MeshMeta(std::string name_, int primitiveType_, std::string vs, std::string ps, std::vector<int>& ids) :
    name(name_), primitiveType(primitiveType_),
    vertexShader(vs), pixelShader(ps) {
    if (!ids.empty()) {
        indiceCnt = ids.size();
        indices = std::shared_ptr<DWORD[]>(new DWORD[indiceCnt]);
        for (uint32_t i = 0; i < indiceCnt; ++i) {
            indices[i] = ids[i];
        }
    }
}

void MeshMeta::setVertexLayout(std::string layout) {
    vertexLayout = layout;
}

std::string MeshMeta::getName() const {
    return name;
}

void ModelMetaBase::addMesh(std::shared_ptr<MeshMeta> pMeshMeta) {
    if (!pMeshMeta) return;
    pMeshMetaVec.emplace_back(pMeshMeta);
}

std::string ModelMetaBase::getVertexLayout() const {
    return vertexLayout;
}

std::shared_ptr<ModelMetaBase> loadVertexFromFile(const char* fileName) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(fileName);
    tinyxml2::XMLElement* vertexData = doc.FirstChildElement("VertexData");
    if (!vertexData)
        return nullptr;
    tinyxml2::XMLElement* vertexLayout = vertexData->FirstChildElement("VertexLayout");
    if (!vertexLayout)
        return nullptr;
    std::string layout(vertexLayout->GetText());
    if (layout == "poscolor") {
        std::shared_ptr<ModelMeta<VertexPosColor>> pModelMeta = std::make_shared<ModelMeta<VertexPosColor>>();
        pModelMeta->parse(vertexData, layout);
        return pModelMeta;
    }
    return nullptr;
}

void loadModelFromFileAsync(const char* fileName, std::function<void(std::shared_ptr<ModelMetaBase>)> callback) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(fileName);
    tinyxml2::XMLElement* model = doc.FirstChildElement("Model");
    if (!model)
        return;
    tinyxml2::XMLElement* vertexData = model->FirstChildElement("VertexData");
    if (!vertexData)
        return;
    std::string vertexFilePath(vertexData->GetText());
    std::shared_ptr<ModelMetaBase> pModelMeta = loadVertexFromFile(getVertexFilepath(vertexFilePath).c_str());
    if (!pModelMeta)
        return;

    for (tinyxml2::XMLNode* mesh = model->FirstChildElement("Meshes")->FirstChild(); mesh; mesh = mesh->NextSibling()) {
        std::string meshName(mesh->FirstChildElement("Name")->GetText());
        int primitiveType = 0;
        mesh->FirstChildElement("PrimitiveType")->QueryIntText(&primitiveType);
        std::string vsFile(mesh->FirstChildElement("VertexShader")->GetText());
        std::string psFile(mesh->FirstChildElement("PixelShader")->GetText());
        std::vector<int> indices;
        for (tinyxml2::XMLElement* i = mesh->FirstChildElement("Indices")->FirstChildElement(); i; i = i->NextSiblingElement()) {
            indices.emplace_back(0);
            i->QueryIntText(&indices.back());
        }
        std::shared_ptr<MeshMeta> pMeshMeta = std::make_shared<MeshMeta>(
            meshName,
            primitiveType,
            vsFile,
            psFile,
            indices
        );
        pMeshMeta->setVertexLayout(pModelMeta->getVertexLayout());
        pModelMeta->addMesh(pMeshMeta);
    }

    MainthreadCallbackHub::getInstance().submit([callback, pModelMeta] {
        callback(pModelMeta);
    });
}