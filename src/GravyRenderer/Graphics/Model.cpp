#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_obj_loader.h"
#include "tiny_gltf.h"

namespace Renderer
{
    Model::Model(std::string filename)
    {
        LoadModel(filename);
    }

    void Model::LoadModel(std::string filename)
    {
        std::string FileExtension = filename.substr(filename.find_last_of(".") + 1);

        if (FileExtension == "obj")
        {
            LoadOBJ(filename);
            return;
        }
        
        if (FileExtension == "gltf")
        {
            //LoadGLTF(filename);
            return;
        }

        LOG_ERROR("Model file extention (.{}) not supported!", FileExtension);
    }
    
    Model::Model(Primitive _type)
    {
        LoadPrimitive(_type);
    }

    void Model::LoadOBJ(std::string filename)
    {        
        tinyobj::attrib_t _attrib;
        std::vector<tinyobj::shape_t> _shapes;
        std::vector<tinyobj::material_t> _materials;

        std::string err;
        std::string warn;

        std::string FilePath = filename.substr(0, filename.find_last_of("/"));
        std::string TexPath = FilePath + "/textures/";

        bool ret = tinyobj::LoadObj(&_attrib, &_shapes, &_materials, &warn, &err, filename.c_str(), FilePath.c_str());

        if (!err.empty())
        {
            LOG_ERROR(err);
        }

        if (!ret)
        {
            exit(1);
        }

        // Loop over _shapes
        for (size_t s = 0; s < _shapes.size(); s++)
        {
            Mesh mesh;

            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < _shapes[s].mesh.num_face_vertices.size(); f++)
            {
                size_t fv = size_t(_shapes[s].mesh.num_face_vertices[f]);

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++)
                {
                    // access to vertex
                    tinyobj::index_t idx = _shapes[s].mesh.indices[index_offset + v];

                    tinyobj::real_t vx = _attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    tinyobj::real_t vy = _attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    tinyobj::real_t vz = _attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                    tinyobj::real_t tx = 0;
                    tinyobj::real_t ty = 0;

                    tinyobj::real_t nx = 0;
                    tinyobj::real_t ny = 0;
                    tinyobj::real_t nz = 0;

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    if (idx.normal_index >= 0)
                    {
                        nx = _attrib.normals[3 * size_t(idx.normal_index) + 0];
                        ny = _attrib.normals[3 * size_t(idx.normal_index) + 1];
                        nz = _attrib.normals[3 * size_t(idx.normal_index) + 2];
                    }

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    if (idx.texcoord_index >= 0)
                    {
                        tx = _attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                        ty = _attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    }

                    
                    mesh.vertices.push_back(Vertex(vx, vy, vz, tx, ty, nx, ny, nz));
                    mesh.indices.push_back(idx.vertex_index);
                    
                }
                index_offset += fv;

                // per-face material
                mesh.MaterialID = _shapes[s].mesh.material_ids[f];
            }
            mesh.Name = _shapes.at(s).name.c_str();

            mesh.Create(mesh.vertices, mesh.indices);

            mesh.VAO.Bind();
            mesh.VBO.Bind();

            mesh.VAO.LinkAttribF(0, 3, sizeof(Vertex), (void *)offsetof(Vertex, posX));
            mesh.VAO.LinkAttribF(1, 3, sizeof(Vertex), (void *)offsetof(Vertex, normalX));
            mesh.VAO.LinkAttribF(2, 2, sizeof(Vertex), (void *)offsetof(Vertex, texCoordX));

            mesh.VAO.UnBind();
            mesh.VBO.UnBind();

            Meshes.push_back(mesh);
        }

        Materials.resize(_materials.size());

        for (size_t i = 0; i < _materials.size(); i++)
        {
            auto &Mat = Materials[i];
            Mat.Name = _materials[i].name;

            auto fullTexturePath = TexPath + _materials[i].diffuse_texname;

            if (!_materials[i].diffuse_texname.empty())
            {
                Materials[i].Diffuse_Texture.LoadTexture(fullTexturePath, ColorSpace::RGB);

                Mat.Diffuse_Texture.LoadTexture(TexPath + _materials.at(i).diffuse_texname);
                Mat.Ambient = glm::vec3(_materials.at(i).ambient[0], _materials.at(i).ambient[1], _materials.at(i).ambient[2]);
                Mat.Diffuse = glm::vec3(_materials.at(i).diffuse[0], _materials.at(i).diffuse[1], _materials.at(i).diffuse[2]);
                Mat.Emission = glm::vec3(_materials.at(i).emission[0], _materials.at(i).emission[1], _materials.at(i).emission[2]);
                Mat.Shininess = _materials.at(i).shininess;
            }else{
                LOG_WARN("Texture not found or not specified for {} in .mtl file! Loading default texture instead...", Mat.Name);
                Mat.Diffuse_Texture.Create(DEFAULT_TEX);
            }

            if (!_materials.at(i).specular_texname.empty())
            {
                Materials[i].Specular_Texture.LoadTexture(fullTexturePath, ColorSpace::RGB);

                Mat.Specular_Texture.LoadTexture(TexPath + _materials.at(i).specular_texname);
                Mat.Specular = glm::vec3(_materials.at(i).specular[0], _materials.at(i).specular[1], _materials.at(i).specular[2]);
            }
        }
    }

    void Model::LoadGLTF(std::string filename)
    {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        bool res = false;
        std::string ext = tinygltf::GetFilePathExtension(filename);
        if (ext == "glb") {
            LOG_TRACE("Attempting to load binary glTF: {}", filename);
            res = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
        } else {
            LOG_TRACE("Attempting to load ASCII glTF: {}", filename);
            res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
        }

        if (!warn.empty()) {
            LOG_WARN(warn);
        }

        if (!err.empty()) {
            LOG_ERROR(err);
        }

        if (!res) {
            LOG_ERROR("Failed to load glTF: {}", filename);
            return;
        }

        LOG_DEBUG("Successfully loaded: {}", filename);

        // --- Accessing Model Data ---

        // Iterate over all nodes in the default scene
        const tinygltf::Scene& scene = model.scenes[model.defaultScene > -1 ? model.defaultScene : 0];
        for (size_t i = 0; i < scene.nodes.size(); ++i) {
            const tinygltf::Node& node = model.nodes[scene.nodes[i]];
            std::cout << "Node: " << (node.name.empty() ? "[unnamed]" : node.name) << std::endl;

            Meshes.resize(1);

            // Check if the node contains a mesh
            if (node.mesh > -1) {
                
                const tinygltf::Mesh& tmesh = model.meshes[node.mesh];
                std::cout << "  Mesh: " << (tmesh.name.empty() ? "[unnamed]" : tmesh.name) << std::endl;

                // A mesh is made of primitives, each with its own material and geometry
                for (size_t j = 0; j < tmesh.primitives.size(); ++j) {
                    const tinygltf::Primitive& primitive = tmesh.primitives[j];

                    // --- Accessing Vertex Positions ---
                    // The "POSITION" attribute is a required key for a primitive.
                    const auto& pos_accessor = model.accessors[primitive.attributes.at("POSITION")];
                    const auto& pos_buffer_view = model.bufferViews[pos_accessor.bufferView];
                    const auto& pos_buffer = model.buffers[pos_buffer_view.buffer];

                    const auto& texcoord_accessor = model.accessors[primitive.attributes.at("POSITION")];
                    const auto& texcoord_buffer_view = model.bufferViews[pos_accessor.bufferView];
                    const auto& texcoord_buffer = model.buffers[pos_buffer_view.buffer];

                    const auto& nor_accessor = model.accessors[primitive.attributes.at("POSITION")];
                    const auto& nor_buffer_view = model.bufferViews[pos_accessor.bufferView];
                    const auto& nor_buffer = model.buffers[pos_buffer_view.buffer];

                    // Get a pointer to the position data
                    const float* positions = reinterpret_cast<const float*>(
                        &pos_buffer.data[pos_buffer_view.byteOffset + pos_accessor.byteOffset]
                    );

                    // Get a pointer to the texcoord data
                    const float* texcoords = reinterpret_cast<const float*>(
                        &texcoord_buffer.data[texcoord_buffer_view.byteOffset + texcoord_accessor.byteOffset]
                    );

                    // Get a pointer to the normal data
                    const float* normals = reinterpret_cast<const float*>(
                        &nor_buffer.data[nor_buffer_view.byteOffset + nor_accessor.byteOffset]
                    );

                    size_t vertexCount = pos_accessor.count;
                    std::cout << "    Primitive " << j << " has " << vertexCount << " vertices." << std::endl;

                    // Example: Print the first 3 vertices
                    for (size_t v = 0; v < vertexCount; ++v) {
                        Meshes[j].PushVertex(Vertex(positions[v * 3 + 0], positions[v * 3 + 1], positions[v * 3 + 2], texcoords[v * 3 + 0], texcoords[v * 3 + 1], normals[v * 3 + 0], normals[v * 3 + 1], normals[v * 3 + 2]));
                    }

                    // --- Accessing Vertex Indices (if they exist) ---
                    if (primitive.indices > -1) {
                        const auto& index_accessor = model.accessors[primitive.indices];
                        const auto& index_buffer_view = model.bufferViews[index_accessor.bufferView];
                        const auto& index_buffer = model.buffers[index_buffer_view.buffer];

                        size_t indexCount = index_accessor.count;
                        std::cout << "    Primitive " << j << " has " << indexCount << " indices." << std::endl;

                        // Indices can be unsigned byte, short, or int. We need to handle each case.
                        // Here's an example for unsigned short (a common case)
                        if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                             const unsigned short* indices = reinterpret_cast<const unsigned short*>(
                                &index_buffer.data[index_buffer_view.byteOffset + index_accessor.byteOffset]
                            );
                            
                            for (size_t v = 0; i < indexCount; ++i) {

                                const uint32_t* indices = reinterpret_cast<const uint32_t*>(
                                    &pos_buffer.data[pos_buffer_view.byteOffset + pos_accessor.byteOffset]
                                );

                                Meshes[j].PushIndex((uint32_t)indices[i]);
                            }
                        }
                    }
                }
            }
        }
    }

    void Model::LoadPrimitive(Primitive primitive)
    {
        Mesh mesh;

        if (primitive == Primitive::Quad)
        {
            mesh.Name = "Quad";
            mesh.MaterialID = 0;
            QuadPrimitive_t QuadPrimitive;

            for(size_t i = 0; i < QuadPrimitive.vertices.size(); i += 8)
            {
                float vx = QuadPrimitive.vertices[i + 0];
                float vy = QuadPrimitive.vertices[i + 1];
                float vz = QuadPrimitive.vertices[i + 2];
                float tx = QuadPrimitive.vertices[i + 3];
                float ty = QuadPrimitive.vertices[i + 4];
                float nx = QuadPrimitive.vertices[i + 5];
                float ny = QuadPrimitive.vertices[i + 6];
                float nz = QuadPrimitive.vertices[i + 7];

                mesh.vertices.push_back(Vertex(vx, vy, vz, tx, ty, nx, ny, nz));
            }

            for(size_t i = 0; i < QuadPrimitive.indices.size(); i++)
            {
                mesh.indices.push_back(QuadPrimitive.indices[i]);
            }
        }
    
        if (primitive == Primitive::Cube)
        {
            mesh.Name = "Cube";
            mesh.MaterialID = 0;
            CubePrimitive_t CubePrimitive;

            for(size_t i = 0; i < CubePrimitive.vertices.size(); i += 8)
            {
                float vx = CubePrimitive.vertices[i + 0];
                float vy = CubePrimitive.vertices[i + 1];
                float vz = CubePrimitive.vertices[i + 2];
                float tx = CubePrimitive.vertices[i + 3];
                float ty = CubePrimitive.vertices[i + 4];
                float nx = CubePrimitive.vertices[i + 5];
                float ny = CubePrimitive.vertices[i + 6];
                float nz = CubePrimitive.vertices[i + 7];

                mesh.vertices.push_back(Vertex(vx, vy, vz, tx, ty, nx, ny, nz));
            }

            for(size_t i = 0; i < CubePrimitive.indices.size(); i++)
            {
                mesh.indices.push_back(CubePrimitive.indices[i]);
            }
        }

        if (primitive == Primitive::Pyramid)
        {
            mesh.Name = "Pyramide";
            mesh.MaterialID = 0;
            PyramidPrimitive_t PyramidePrimitive;

            for (size_t i = 0; i < PyramidePrimitive.vertices.size(); i += 8)
            {
                float vx = PyramidePrimitive.vertices[i + 0];
                float vy = PyramidePrimitive.vertices[i + 1];
                float vz = PyramidePrimitive.vertices[i + 2];
                float tx = PyramidePrimitive.vertices[i + 3];
                float ty = PyramidePrimitive.vertices[i + 4];
                float nx = PyramidePrimitive.vertices[i + 5];
                float ny = PyramidePrimitive.vertices[i + 6];
                float nz = PyramidePrimitive.vertices[i + 7];

                mesh.vertices.push_back(Vertex(vx, vy, vz, tx, ty, nx, ny, nz));
            }

            for(size_t i = 0; i < PyramidePrimitive.indices.size(); i++)
            {
                mesh.indices.push_back(PyramidePrimitive.indices[i]);
            }
        }

        
        mesh.Create(mesh.vertices, mesh.indices);
        
        mesh.VAO.Bind();
        mesh.VBO.Bind();
        
        mesh.VAO.LinkAttribF(0, 3, sizeof(Vertex), (void *)offsetof(Vertex, posX));
        mesh.VAO.LinkAttribF(1, 3, sizeof(Vertex), (void *)offsetof(Vertex, normalX));
        mesh.VAO.LinkAttribF(2, 2, sizeof(Vertex), (void *)offsetof(Vertex, texCoordX));
        
        mesh.VAO.UnBind();
        mesh.VBO.UnBind();

        mesh.MaterialID = 0; // Default material ID for primitive
        
        Meshes.push_back(mesh);
    }

    glm::mat4 Model::UpdateMatrix()
    {
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, Transform.Position);
        ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform.Rotation.x),glm::vec3(1,0,0)); //rotation x
        ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform.Rotation.y),glm::vec3(0,1,0)); //rotation y
        ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform.Rotation.z),glm::vec3(0,0,1)); //rotation z
        ModelMatrix = glm::scale(ModelMatrix, Transform.Scale);

        return ModelMatrix;
    }

    void Model::Render()
    {
        for (auto& mesh : Meshes)
        {
            if (Materials.size() >= 1) {
                Materials[mesh.MaterialID].Diffuse_Texture.SetActiveTexture(GL_TEXTURE0);
                Materials[mesh.MaterialID].Diffuse_Texture.Bind();
                Materials[mesh.MaterialID].Specular_Texture.SetActiveTexture(GL_TEXTURE1);
                Materials[mesh.MaterialID].Specular_Texture.Bind();
            }

            mesh.Draw();

            if (Materials.size() >= 1) {
                Materials[mesh.MaterialID].Diffuse_Texture.UnBind();
                Materials[mesh.MaterialID].Specular_Texture.UnBind();
            }
        }
    }
}