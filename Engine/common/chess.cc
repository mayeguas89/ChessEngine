#include "chess.h"

#include "engine.h"
#include "glad/gl.h"
#include "spdlog/spdlog.h"

#include <filesystem>

namespace
{
using namespace chess;
template<typename T>
std::shared_ptr<T> CreatePiece(const Color color,
                               const std::filesystem::path& spriteFilePath,
                               const glm::vec2& position,
                               const uint32_t boardRow)
{
  static const float zOffset{-0.01f};
  auto piece = std::make_shared<T>(spriteFilePath.string(), color);
  piece->boardPosition = {position.x, boardRow};
  piece->SetPosition({position.x, position.y, zOffset, 0.f});
  return std::move(piece);
}

std::vector<std::shared_ptr<Piece>> CreatePieces(const Color color)
{
  static const std::filesystem::path spriteFilePath{"Assets/Madware's Chess Set/tileset_1024.png"};
  std::vector<std::shared_ptr<Piece>> pieces;
  for (uint32_t i = 0; i < 8; ++i)
  {
    auto x = i;
    auto y = (color == Color::White) ? 6 : 1;
    auto boardRow = (color == Color::White) ? 1 : 6;
    auto pawn = CreatePiece<Pawn>(color, spriteFilePath, glm::vec2{x, y}, boardRow);
    pieces.push_back(std::move(pawn));
  }

  auto xBishopLeft = 2;
  auto xBishopRight = 5;
  auto y = (color == Color::White) ? 7 : 0;
  auto boardRow = (color == Color::White) ? 0 : 7;
  pieces.push_back(std::move(CreatePiece<Bishop>(color, spriteFilePath, glm::vec2{xBishopLeft, y}, boardRow)));
  pieces.push_back(std::move(CreatePiece<Bishop>(color, spriteFilePath, glm::vec2{xBishopRight, y}, boardRow)));

  auto xRockLeft = 0;
  auto xRockRight = 7;
  pieces.push_back(std::move(CreatePiece<Rock>(color, spriteFilePath, glm::vec2{xRockLeft, y}, boardRow)));
  pieces.push_back(std::move(CreatePiece<Rock>(color, spriteFilePath, glm::vec2{xRockRight, y}, boardRow)));

  auto xKnightLeft = 1;
  auto xKnightRight = 6;
  pieces.push_back(std::move(CreatePiece<Knight>(color, spriteFilePath, glm::vec2{xKnightLeft, y}, boardRow)));
  pieces.push_back(std::move(CreatePiece<Knight>(color, spriteFilePath, glm::vec2{xKnightRight, y}, boardRow)));

  auto x = 4;
  pieces.push_back(std::move(CreatePiece<Queen>(color, spriteFilePath, glm::vec2{x, y}, boardRow)));

  x = 3;
  pieces.push_back(CreatePiece<King>(color, spriteFilePath, glm::vec2{x, y}, boardRow));
  return std::move(pieces);
}
}

namespace chess
{

Player::Player(const Color color): color_{color} {}
void Player::Init()
{
  pieces_ = CreatePieces(color_);
}

Player::~Player() {}

Piece::Piece(const std::string& textureFile, const glm::vec<2, int>& spriteView):
  Sprite2D(textureFile, pieceTextureSize, spriteView)
{}

void Piece::Draw()
{
  Sprite2D::Draw();
}

void Piece::Update()
{
  Entity::Update();
}

Pawn::Pawn(const std::string& textureFile, const Color color):
  Piece(textureFile, (color == Color::White) ? wSpriteView : bSpriteView)
{
  name_ = "Pawn";
}

Bishop::Bishop(const std::string& textureFile, const Color color):
  Piece(textureFile, (color == Color::White) ? wSpriteView : bSpriteView)
{
  name_ = "Bishop";
}

Rock::Rock(const std::string& textureFile, const Color color):
  Piece(textureFile, (color == Color::White) ? wSpriteView : bSpriteView)
{
  name_ = "Rock";
}

Queen::Queen(const std::string& textureFile, const Color color):
  Piece(textureFile, (color == Color::White) ? wSpriteView : bSpriteView)
{
  name_ = "Queen";
}

King::King(const std::string& textureFile, const Color color):
  Piece(textureFile, (color == Color::White) ? wSpriteView : bSpriteView)
{
  name_ = "King";
}

Knight::Knight(const std::string& textureFile, const Color color):
  Piece(textureFile, (color == Color::White) ? wSpriteView : bSpriteView)
{
  name_ = "Knight";
}

Tile::Tile(const glm::vec4& color): Entity()
{
  program_.AddShader("Assets/shaders/tile.vertex", ShaderType::Vertex);
  program_.AddShader("Assets/shaders/tile.fragment", ShaderType::Fragment);
  if (!program_.Link())
    spdlog::error("Error in shaders!");

  std::vector<Vertex> vertices(vertexPosition_.size());
  std::generate(vertices.begin(),
                vertices.end(),
                [&, n = 0]() mutable
                {
                  auto vertex = Vertex{vertexPosition_.at(n), color, glm::vec2{0.f, 0.f}};
                  n++;
                  return vertex;
                });

  glGenVertexArrays(1, &bufferObject_.id);
  glGenBuffers(1, &bufferObject_.VBO);
  glBindBuffer(GL_ARRAY_BUFFER, bufferObject_.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &bufferObject_.VIO);
  glBindBuffer(GL_ARRAY_BUFFER, bufferObject_.VIO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * indices_.size(), indices_.data(), GL_STATIC_DRAW);
}

void Tile::Draw()
{
  glBindVertexArray(bufferObject_.id);
  // Se hace el bind buffer y se suben los datos pertenecientes a este buffer
  glBindBuffer(GL_ARRAY_BUFFER, bufferObject_.VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject_.VIO);

  program_.Use();
  program_.SetVertextAttribArray("vertexPosition", 4, GL_FLOAT, sizeof(Vertex), (void*)0x00);
  program_.SetVertextAttribArray("color", 4, GL_FLOAT, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
  program_.SetUniformMat4("Model", GetModelMatrix());
  // Set Common stuff like camera matrix
  // program_.SetVariables();
  program_.SetUniformMat4("Projection", Engine::Instance().GetMainCamera().GetProjectionMatrix());
  program_.SetUniformMat4("View", Engine::Instance().GetMainCamera().GetViewMatrix());

  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, nullptr);
}

Board::Board(): Entity()
{
  tiles.resize(8 * 8);
  std::generate(tiles.begin(),
                tiles.end(),
                [n = 0]() mutable
                {
                  auto x = std::floor(n / 8.f);
                  auto y = n % 8;
                  glm::vec4 color;
                  if ((uint32_t)x % 2 == 0)
                    color = (n % 2 == 0) ? BLACK_COLOR : WHITE_COLOR;
                  else
                    color = (n % 2 == 0) ? WHITE_COLOR : BLACK_COLOR;
                  auto tile = std::make_shared<Tile>(color);
                  tile->SetScale({1.f, 1.f, 1.f, 0.f});
                  tile->SetPosition(glm::vec4(x, y, 0.f, 1.f));
                  n++;
                  return std::move(tile);
                });
}

void Board::Draw()
{
  std::for_each(tiles.begin(), tiles.end(), [](auto& tile) { tile->Draw(); });
}

void Board::Update()
{
  std::for_each(tiles.begin(), tiles.end(), [](auto& tile) { tile->Update(); });
}
}