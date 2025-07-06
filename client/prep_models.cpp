#include "raylib.h"
#include "utils.h"
#include "utils_client.h"
#include <cmath>
#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MATERIAL_MAPS 12
#define RL_MAX_SHADER_LOCATIONS 32

Model& LoadModelFromChunkTest(const rresResourceChunk &chunk, Model &testModel, Arena& arena, MapCT<const char*, Material, 100>& materialPool, rresCentralDir& dir) {
  Model& model = arena.alloc<Model>();

  if (!chunk.data.raw) {
    LOG_ERROR("Chunk data is null");
    return model;
  }

  // Initialize all pointers to nullptr explicitly
  model.meshes = nullptr;
  model.materials = nullptr;
  model.meshMaterial = nullptr;
  model.bones = nullptr;
  model.bindPose = nullptr;

  const unsigned char *data = (const unsigned char *)chunk.data.raw;
  size_t offset = 0;

  // Read transform matrix
  memcpy(&model.transform, data + offset, sizeof(Matrix));
  LOG_ASSERT(CompareMatrices(&model.transform, &testModel.transform, 1),
             "testModel and model don't match...");
  offset += sizeof(Matrix);

  // Read counts
  memcpy(&model.meshCount, data + offset, sizeof(int));
  LOG_ASSERT(model.meshCount == testModel.meshCount,
             "testModel and model don't match...");
  offset += sizeof(int);
  memcpy(&model.materialCount, data + offset, sizeof(int));
  LOG_ASSERT(model.materialCount == testModel.materialCount,
             "testModel and model don't match...");
  offset += sizeof(int);

  LOG_TRACE("Loading model with %d meshes and %d materials", model.meshCount,
            model.materialCount);

  // Read global flags
  unsigned char globalFlags;
  memcpy(&globalFlags, data + offset, sizeof(unsigned char));
  offset += sizeof(unsigned char);

  // Read meshes
  if (globalFlags & 1) {
    int size = model.meshCount * sizeof(Mesh);
    model.meshes = arena.alloc_raw<Mesh>(size);
    LOG_ASSERT(model.meshes != nullptr,
               "Failed to allocate memory for meshes: %zu bytes", size);

    for (int i = 0; i < model.meshCount; i++) {
      Mesh& mesh = model.meshes[i];

      // Initialize all mesh pointers to nullptr
      mesh.vertices = nullptr;
      mesh.texcoords = nullptr;
      mesh.texcoords2 = nullptr;
      mesh.normals = nullptr;
      mesh.tangents = nullptr;
      mesh.colors = nullptr;
      mesh.indices = nullptr;
      mesh.animVertices = nullptr;
      mesh.animNormals = nullptr;
      mesh.boneIds = nullptr;
      mesh.boneWeights = nullptr;
      mesh.boneMatrices = nullptr;
      mesh.vboId = nullptr;

      // Initialize all values to be 0
      mesh.vertexCount = 0;
      mesh.triangleCount = 0;
      mesh.vaoId = 0;
      mesh.boneCount = 0;

      // Read counts
      memcpy(&mesh.vertexCount, data + offset, sizeof(int));
      LOG_ASSERT(model.meshes[i].vertexCount ==
                     testModel.meshes[i].vertexCount,
                 "testModel and model don't match...");
      offset += sizeof(int);
      memcpy(&mesh.triangleCount, data + offset, sizeof(int));
      LOG_ASSERT(model.meshes[i].triangleCount ==
                     testModel.meshes[i].triangleCount,
                 "testModel and model don't match...");
      offset += sizeof(int);
      memcpy(&mesh.boneCount, data + offset, sizeof(int));
      LOG_ASSERT(model.meshes[i].boneCount == testModel.meshes[i].boneCount,
                 "testModel and model don't match...");
      offset += sizeof(int);

      unsigned char meshFlags;
      memcpy(&meshFlags, data + offset, sizeof(unsigned char));
      offset += sizeof(unsigned char);

      unsigned char animFlags;
      memcpy(&animFlags, data + offset, sizeof(unsigned char));
      offset += sizeof(unsigned char);

      // Read vertex data
      if (mesh.vertexCount > 0) {
        // Vertices
        if (meshFlags & 1) {
          size_t size = mesh.vertexCount * 3 * sizeof(float);
          mesh.vertices = arena.alloc_raw<float>(size);
          LOG_ASSERT(mesh.vertices != nullptr,
                     "Failed to allocate memory for vertices: %zu bytes", size);
          memcpy(mesh.vertices, data + offset, size);
          LOG_ASSERT(CompareFloatArrays(model.meshes[i].vertices,
                                        testModel.meshes[i].vertices, size),
                     "testModel and model don't match...");
          offset += size;
        }
        // Texcoords
        if (meshFlags & 2) {
          size_t size = mesh.vertexCount * 2 * sizeof(float);
          mesh.texcoords = arena.alloc_raw<float>(size);
          LOG_ASSERT(mesh.texcoords != nullptr,
                     "Failed to allocate memory for texcoords: %zu bytes",
                     size);
          memcpy(mesh.texcoords, data + offset, size);
          LOG_ASSERT(CompareFloatArrays(model.meshes[i].texcoords,
                                        testModel.meshes[i].texcoords, size),
                     "testModel and model don't match...");
          offset += size;
        }
        // Texcoords2
        if (meshFlags & 4) {
          size_t size = mesh.vertexCount * 2 * sizeof(float);
          mesh.texcoords2 = arena.alloc_raw<float>(size);
          LOG_ASSERT(mesh.texcoords2 != nullptr,
                     "Failed to allocate memory for texcoords2: %zu bytes",
                     size);
          memcpy(mesh.texcoords2, data + offset, size);
          LOG_ASSERT(CompareFloatArrays(model.meshes[i].texcoords2,
                                        testModel.meshes[i].texcoords2, size),
                     "testModel and model don't match...");
          offset += size;
        }
        // Normals
        if (meshFlags & 8) {
          size_t size = mesh.vertexCount * 3 * sizeof(float);
          mesh.normals = arena.alloc_raw<float>(size);
          LOG_ASSERT(mesh.normals != nullptr,
                     "Failed to allocate memory for normals: %zu bytes", size);
          memcpy(mesh.normals, data + offset, size);
          LOG_ASSERT(CompareFloatArrays(model.meshes[i].normals,
                                        testModel.meshes[i].normals, size),
                     "testModel and model don't match...");
          offset += size;
        }
        // Tangents
        if (meshFlags & 16) {
          size_t size = mesh.vertexCount * 4 * sizeof(float);
          mesh.tangents = arena.alloc_raw<float>(size);
          LOG_ASSERT(mesh.tangents != nullptr,
                     "Failed to allocate memory for tangents: %zu bytes", size);
          memcpy(mesh.tangents, data + offset, size);
          LOG_ASSERT(CompareFloatArrays(model.meshes[i].tangents,
                                        testModel.meshes[i].tangents, size),
                     "testModel and model don't match...");
          offset += size;
        }
        // Colors
        if (meshFlags & 32) {
          size_t size = mesh.vertexCount * 4 * sizeof(unsigned char *);
          mesh.colors = arena.alloc_raw<unsigned char>(size);
          LOG_ASSERT(mesh.colors != nullptr,
                     "Failed to allocate memory for colors: %zu bytes", size);
          memcpy(mesh.colors, data + offset, size);
          LOG_ASSERT(CompareUCharArrays(model.meshes[i].colors,
                                        testModel.meshes[i].colors, size),
                     "testModel and model don't match...");
          offset += size;
        }

        // Animation data
        // Animated vertices
        if (animFlags & 1) {
          size_t size = mesh.vertexCount * 3 * sizeof(float);
          mesh.animVertices = arena.alloc_raw<float>(size);
          LOG_ASSERT(mesh.animVertices != nullptr,
                     "Failed to allocate memory for anim vertices: %zu bytes",
                     size);
          memcpy(mesh.animVertices, data + offset, size);
          LOG_ASSERT(CompareFloatArrays(model.meshes[i].animVertices,
                                        testModel.meshes[i].animVertices, size),
                     "testModel and model don't match...");
          offset += size;
        }
        // Animated normals
        if (animFlags & 2) {
          size_t size = mesh.vertexCount * 3 * sizeof(float);
          mesh.animNormals = arena.alloc_raw<float>(size);
          LOG_ASSERT(mesh.animNormals != nullptr,
                     "Failed to allocate memory for anim normals: %zu bytes",
                     size);
          memcpy(mesh.animNormals, data + offset, size);
          LOG_ASSERT(CompareFloatArrays(model.meshes[i].animNormals,
                                        testModel.meshes[i].animNormals, size),
                     "testModel and model don't match...");
          offset += size;
        }
        // Bone IDs
        if (animFlags & 4) {
          size_t size = mesh.vertexCount * 4;
          mesh.boneIds = arena.alloc_raw<unsigned char>(size);
          LOG_ASSERT(mesh.boneIds != nullptr,
                     "Failed to allocate memory for bone IDs: %zu bytes", size);
          memcpy(mesh.boneIds, data + offset, size);
          LOG_ASSERT(CompareUCharArrays(model.meshes[i].boneIds,
                                        testModel.meshes[i].boneIds, size),
                     "testModel and model don't match...");
          offset += size;
        }
        // Bone weights
        if (animFlags & 8) {
          size_t size = mesh.vertexCount * 4 * sizeof(float);
          mesh.boneWeights = arena.alloc_raw<float>(size);
          LOG_ASSERT(mesh.boneWeights != nullptr,
                     "Failed to allocate memory for bone weights: %zu bytes",
                     size);
          memcpy(mesh.boneWeights, data + offset, size);
          LOG_ASSERT(CompareFloatArrays(model.meshes[i].boneWeights,
                                        testModel.meshes[i].boneWeights, size),
                     "testModel and model don't match...");
          offset += size;
        }
        // Bone matrices
        if (animFlags & 16 && mesh.boneCount > 0) {
          size_t size = mesh.boneCount * sizeof(Matrix);
          mesh.boneMatrices = arena.alloc_raw<Matrix>(size);
          LOG_ASSERT(mesh.boneMatrices != nullptr,
                     "Failed to allocate memory for bone matrices: %zu bytes",
                     size);
          memcpy(mesh.boneMatrices, data + offset, size);
          LOG_ASSERT(CompareMatrices(model.meshes[i].boneMatrices,
                                     testModel.meshes[i].boneMatrices,
                                     mesh.boneCount),
                     "testModel and model don't match...");
          offset += size;
        }
      }

      // Read indices
      if (mesh.triangleCount > 0 && (meshFlags & 64)) {
        size_t size = mesh.triangleCount * 3 * sizeof(unsigned short);
        mesh.indices = arena.alloc_raw<unsigned short>(size);
        LOG_ASSERT(mesh.indices != nullptr,
                   "Failed to allocate memory for indices: %zu bytes", size);
        memcpy(mesh.indices, data + offset, size);
        LOG_ASSERT(CompareUShortArrays(model.meshes[i].indices,
                                       testModel.meshes[i].indices, size),
                   "testModel and model don't match...");
        offset += size;
      }

      // Read OpenGL identifiers
      UploadMesh(&mesh, false);
    }
  }

  // Read materials
  if (globalFlags & 2) {
    int size = model.materialCount * sizeof(Material);
    // TODO: should this be allocated here? what if we overwrite it later...
    model.materials = arena.alloc_raw<Material>(size);
    LOG_ASSERT(model.materials != nullptr,
               "Failed to allocate memory for materials: %zu bytes", size);

    for (int i = 0; i < model.materialCount; i++) {
      const char* materialId = (const char*)(data + offset);
      offset += sizeof(char[128]);

      LOG_TRACE("Reading Material id: %s", materialId);

      if(materialPool.contains(materialId)) {
        LOG_TRACE("Duplicate found!");
        model.materials[i] = materialPool.get(materialId); // fetch ptr to existing material
      } else {
        LOG_TRACE("Creating a new material!");

        size_t sizeInBytes = strlen(materialId) + 1;
        char* materialIdOnHeap = arena.alloc_raw<char>(sizeInBytes);
        memcpy(materialIdOnHeap, materialId, sizeInBytes);

        model.materials[i] = materialPool.get(materialIdOnHeap); // create new material & return ptr
        Material& material = model.materials[i];
        size_t materialOffset = 0;

        char combined[256];
        snprintf(combined, sizeof(combined), "%s%s", materialId, ".mat");
        int rresMaterialId = rresGetResourceId(dir, combined);
        rresResourceChunk chunkMaterial = rresLoadResourceChunk("resources.rres", rresMaterialId);
        const unsigned char* materialData = static_cast<const unsigned char*>(chunkMaterial.data.raw);

        // Initialize material pointers
        material.shader.locs = nullptr;
        material.maps = nullptr;

        unsigned char matFlags;
        memcpy(&matFlags, materialData + materialOffset, sizeof(unsigned char));
        materialOffset += sizeof(unsigned char);

        // Read shader ID
        memcpy(&material.shader.id, materialData + materialOffset, sizeof(unsigned int));
        materialOffset += sizeof(unsigned int);

        // Read shader locations
        if (matFlags & 1) {
          size_t size = RL_MAX_SHADER_LOCATIONS * sizeof(int);
          material.shader.locs = arena.alloc_raw<int>(size);
          memcpy(material.shader.locs, materialData + materialOffset, size);
          materialOffset += size;
        }
        // Read material maps
        if (matFlags & 2) {
          size_t size = MAX_MATERIAL_MAPS * sizeof(MaterialMap);
          material.maps = arena.alloc_raw<MaterialMap>(size);

          // Read each material map
          for (int j = 0; j < MAX_MATERIAL_MAPS; j++) {
            // Read texture
            memcpy(&material.maps[j].texture, materialData + materialOffset, sizeof(Texture));
            materialOffset += sizeof(Texture);

            // Read color
            memcpy(&material.maps[j].color, materialData + materialOffset, sizeof(Color));
            materialOffset += sizeof(Color);

            // Read value
            memcpy(&material.maps[j].value, materialData + materialOffset, sizeof(float));
            materialOffset += sizeof(float);

            if(IsTextureValid(material.maps[j].texture)) {
              long unsigned int pixelDataSize = GetPixelDataSize(material.maps[j].texture.width, material.maps[j].texture.height, material.maps[j].texture.format);
              // Creating a local stack buffer
              // WARNING: For large images this might be unsafe, upgrade to max size and heap allocation if needed!!
              unsigned char pixelDataStack[1024*1024*4];

              // Safety check
              LOG_ASSERT(pixelDataSize <= sizeof(pixelDataStack), "Texture is too large...");

              memcpy(pixelDataStack, materialData + materialOffset, pixelDataSize);
              materialOffset += pixelDataSize;

              Image img = {}; // Also stack?
              img.data = &pixelDataStack;
              img.height = material.maps[j].texture.height;
              img.width = material.maps[j].texture.width;
              img.format = material.maps[j].texture.format;
              img.mipmaps = material.maps[j].texture.mipmaps;
              material.maps[j].texture = LoadTextureFromImage(img); //atleast this is heap
            }
          }
        }

        // Read material params (all 4 floats)
        memcpy(&material.params, materialData + materialOffset, sizeof(float) * 4);
        LOG_ASSERT(CompareFloatArrays(model.materials[i].params,
                                      testModel.materials[i].params,
                                      sizeof(float) * 4),
                   "testModel and model don't match...");
        materialOffset += sizeof(float) * 4;

        rresUnloadResourceChunk(chunkMaterial);
      }
    }
  }

  // Read mesh material indices
  if (globalFlags & 4) {
    int size = model.meshCount * sizeof(int);
    model.meshMaterial = arena.alloc_raw<int>(size);
    LOG_ASSERT(model.meshMaterial != nullptr,
               "Failed to allocate memory for mesh materials: %zu bytes", size);
    memcpy(model.meshMaterial, data + offset, size);
    LOG_ASSERT(
        CompareIntArrays(model.meshMaterial, testModel.meshMaterial, size),
        "testModel and model don't match...");
    offset += size;
  }

  memcpy(&model.boneCount, data + offset, sizeof(int));
  LOG_ASSERT(model.boneCount == testModel.boneCount,
             "testModel and model don't match...");
  offset += sizeof(int);

  if (model.boneCount > 0) {
    // Read bones
    if (globalFlags & 8) {
      int size = model.boneCount * sizeof(BoneInfo);
      model.bones = arena.alloc_raw<BoneInfo>(size);
      LOG_ASSERT(model.bones != nullptr,
                 "Failed to allocate memory for bones: %zu bytes", size);
      memcpy(model.bones, data + offset, size);
      LOG_ASSERT(CompareBones(model.bones, testModel.bones, size),
                 "testModel and model don't match...");
      offset += size;
    }

    // Read bind pose
    if (globalFlags & 16) {
      int size = model.boneCount * sizeof(Transform);
      model.bindPose = arena.alloc_raw<Transform>(size);
      LOG_ASSERT(model.bindPose != nullptr,
                 "Failed to allocate memory for bind pose: %zu bytes", size);

      for (int i = 0; i < model.boneCount; i++) {
        memcpy(&model.bindPose[i].translation, data + offset, sizeof(Vector3));
        LOG_ASSERT(CompareVector3(model.bindPose[i].translation,
                                  testModel.bindPose[i].translation),
                   "testModel and model don't match...");
        offset += sizeof(Vector3);
        memcpy(&model.bindPose[i].rotation, data + offset, sizeof(Vector4));
        LOG_ASSERT(CompareVector4(model.bindPose[i].rotation,
                                  testModel.bindPose[i].rotation),
                   "testModel and model don't match...");
        offset += sizeof(Vector4);
        memcpy(&model.bindPose[i].scale, data + offset, sizeof(Vector3));
        LOG_ASSERT(CompareVector3(model.bindPose[i].scale,
                                  testModel.bindPose[i].scale),
                   "testModel and model don't match...");
        offset += sizeof(Vector3);
      }
    }
  }

  return model;
}

ModelAnimation& LoadModelAnimationFromChunkTest(const rresResourceChunk &chunk, ModelAnimation &testAnim, Arena& arena) {
  ModelAnimation& anim = arena.alloc<ModelAnimation>();

  if (!chunk.data.raw) {
    LOG_ERROR("Chunk data is null");
    return anim;
  }

  const unsigned char *data = (const unsigned char *)chunk.data.raw;
  size_t offset = 0;

  // Read counts
  memcpy(&anim.boneCount, data + offset, sizeof(int));
  LOG_ASSERT(anim.boneCount == testAnim.boneCount, "testAnim and anim don't match...");
  offset += sizeof(int);

  memcpy(&anim.frameCount, data + offset, sizeof(int));
  LOG_ASSERT(anim.frameCount == testAnim.frameCount, "testAnim and anim don't match...");
  offset += sizeof(int);

  // Read name
  memcpy(anim.name, data + offset, sizeof(char) * 32);
  LOG_ASSERT(strcmp(anim.name, testAnim.name) == 0, "testAnim and anim don't match...");
  offset += sizeof(char) * 32;

  // Read bones
  if (anim.boneCount > 0) {
    size_t boneSize = sizeof(BoneInfo) * anim.boneCount;
    anim.bones = arena.alloc_raw<BoneInfo>(boneSize);
    LOG_ASSERT(anim.bones != nullptr, "Failed to allocate memory for bones");
    memcpy(anim.bones, data + offset, boneSize);
    LOG_ASSERT(CompareBones(anim.bones, testAnim.bones, boneSize), "testAnim and anim don't match...");
    offset += boneSize;
  }

  // Read frame poses
  if (anim.frameCount > 0 && anim.boneCount > 0) {
    // Allocate array of frame pose pointers
    anim.framePoses = arena.alloc_raw<Transform*>(sizeof(Transform*) * anim.frameCount);
    LOG_ASSERT(anim.framePoses != nullptr, "Failed to allocate memory for frame poses");

    // Allocate and read each frame's poses
    for (int frame = 0; frame < anim.frameCount; frame++) {
      anim.framePoses[frame] = arena.alloc_raw<Transform>(sizeof(Transform) * anim.boneCount);
      LOG_ASSERT(anim.framePoses[frame] != nullptr, "Failed to allocate memory for frame pose");

      for (int bone = 0; bone < anim.boneCount; bone++) {
        Transform& transform = anim.framePoses[frame][bone];

        // Read translation
        memcpy(&transform.translation, data + offset, sizeof(Vector3));
        LOG_ASSERT(CompareVector3(transform.translation, testAnim.framePoses[frame][bone].translation), 
                   "testAnim and anim don't match...");
        offset += sizeof(Vector3);

        // Read rotation
        memcpy(&transform.rotation, data + offset, sizeof(Quaternion));
        LOG_ASSERT(CompareVector4(transform.rotation, testAnim.framePoses[frame][bone].rotation), 
                   "testAnim and anim don't match...");
        offset += sizeof(Quaternion);

        // Read scale
        memcpy(&transform.scale, data + offset, sizeof(Vector3));
        LOG_ASSERT(CompareVector3(transform.scale, testAnim.framePoses[frame][bone].scale), 
                   "testAnim and anim don't match...");
        offset += sizeof(Vector3);
      }
    }
  }

  return anim;
}

void GetMaterialId(const Material& material, char* outBuffer, size_t bufferSize) {
  unsigned int hash = 5381;

  // Hash all material maps
  for (int i = 0; i < MAX_MATERIAL_MAPS; i++) {
    const MaterialMap& map = material.maps[i];

    // Hash color and value
    hash = HashCombine(hash, &map.color, sizeof(map.color));
    hash = HashCombine(hash, &map.value, sizeof(map.value));

    // Hash texture data if present
    if (IsTextureValid(map.texture)) {
      Image img = LoadImageFromTexture(map.texture);
      hash = HashCombine(hash, &img.width, sizeof(img.width));
      hash = HashCombine(hash, &img.height, sizeof(img.height));
      hash = HashCombine(hash, &img.format, sizeof(img.format));
      hash = HashCombine(hash, img.data, GetPixelDataSize(img.width, img.height, img.format));
      UnloadImage(img);
    }
  }

  snprintf(outBuffer, bufferSize, "%08x", hash);
}

void ExportMaterialsToBinary(const Model &model, const char *mat_path, Arena& arena) {
  if (model.materials) {
    for (int i = 0; i < model.materialCount; i++) {
      char* buffer = arena.alloc_raw<char>(MB(200));
      char* current = buffer;

      char matId[128];  // Buffer for the material ID/ file name
      GetMaterialId(model.materials[i], matId, sizeof(matId));
      LOG_TRACE("Writing Material ID: %s", matId);

      const Material &material = model.materials[i];

      // Write material flags
      unsigned char matFlags = 0;
      matFlags |= (material.shader.locs ? 1 : 0);
      matFlags |= (material.maps ? 2 : 0);
      memcpy(current, &matFlags, sizeof(unsigned char));
      current += sizeof(unsigned char);
      // LOG_TRACE("After material %i material flags: %zu bytes\n", i, current - buffer);

      // Write shader  NOTE: this is kinda stupid, no shaders will be applied at write time
      memcpy(current, &material.shader.id, sizeof(unsigned int));
      current += sizeof(unsigned int);
      // LOG_TRACE("After material %i shader id: %zu bytes\n", i, current - buffer);

      if (material.shader.locs) {
        size_t size = sizeof(int) * RL_MAX_SHADER_LOCATIONS;
        memcpy(current, material.shader.locs, size);
        current += size;
        // LOG_TRACE("After material %i shader locs: %zu bytes\n", i, current - buffer);
      }

      // Write material maps
      if (material.maps) {
        for (int j = 0; j < MAX_MATERIAL_MAPS; j++) {
          const MaterialMap &map = material.maps[j];

          // Serialize Texture struct from map
          memcpy(current, &map.texture, sizeof(Texture));
          current += sizeof(Texture);

          // Serialize Color and float value from map
          memcpy(current, &map.color, sizeof(Color));
          current += sizeof(Color);
          memcpy(current, &map.value, sizeof(float));
          current += sizeof(float);

          if(IsTextureValid(material.maps[j].texture)) {
            // Load image from GPU texture for raw pixel serialization
            Image img = LoadImageFromTexture(map.texture);

            // Calculate size of pixel data for serialization
            int pixelDataSize = GetPixelDataSize(img.width, img.height, img.format);

            // Serialize raw pixel data
            memcpy(current, img.data, pixelDataSize);
            current += pixelDataSize;
            UnloadImage(img);
          }
        }
      }

      // Write material parameters
      memcpy(current, material.params, sizeof(float) * 4);
      current += sizeof(float) * 4;
      // LOG_TRACE("After material %i params: %zu bytes\n", i, current - buffer);

      // Create full path + name
      char combined[256];
      snprintf(combined, sizeof(combined), "%s%s%s", mat_path, matId, ".mat");
      write_file(combined, buffer, current - buffer);
    }
  }
}

void ExportModelToBinary(const Model &model, const char *filename, Arena& arena) {
  if (!filename)
    return;

  char* buffer = arena.alloc_raw<char>(MB(500));
  char* current = buffer;

  // Write transform matrix
  memcpy(current, &model.transform, sizeof(Matrix));
  current += sizeof(Matrix);
  // LOG_TRACE("After transform matrix: %zu bytes\n", current - buffer);

  // Write counts
  memcpy(current, &model.meshCount, sizeof(int));
  current += sizeof(int);
  // LOG_TRACE("After mesh count: %zu bytes\n", current - buffer);
  memcpy(current, &model.materialCount, sizeof(int));
  current += sizeof(int);
  // LOG_TRACE("After material count: %zu bytes\n", current - buffer);

  // Write global flags
  unsigned char globalFlags = 0;
  globalFlags |= (model.meshes ? 1 : 0);
  globalFlags |= (model.materials ? 2 : 0);
  globalFlags |= (model.meshMaterial ? 4 : 0);
  globalFlags |= (model.bones ? 8 : 0);
  globalFlags |= (model.bindPose ? 16 : 0);
  memcpy(current, &globalFlags, sizeof(unsigned char));
  current += sizeof(unsigned char);
  // LOG_TRACE("After global flags: %zu bytes\n", current - buffer);

  // Write meshes
  if (model.meshes) {
    for (int i = 0; i < model.meshCount; i++) {
      const Mesh &mesh = model.meshes[i];

      // Write counts
      memcpy(current, &mesh.vertexCount, sizeof(int));
      current += sizeof(int);
      // LOG_TRACE("After mesh %i vertex count: %zu bytes\n", i, current - buffer);
      memcpy(current, &mesh.triangleCount, sizeof(int));
      current += sizeof(int);
      // LOG_TRACE("After mesh %i triangle count: %zu bytes\n", i, current - buffer);
      memcpy(current, &mesh.boneCount, sizeof(int));
      current += sizeof(int);
      // LOG_TRACE("After mesh %i bone count: %zu bytes\n", i, current - buffer);

      // Write mesh flags
      unsigned char meshFlags = 0;
      meshFlags |= (mesh.vertices ? 1 : 0);
      meshFlags |= (mesh.texcoords ? 2 : 0);
      meshFlags |= (mesh.texcoords2 ? 4 : 0);
      meshFlags |= (mesh.normals ? 8 : 0);
      meshFlags |= (mesh.tangents ? 16 : 0);
      meshFlags |= (mesh.colors ? 32 : 0);
      meshFlags |= (mesh.indices ? 64 : 0);
      memcpy(current, &meshFlags, sizeof(unsigned char));
      current += sizeof(unsigned char);
      // LOG_TRACE("After mesh %i mesh flags: %zu bytes\n", i, current - buffer);

      // Write animation flags
      unsigned char animFlags = 0;
      animFlags |= (mesh.animVertices ? 1 : 0);
      animFlags |= (mesh.animNormals ? 2 : 0);
      animFlags |= (mesh.boneIds ? 4 : 0);
      animFlags |= (mesh.boneWeights ? 8 : 0);
      animFlags |= (mesh.boneMatrices ? 16 : 0);
      memcpy(current, &animFlags, sizeof(unsigned char));
      current += sizeof(unsigned char);
      // LOG_TRACE("After mesh %i animation flags: %zu bytes\n", i, current - buffer);

      // Write vertex data
      if (mesh.vertexCount > 0) {
        if (mesh.vertices) {
          size_t size = sizeof(float) * mesh.vertexCount * 3;
          memcpy(current, mesh.vertices, size);
          current += size;
          // LOG_TRACE("After mesh %i vertices: %zu bytes\n", i, current - buffer);
        }
        if (mesh.texcoords) {
          size_t size = sizeof(float) * mesh.vertexCount * 2;
          memcpy(current, mesh.texcoords, size);
          current += size;
          // LOG_TRACE("After mesh %i texcoords: %zu bytes\n", i, current - buffer);
        }
        if (mesh.texcoords2) {
          size_t size = sizeof(float) * mesh.vertexCount * 2;
          memcpy(current, mesh.texcoords2, size);
          current += size;
          // LOG_TRACE("After mesh %i texcoords2: %zu bytes\n", i, current - buffer);
        }
        if (mesh.normals) {
          size_t size = sizeof(float) * mesh.vertexCount * 3;
          memcpy(current, mesh.normals, size);
          current += size;
          // LOG_TRACE("After mesh %i normals: %zu bytes\n", i, current - buffer);
        }
        if (mesh.tangents) {
          size_t size = sizeof(float) * mesh.vertexCount * 4;
          memcpy(current, mesh.tangents, size);
          current += size;
          // LOG_TRACE("After mesh %i tangents: %zu bytes\n", i, current - buffer);
        }
        if (mesh.colors) {
          size_t size = sizeof(unsigned char) * mesh.vertexCount * 4;
          memcpy(current, mesh.colors, size);
          current += size;
          // LOG_TRACE("After mesh %i colors: %zu bytes\n", i, current - buffer);
        }

        // Write animation data
        if (mesh.animVertices) {
          size_t size = sizeof(float) * mesh.vertexCount * 3;
          memcpy(current, mesh.animVertices, size);
          current += size;
          // LOG_TRACE("After mesh %i anim vertices: %zu bytes\n", i, current - buffer);
        }
        if (mesh.animNormals) {
          size_t size = sizeof(float) * mesh.vertexCount * 3;
          memcpy(current, mesh.animNormals, size);
          current += size;
          // LOG_TRACE("After mesh %i anim normals: %zu bytes\n", i, current - buffer);
        }
        if (mesh.boneIds) {
          size_t size = sizeof(unsigned char) * mesh.vertexCount * 4;
          memcpy(current, mesh.boneIds, size);
          current += size;
          // LOG_TRACE("After mesh %i bone IDs: %zu bytes\n", i, current - buffer);
        }
        if (mesh.boneWeights) {
          size_t size = sizeof(float) * mesh.vertexCount * 4;
          memcpy(current, mesh.boneWeights, size);
          current += size;
          // LOG_TRACE("After mesh %i bone weights: %zu bytes\n", i, current - buffer);
        }
        if (mesh.boneMatrices && mesh.boneCount > 0) {
          size_t size = sizeof(Matrix) * mesh.boneCount;
          memcpy(current, mesh.boneMatrices, size);
          current += size;
          // LOG_TRACE("After mesh %i boneMatrices: %zu bytes\n", i, current - buffer);
        }
      }

      // Write indices
      if (mesh.triangleCount > 0 && mesh.indices) {
        size_t size = sizeof(unsigned short) * mesh.triangleCount * 3;
        memcpy(current, mesh.indices, size);
        current += size;
        // LOG_TRACE("After mesh %i indices: %zu bytes\n", i, current - buffer);
      }
    }
  }

  // Write materials
  if (model.materials) {
    for (int i = 0; i < model.materialCount; i++) {
      char matId[128];  // Buffer for the material ID/ file name
      GetMaterialId(model.materials[i], matId, sizeof(matId));
      memcpy(current, &matId, sizeof(char[128]));
      current += sizeof(char[128]);
    }
  }

  // Write mesh material indices
  if (model.meshMaterial) {
    size_t size = sizeof(int) * model.meshCount;
    memcpy(current, model.meshMaterial, size);
    current += size;
    // LOG_TRACE("After mesh material: %zu bytes\n", current - buffer);
  }

  // BoneCount
  memcpy(current, &model.boneCount, sizeof(int));
  current += sizeof(int);
  // LOG_TRACE("After bone count: %zu bytes\n", current - buffer);

  if (model.boneCount > 0) {
    // BoneInfo
    if (model.bones) {
      size_t size = sizeof(BoneInfo) * model.boneCount;
      memcpy(current, model.bones, size);
      current += size;
      // LOG_TRACE("After bone info: %zu bytes\n", current - buffer);
    }

    // bindPose
    if (model.bindPose) {
      for (int i = 0; i < model.boneCount; i++) {
        memcpy(current, &model.bindPose[i].translation, sizeof(Vector3));
        current += sizeof(Vector3);
        // LOG_TRACE("After bind pose %i translation: %zu bytes\n", i, current - buffer);
        memcpy(current, &model.bindPose[i].rotation, sizeof(Vector4));
        current += sizeof(Vector4);
        // LOG_TRACE("After bind pose %i rotation: %zu bytes\n", i, current - buffer);
        memcpy(current, &model.bindPose[i].scale, sizeof(Vector3));
        current += sizeof(Vector3);
        // LOG_TRACE("After bind pose %i scale: %zu bytes\n", i, current - buffer);
      }
    }
  }

  write_file(filename, buffer, current - buffer);
}

void ExportModelAnimationToBinary(const ModelAnimation &anim, const char *filename, Arena& arena) {
  if (!filename) return;

  char* buffer = arena.alloc_raw<char>(KB(50));
  char* current = buffer;

  // Write counts
  memcpy(current, &anim.boneCount, sizeof(int));
  current += sizeof(int);
  memcpy(current, &anim.frameCount, sizeof(int));
  current += sizeof(int);

  // Write name
  memcpy(current, anim.name, sizeof(char) * 32);
  current += sizeof(char) * 32;

  // Write bones if they exist
  if (anim.bones) {
    size_t boneSize = sizeof(BoneInfo) * anim.boneCount;
    memcpy(current, anim.bones, boneSize);
    current += boneSize;
  }

  // Write frame poses
  if (anim.framePoses) {
    for (int frame = 0; frame < anim.frameCount; frame++) {
      for (int bone = 0; bone < anim.boneCount; bone++) {
        const Transform& transform = anim.framePoses[frame][bone];
        
        // Write translation
        memcpy(current, &transform.translation, sizeof(Vector3));
        current += sizeof(Vector3);
        
        // Write rotation
        memcpy(current, &transform.rotation, sizeof(Quaternion));
        current += sizeof(Quaternion);
        
        // Write scale
        memcpy(current, &transform.scale, sizeof(Vector3));
        current += sizeof(Vector3);
      }
    }
  }

  write_file(filename, buffer, current - buffer);
}

bool PackResources() {
  // Check if rrespacker exists
  FILE* test = fopen(RRESPACKER_PATH, "r");
  if (!test) {
    LOG_ERROR("Error: rrespacker not found at %s\n", RRESPACKER_PATH);
    return false;
  }
  fclose(test);

  // Create command
  char cmd[512];
  snprintf(cmd, sizeof(cmd), "\"%s\" -o resources.rres --rrp resources.rrp", RRESPACKER_PATH);

  // Execute command
  int result = system(cmd);
  return result == 0;
}

int main(int argc, char *argv[]) {
  SetConfigFlags(FLAG_WINDOW_HIDDEN);
  InitWindow(800, 450, "prep models");

  Arena& arena = * new Arena(GB(3));
  MapCT<const char*, Model, 100>& modelMap = arena.create_map_ct<const char*, Model, 100>();
  // Store both animations and count
  struct AnimData {
    ModelAnimation* animations;
    int count;
  };
  MapCT<const char*, AnimData, 100>& animMap = arena.create_map_ct<const char*, AnimData, 100>();

  ArrayCT<const char*, 100>& allFiles = listFiles("resources/models", arena);

  ArrayCT<const char*, 100>& models = arena.create_array_ct<const char*, 100>();
  for (uint32_t i = 0; i < allFiles.size(); i++) {
    if (strstr(allFiles[i], ".gltf")) {
      models.add(allFiles[i]);
    }
  }

  const char* MODELS_OUTPUT_DIR = "./resources/models/";
  const char* ANIMATIONS_OUTPUT_DIR = "./resources/animations/";

  char model_path[256];
  const char* mat_path = "./resources/materials/";
  char anim_path[256];

  for (uint32_t i = 0; i < models.size(); i++) {
    const char *in = models[i];
    const char *filename = strrchr(in, '/');
    filename = filename ? filename + 1 : in;

    // Create model filename without .gltf extension
    char model_filename[256];
    snprintf(model_filename, sizeof(model_filename), "%.*s.bin", (int)(strlen(filename) - 5), filename);

    // Create animation binary filename
    char anim_filename[256];
    snprintf(anim_filename, sizeof(anim_filename), "%.*s.anim", (int)(strlen(filename) - 5), filename);

    // Construct full output paths
    size_t remaining = sizeof(model_path);
    strncpy(model_path, MODELS_OUTPUT_DIR, remaining);
    remaining -= strlen(MODELS_OUTPUT_DIR);
    strncat(model_path, model_filename, remaining - 1);

    remaining = sizeof(anim_path);
    strncpy(anim_path, ANIMATIONS_OUTPUT_DIR, remaining);
    remaining -= strlen(ANIMATIONS_OUTPUT_DIR);
    strncat(anim_path, anim_filename, remaining - 1);

    LOG_TRACE("%s -> %s", in, model_path);

    // Load model and animations
    Model model = LoadModel(in);
    int animCount = 0;
    ModelAnimation* animations = LoadModelAnimations(in, &animCount);

    // Export model
    ExportModelToBinary(model, model_path, arena);

    // Export materials if any exist
    ExportMaterialsToBinary(model, mat_path, arena);

    // Export animations if any exist
    if (animations != nullptr && animCount > 0) {  // Changed order of checks
      LOG_TRACE("Exporting %d animations for %s", animCount, filename);
      for (int j = 0; j < animCount; j++) {
        // Create unique filename using animation name
        char numbered_anim_path[256];
        size_t max_len = sizeof(numbered_anim_path);

        // Remove the .anim extension before adding the animation name
        const char* base_anim_path = anim_path;
        size_t anim_path_len = strlen(anim_path);
        if (anim_path_len > 5) {  // Make sure we have enough characters for ".anim"
          anim_path_len -= 5;  // Remove ".anim"
        }

        int written = snprintf(numbered_anim_path, max_len, 
                               "%.*s_%s.anim",
                               (int)anim_path_len,
                               base_anim_path, 
                               animations[j].name);

        if (written < 0 || static_cast<size_t>(written) >= max_len) {
          LOG_ERROR("Path too long for animation %s", animations[j].name);
          continue;
        }

        ExportModelAnimationToBinary(animations[j], numbered_anim_path, arena);
        LOG_TRACE("Exported animation '%s' to %s", animations[j].name, numbered_anim_path);
      }

      // Store in maps with persistent keys
      size_t key_len = strlen(anim_filename) + 1;
      char* persistent_key = arena.alloc_count_raw<char>(key_len);
      strcpy(persistent_key, anim_filename);
      animMap[persistent_key] = {animations, animCount};
    } else {
      LOG_TRACE("No animations found for %s", filename);
    }

    // Store model in map
    size_t key_len = strlen(model_filename) + 1;
    char* persistent_key = arena.alloc_count_raw<char>(key_len);
    strcpy(persistent_key, model_filename);
    modelMap[persistent_key] = model;
  }

  if (!PackResources()) {
    LOG_ERROR("Failed to pack resources\n");
    return 1;
  }

  // Testing
  rresCentralDir dir = rresLoadCentralDirectory("resources.rres");
  MapCT<const char*, Material, 100>& materialPool = arena.create_map_ct<const char*, Material, 100>();

  // Test models
  for (auto &[path, testModel] : modelMap) {
    int idModel = rresGetResourceId(dir, path);
    rresResourceChunk chunkModel = rresLoadResourceChunk("resources.rres", idModel);
    Model& modelTest = LoadModelFromChunkTest(chunkModel, testModel, arena, materialPool, dir);
    UnloadModel(testModel);
    rresUnloadResourceChunk(chunkModel);
  }

  // Test animations
  for (auto &[path, animData] : animMap) {
    LOG_TRACE("Testing animations for path: %s (count: %d)", path, animData.count);

    // Extract base path without extension
    char base_path[256];
    size_t path_len = strlen(path);
    if (path_len > 5) {
      size_t base_len = path_len - 5; // Remove .anim
      if (base_len >= sizeof(base_path)) {
        base_len = sizeof(base_path) - 1;
      }
      strncpy(base_path, path, base_len);
      base_path[base_len] = '\0';
    } else {
      strcpy(base_path, path);
    }

    LOG_TRACE("Base path for animations: %s", base_path);

    // Test each animation
    for (int animIndex = 0; animIndex < animData.count; animIndex++) {
      char anim_path[256];
      size_t max_len = sizeof(anim_path);
      int written = snprintf(anim_path, max_len, "%s_%s.anim", 
                             base_path, 
                             animData.animations[animIndex].name);

      LOG_TRACE("Trying to load animation: %s", anim_path);

      if (written < 0 || static_cast<size_t>(written) >= max_len) {
        LOG_ERROR("Path too long for animation %s", animData.animations[animIndex].name);
        continue;
      }

      int idAnim = rresGetResourceId(dir, anim_path);
      if (idAnim == -1) {
        LOG_ERROR("Animation resource not found: %s (ID lookup failed)", anim_path);
        continue;
      }

      LOG_TRACE("Found animation resource ID: 0x%08x", idAnim);
      
      rresResourceChunk chunkAnim = rresLoadResourceChunk("resources.rres", idAnim);
      if (!chunkAnim.data.raw) {
        LOG_ERROR("Failed to load animation chunk: %s (chunk data null)", anim_path);
        continue;
      }

      ModelAnimation& animTest = LoadModelAnimationFromChunkTest(chunkAnim, animData.animations[animIndex], arena);
      rresUnloadResourceChunk(chunkAnim);
    }

    // Unload original animations
    UnloadModelAnimations(animData.animations, animData.count);
  }

  LOG_TRACE("Done!!");

  rresUnloadCentralDirectory(dir);
  delete &arena;

  return 0;
}
