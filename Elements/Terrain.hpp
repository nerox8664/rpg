#ifndef __TERRAIN__
#define __TERRAIN__

#include <Core/Texture.hpp>
#include <Core/Node.hpp>
#include <Core/Noise/Perlin.hpp>

struct TerrainChunk {
  std::vector < std::vector <int> > heightmap;
};

class Terrain : public Node {
private:
  Perlin* noise;

  std::map <std::pair<int, int>, TerrainChunk*> chunks;
  void LoadChunk(std::pair<int, int> pos);
  void UnloadChunk(std::pair<int, int> pos);
  void RenderChunk(TerrainChunk* chunk);

public:
  Terrain();
  ~Terrain();

  virtual void OnAttach(Node*);
  virtual void OnDetach(Node*);

  virtual void Tick(Node*);
};

#endif
