#include <Elements/Terrain.hpp>

Terrain::Terrain() {
  noise = new Perlin(6, 0.14, 0.3, 1488);
  shader = new Shader("./data/shaders/terrain/terrain.vert", "./data/shaders/terrain/terrain.frag");
  textures.push_back(new Texture("./data/textures/terrain/dirt.png"));
  textures.push_back(new Texture("./data/textures/terrain/grass.png"));
  textures.push_back(new Texture("./data/textures/terrain/rock.png"));
  textures.push_back(new Texture("./data/textures/terrain/snow.png"));
  textures.push_back(new Texture("./data/textures/terrain/water.png"));
  LoadChunk(std::pair<int, int>(0,0));

  using namespace std::placeholders;
  std::function<void(Event &e)> drawEvent = std::bind(&Terrain::RenderAll, (Terrain*)this, _1);
  Engine::GetInstance()->Subscribe(
    std::pair<Event_t, Event_sub_t>(EVENT_TYPE_GENERAL, EVENT_GENERAL_SCENE_RENDER),
    drawEvent
  );
}

void Terrain::LoadChunk(std::pair<int, int> pos) {
  TerrainChunk* chunk = new TerrainChunk;

  chunk->i = pos.first;
  chunk->j = pos.second;
  chunk->heightmap = new float[chunkSize * chunkSize];

  for (int i = 0; i < chunkSize; i++) {
    for (int j = 0; j< chunkSize; j++) {
      chunk->heightmap[i * chunkSize + j] = noise->Get(
        pos.first * chunkSize + i,
        pos.second * chunkSize + j
      );
    }
  }

  chunks[pos] = chunk;
}

void Terrain::RenderChunk(TerrainChunk* chunk) {
  /*GLuint texture=-1;

  // allocate a texture name
  glGenTextures( 1, &texture );

  // select our current texture
  glBindTexture( GL_TEXTURE_2D, texture );

  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0 , 4, 256, 256, 0, GL_RED, GL_UNSIGNED_BYTE, chunk->heightmap);

  glActiveTexture(GL_TEXTURE0 + 5);
  glBindTexture(GL_TEXTURE_2D, texture);

*/
  glPushMatrix();
  glTranslatef(chunk->i * chunkSize, chunk->j * chunkSize, 0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  GLdouble vertices[] = {
    0, 0,
    0, 256,
    256, 0,
    256, 256
  };
  GLdouble texVertices[] = {
    1, 1,
    0, 1,
    1, 0,
    0, 0
  };

  glTexCoordPointer(2, GL_DOUBLE, 0, texVertices);
  glVertexPointer(2, GL_DOUBLE, 0, vertices);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  glPopMatrix();
}

Terrain::~Terrain() {
  delete noise;
}

void Terrain::OnAttach(Node*) {

}

void Terrain::OnDetach(Node*) {

}

void Terrain::RenderAll(Event &e) {
  for (int i = 0; i < textures.size(); i++) {
    textures[i]->Bind(i);
  }
  shader->Bind();
  shader->SetUniform1i("TerrainTexture1", 0);
  shader->SetUniform1i("TerrainTexture2", 1);
  shader->SetUniform1i("TerrainTexture3", 2);
  shader->SetUniform1i("TerrainTexture4", 3);
  shader->SetUniform1i("TerrainTexture5", 4);

  shader->SetUniform1i("heightTexture", 5);

  for (auto i: chunks) {
    RenderChunk(i.second);
  }

  shader->Unbind();
  for (int i = 0; i < textures.size(); i++) {
    textures[i]->Unbind();
  }
}

void Terrain::Tick(uint64_t time) {

}
