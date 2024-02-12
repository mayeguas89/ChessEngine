#pragma once

#include "entity.h"
#include "glm/glm.hpp"
#include "sprite.h"
#include "utils/types.h"

#include <memory>
#include <vector>

namespace chess
{
struct Tile: public Entity
{
  Tile(const glm::vec4& color);
  struct BufferObject
  {
    unsigned int id, VBO, VIO;
  };
  virtual void Draw() override;
  BufferObject bufferObject_;
  std::vector<Vertex> vertices_;
  glm::vec<2, int> boardPosition_;

  inline static const std::array<glm::vec4, 4> vertexPosition_{glm::vec4{-0.5f, -0.5f, 0.f, 1.f},
                                                               glm::vec4{-0.5f, 0.5f, 0.f, 1.f},
                                                               glm::vec4{0.5f, 0.5f, 0.f, 1.f},
                                                               glm::vec4{0.5f, -0.5f, 0.f, 1.f}};

  inline static const std::array<uint32_t, 6> indices_{0, 1, 2, 2, 3, 0};
  RenderProgram program_;
};

struct Board: public Entity
{
  inline static const glm::vec4 BLACK_COLOR{119.f / 255.f, 153.f / 255.f, 84.f / 255.f, 1.f};
  inline static const glm::vec4 WHITE_COLOR{233.f / 255.f, 237.f / 255.f, 204.f / 255.f, 1.f};
  Board();
  std::vector<std::shared_ptr<Tile>> tiles;
  virtual void Draw() override;
  virtual void Update() override;
};

struct Piece: public Sprite2D
{
  inline static const glm::vec<2, int> pieceTextureSize{1024, 1024};
  Piece(const std::string& textureFile, const glm::vec<2, int>& spriteView);
  virtual void Draw() override;
  virtual void Update() override;
  virtual std::vector<glm::vec<2, int>> GetMovementsPosition() = 0;
  void SetBoardPosition(const glm::vec<2, int>& position);
  glm::vec<2, int> boardPosition;
};

struct Pawn: public Piece
{
  inline static const glm::vec<2, int> wSpriteView{5, 1};
  inline static const glm::vec<2, int> bSpriteView{5, 0};
  Pawn(const std::string& textureFile, const Color color);
  std::vector<glm::vec<2, int>> GetMovementsPosition() override;
};

struct Bishop: public Piece
{
  inline static const glm::vec<2, int> wSpriteView{2, 1};
  inline static const glm::vec<2, int> bSpriteView{2, 0};
  Bishop(const std::string& textureFile, const Color color);
  std::vector<glm::vec<2, int>> GetMovementsPosition() override;
};

struct Rock: public Piece
{
  inline static const glm::vec<2, int> wSpriteView{0, 1};
  inline static const glm::vec<2, int> bSpriteView{0, 0};
  Rock(const std::string& textureFile, const Color color);
  std::vector<glm::vec<2, int>> GetMovementsPosition() override;
};

struct Queen: public Piece
{
  inline static const glm::vec<2, int> wSpriteView{4, 1};
  inline static const glm::vec<2, int> bSpriteView{4, 0};
  Queen(const std::string& textureFile, const Color color);
  std::vector<glm::vec<2, int>> GetMovementsPosition() override;
};

struct King: public Piece
{
  inline static const glm::vec<2, int> wSpriteView{3, 1};
  inline static const glm::vec<2, int> bSpriteView{3, 0};
  King(const std::string& textureFile, const Color color);
  std::vector<glm::vec<2, int>> GetMovementsPosition() override;
};

struct Knight: public Piece
{
  inline static const glm::vec<2, int> wSpriteView{1, 1};
  inline static const glm::vec<2, int> bSpriteView{1, 0};
  Knight(const std::string& textureFile, const Color color);
  std::vector<glm::vec<2, int>> GetMovementsPosition() override;
};

class Player
{
private:
  inline static const uint32_t NUMBER_OF_INITIAL_PIECES{16};
  Color color_;

public:
  inline static glm::vec3 blackTransparentColor{0.f, 128.f / 255.f, 128.f / 255.f};
  inline static glm::vec3 whiteTransparentColor{0.f, 0.f, 0.f};
  void Init();
  std::vector<std::shared_ptr<Piece>> pieces_;
  Player(const Color color);

  ~Player();
};

}