#ifndef __TERRAIN__
#define __TERRAIN__

#include <Core/Texture.hpp>
#include <Core/Shader.hpp>
#include <Core/Node.hpp>
#include <Core/Noise/Perlin.hpp>

struct TerrainChunk {
  float* heightmap;
  uint32_t i;
  uint32_t j;
};

class Terrain : public Node {
private:
  Perlin* noise;
  Shader* shader;

  const uint32_t chunkSize = 1024;
  std::map <std::pair<int, int>, TerrainChunk*> chunks;
  std::vector <Texture*> textures;

  void LoadChunk(std::pair<int, int> pos);
  void UnloadChunk(std::pair<int, int> pos);
  void RenderChunk(TerrainChunk* chunk);

  virtual void RenderAll(Event &e);

public:
  Terrain();
  ~Terrain();

  virtual void OnAttach(Node*);
  virtual void OnDetach(Node*);

  virtual void Tick(uint64_t time);
};

#endif
