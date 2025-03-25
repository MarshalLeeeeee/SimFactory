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



MeshMeta::MeshMeta(std::string name_, int primitiveType_,
    std::string vertexShader_, std::string pixelShader_, std::string vertexLayout_,
    std::vector<int>& indices_) :
    name(name_), primitiveType(primitiveType_), indexCnt(0) {
    // init vertex shader fullname
    vertexShader = std::string("vs_");
    vertexShader += vertexLayout_;
    vertexShader += "_";
    vertexShader += vertexShader_;
    // init pixel shader fullname
    pixelShader = std::string("ps_");
    pixelShader += vertexLayout_;
    pixelShader += "_";
    pixelShader += pixelShader_;
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

std::string MeshMeta::getVertexShader() const {
    return vertexShader;
}

std::string MeshMeta::getPixelShader() const {
    return pixelShader;
}

std::string MeshMeta::getVertexLayout() const {
    return vertexLayout;
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




void ModelMetaBase::addMesh(std::shared_ptr<MeshMeta> pMeshMeta) {
    if (!pMeshMeta) return;
    pMeshMetaVec.emplace_back(pMeshMeta);
}

std::string ModelMetaBase::getVertexLayout() const {
    return vertexLayout;
}

std::string ModelMetaBase::getVertexFileName() const {
    return vertexFileName;
}

const std::vector<std::shared_ptr<MeshMeta>>& ModelMetaBase::getMeshMetaVec() const {
    return pMeshMetaVec;
}

void* ModelMetaBase::getData() const {
    return nullptr;
}

size_t ModelMetaBase::getSize() const {
    return 0;
}

size_t ModelMetaBase::getByteWidth() const {
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
    std::string vertexFilePath(vertexData->GetText());
    std::shared_ptr<ModelMetaBase> pModelMeta = loadVertexFromFile(vertexFilePath);
    if (!pModelMeta)
        return;

    std::string vertexLayout = pModelMeta->getVertexLayout();
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
            vertexLayout,
            indices
        );
        pModelMeta->addMesh(pMeshMeta);
    }

    MainthreadCallbackHub::getInstance().submit([callback, pModelMeta] {
        callback(pModelMeta);
    });
}