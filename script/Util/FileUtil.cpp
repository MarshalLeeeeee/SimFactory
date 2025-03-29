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

std::string getShaderFilepath(std::string fileName) {
    std::string p("./res/shader/");
    p += fileName;
    p += ".hlsl";
    return p;
}

std::string getShaderConfFilepath(std::string fileName) {
    std::string p("./res/shader/");
    p += fileName;
    p += ".xml";
    return p;
}


MeshMeta::MeshMeta(std::string name_, int primitiveType_,
    std::string vertexShaderName, std::string pixelShaderName, std::string vertexLayoutName,
    std::vector<int>& indices_) :
    name(name_), primitiveType(primitiveType_), indexCnt(0) {
    // init vertex shader filename
    vertexShaderFilename = std::string("vs_");
    vertexShaderFilename += vertexLayoutName;
    vertexShaderFilename += "_";
    vertexShaderFilename += vertexShaderName;
    // init pixel shader filename
    pixelShaderFilename = std::string("ps_");
    pixelShaderFilename += vertexLayoutName;
    pixelShaderFilename += "_";
    pixelShaderFilename += pixelShaderName;
    // init indices
    if (!indices_.empty()) {
        indexCnt = indices_.size();
        indices = std::shared_ptr<DWORD[]>(new DWORD[indexCnt]);
        for (uint32_t i = 0; i < indexCnt; ++i) {
            indices[i] = indices_[i];
        }
    }
}

int MeshMeta::getPrimitiveType() const {
    return primitiveType;
}

const std::string& MeshMeta::getVertexShaderFilename() const {
    return vertexShaderFilename;
}

const std::string& MeshMeta::getPixelShaderFilename() const {
    return pixelShaderFilename;
}

std::string MeshMeta::getName() const {
    return name;
}

std::shared_ptr<DWORD[]> MeshMeta::getIndices() const {
    return indices;
}

uint32_t MeshMeta::getIndexCnt() const {
    return indexCnt;
}




void ModelMetaBase::addMeshMeta(std::shared_ptr<MeshMeta> pMeshMeta) {
    if (!pMeshMeta) return;
    pMeshMetas.emplace_back(pMeshMeta);
}

std::string ModelMetaBase::getVertexLayoutName() const {
    return vertexLayoutName;
}

std::string ModelMetaBase::getVertexFilename() const {
    return vertexFilename;
}

const std::vector<std::shared_ptr<MeshMeta>>& ModelMetaBase::getMeshMetas() const {
    return pMeshMetas;
}

void* ModelMetaBase::getVertexData() const {
    return nullptr;
}

size_t ModelMetaBase::getVertexDataTypeSize() const {
    return 0;
}

size_t ModelMetaBase::getVertexDataByteWidth() const {
    return 0;
}

const D3D11_INPUT_ELEMENT_DESC* ModelMetaBase::getVertexLayoutDesc() const {
    return nullptr;
}

UINT ModelMetaBase::getVertexLayoutDescSize() const {
    return 0;
}




std::shared_ptr<ModelMetaBase> loadVertexFromFile(std::string fileName) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(getVertexFilepath(fileName).c_str());
    tinyxml2::XMLElement* vertexData = doc.FirstChildElement("VertexData");
    if (!vertexData)
        return nullptr;
    tinyxml2::XMLElement* vertexLayout = vertexData->FirstChildElement("VertexLayout");
    if (!vertexLayout)
        return nullptr;
    std::string layout(vertexLayout->GetText());
    if (layout == "poscolor") {
        std::shared_ptr<ModelMeta<VertexPosColor>> pModelMeta = std::make_shared<ModelMeta<VertexPosColor>>();
        pModelMeta->parse(vertexData, layout, fileName);
        return pModelMeta;
    }
    return nullptr;
}

void loadModelFromFileAsync(std::string fileName, std::function<void(std::shared_ptr<ModelMetaBase>)> callback) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(getModelFilepath(fileName).c_str());
    tinyxml2::XMLElement* model = doc.FirstChildElement("Model");
    if (!model)
        return;
    tinyxml2::XMLElement* vertexData = model->FirstChildElement("VertexData");
    if (!vertexData)
        return;
    std::string vertexName(vertexData->GetText());
    std::shared_ptr<ModelMetaBase> pModelMeta = loadVertexFromFile(vertexName);
    if (!pModelMeta)
        return;

    std::string vertexLayoutName = pModelMeta->getVertexLayoutName();
    for (tinyxml2::XMLNode* mesh = model->FirstChildElement("Meshes")->FirstChild(); mesh; mesh = mesh->NextSibling()) {
        std::string meshName(mesh->FirstChildElement("Name")->GetText());
        meshName = vertexName + "_" + meshName;
        int primitiveType = 0;
        mesh->FirstChildElement("PrimitiveType")->QueryIntText(&primitiveType);
        std::string vertexShaderName(mesh->FirstChildElement("VertexShader")->GetText());
        std::string pixelShaderName(mesh->FirstChildElement("PixelShader")->GetText());
        std::vector<int> indices;
        for (tinyxml2::XMLElement* i = mesh->FirstChildElement("Indices")->FirstChildElement(); i; i = i->NextSiblingElement()) {
            indices.emplace_back(0);
            i->QueryIntText(&indices.back());
        }
        std::shared_ptr<MeshMeta> pMeshMeta = std::make_shared<MeshMeta>(
            meshName,
            primitiveType,
            vertexShaderName,
            pixelShaderName,
            vertexLayoutName,
            indices
        );
        pModelMeta->addMeshMeta(pMeshMeta);
    }

    MainthreadCallbackHub::getInstance().submit([callback, pModelMeta] {
        callback(pModelMeta);
    });
}