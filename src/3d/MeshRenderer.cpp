#include "MeshRenderer.h"
#include <tiny_obj_loader.h>
#include "base/FileUtils.h"

NS_OCF_BEGIN

MeshRenderer* MeshRenderer::create(std::string_view fileName)
{
    MeshRenderer* result = new MeshRenderer();
    if (result->initWithObjFile(fileName)) {
        result->autorelease();
        return result;
    }
    OCF_SAFE_DELETE(result);
    return nullptr;
}

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
}

bool MeshRenderer::initWithObjFile(std::string_view objFile)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(objFile.data());

    tinyobj::ObjReaderConfig readerConfig;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(fullPath, readerConfig)) {
        if (!reader.Error().empty()) {
            OCFLOG("[TinyObjReader] %s\n", reader.Error());
        }

        return false;
    }

    if (!reader.Warning().empty()) {
        OCFLOG("[TinyObjReader] %s\n", reader.Warning());
    }

    const tinyobj::attrib_t& attrib                   = reader.GetAttrib();
    const std::vector<tinyobj::shape_t>& shapes       = reader.GetShapes();
    const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();


    for (size_t s = 0; s < shapes.size(); s++) {
        size_t indexOffset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = shapes[s].mesh.num_face_vertices[f];

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t index = shapes[s].mesh.indices[indexOffset + v];

                // 頂点座標を取得
                tinyobj::real_t vx = attrib.vertices[3 * static_cast<size_t>(index.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * static_cast<size_t>(index.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * static_cast<size_t>(index.vertex_index) + 2];

                // 法線データがあるかチェック
                if (index.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * static_cast<size_t>(index.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * static_cast<size_t>(index.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * static_cast<size_t>(index.normal_index) + 2];
                }
                // テクスチャ座標があるかチェック
                if (index.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * static_cast<size_t>(index.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * static_cast<size_t>(index.texcoord_index) + 1];
                }
            }
            indexOffset += fv;

            //shapes[s].mesh.material_ids[f];
        }
    }

    return true;
}

NS_OCF_END
