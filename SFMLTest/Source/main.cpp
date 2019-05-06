#include <tb_types.h>
#include <tb_Vector2.h>
#include <tb_math.h>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

float
clamp(float value, float min, float max) {
  if (value < min) {
    return min;
  }
  else if (value > max) {
    return max;
  }
  return value;
}

toyboxSDK::UInt32
clamp(toyboxSDK::UInt32 value, toyboxSDK::UInt32 min, toyboxSDK::UInt32 max) {
  if (value < min) {
    return min;
  }
  else if (value > max) {
    return max;
  }
  return value;
}

struct Node {
  bool walkable;
  float weight;
  Node* parent;
  Node* goal;

  toyboxSDK::Vector2 position;

  float f, g, h;

  toyboxSDK::UInt32 tileIDY, tileIDX, uniqueID;

  Node() {
    walkable = true;
    parent = nullptr;
    goal = nullptr;
    weight = 1.0f;
    position = toyboxSDK::Vector2(0.0f, 0.0f);
    f = g = h = 0.0f;
    tileIDX = tileIDY = uniqueID = static_cast<toyboxSDK::UInt32>(-1);
  }

  void
  AddParent(Node* newParent) {
    if (newParent != nullptr) {
      if (newParent->goal != nullptr) {
        parent = newParent;
        goal = parent->goal;

        //g = parent->g + (parent->position - position).length();
        //h = (goal->position - position).length();
        //Squared lenght for speed purposes
        g = parent->g + (parent->position - position).lengthSqr();
        h = (goal->position - position).lengthSqr();
        f = g + h + weight;
      }
    }
  }
};

class NodeComparer
{
 public:
  bool
  operator() (const Node* LValue, const Node* RValue) {
    return LValue->f > RValue->f;
  }
};

using Layer = std::vector<Node>;
using Grid = std::vector<Layer>;

bool
validID(const toyboxSDK::Int32 x,
        const toyboxSDK::Int32 y,
        const toyboxSDK::UInt32 maxX,
        const toyboxSDK::UInt32 maxY) {
  return (x >= 0) && (x < static_cast<toyboxSDK::Int32>(maxX)) &&
         (y >= 0) && (y < static_cast<toyboxSDK::Int32>(maxY));
}

std::vector<Node*>
AddSuccessorsToList(Grid& tileMap,
                    std::priority_queue<Node*, std::vector<Node*>, NodeComparer>& openList,
                    std::unordered_map<toyboxSDK::UInt32, Node*>& closedList,
                    const toyboxSDK::UInt32 tileX,
                    const toyboxSDK::UInt32 tileY,
                    const toyboxSDK::UInt32 tileMapWidth,
                    const toyboxSDK::UInt32 tileMapHeight) {
  std::vector<Node*> successors;
  bool corners = true;

  std::vector<std::pair<toyboxSDK::Int32, toyboxSDK::Int32>> adjacentIDs;

  adjacentIDs.push_back({  0,  1 }); //up
  adjacentIDs.push_back({  0, -1 }); //down
  adjacentIDs.push_back({ -1,  0 }); //left
  adjacentIDs.push_back({  1,  0 }); //right
  if (corners) {
    adjacentIDs.push_back({ -1,  1 }); //up left
    adjacentIDs.push_back({  1,  1 }); //up right
    adjacentIDs.push_back({ -1, -1 }); //down left
    adjacentIDs.push_back({  1, -1 }); //down right
  }

  toyboxSDK::Int32 currentX, currentY;

  Node* contenderNode;
  for (const auto& offset : adjacentIDs) {
    currentY = clamp(tileY + offset.second, 0, tileMapHeight);
    currentX = clamp(tileX + offset.first, 0, tileMapWidth);

    if (validID(currentX, currentY, tileMapWidth, tileMapHeight)) {
      contenderNode = &tileMap[currentY][currentX];
      if (contenderNode->walkable) {
        if (closedList.find(contenderNode->uniqueID) == closedList.end()) {
          contenderNode->AddParent(&tileMap[tileY][tileX]);

          successors.push_back(contenderNode);
          closedList[contenderNode->uniqueID] = contenderNode;

          openList.push(contenderNode);
        }
      }
    }
  }

  return successors;
}

void
PrintPath(Node* pathEnd) {
  Node* parent = pathEnd;
  toyboxSDK::UInt32 totalNodes = 0;
  while (parent != nullptr) {
    std::cout << parent->uniqueID << ", ";
    parent = parent->parent;
    ++totalNodes;
  }
  std::cout << std::endl << "Total nodes: " << totalNodes << std::endl;
}

toyboxSDK::Int32
main() {
  if (true) {
    toyboxSDK::UInt32 mapWidth = 20;
    toyboxSDK::UInt32 mapHeight = 10;

    Grid tileMap;
    tileMap.resize(mapHeight, Layer(mapWidth));

    std::cout << "number of layers: " << tileMap.size() << std::endl;
    std::cout << "tiles per layer: " << tileMap.front().size() << std::endl;

    std::cout << std::endl << "Grid" << std::endl << std::endl;

    //Initialize tile map
    {
      toyboxSDK::Vector2 position = toyboxSDK::Vector2(0.0f, 0.0f);
      toyboxSDK::UInt32 idX = 0, idY = 0, uniqueID = 0;

      for (auto& layer : tileMap) {
        position.x = 0.0f;
        idX = 0;
        for (auto& node : layer) {
          node.position = position;
          node.tileIDX = idX;
          node.tileIDY = idY;
          node.uniqueID = uniqueID;

          position.x += 1.0f;
          ++idX;
          ++uniqueID;

          //std::cout << node.weight << " ";
          //std::cout << node.uniqueID << "\t";
        }
        position.y += 1.0f;
        ++idY;

        //std::cout << std::endl;
      }

    }

    std::priority_queue<Node*, std::vector<Node*>, NodeComparer> openList;
    std::unordered_map<toyboxSDK::UInt32, Node*> closedList;

    //Initialize open and closed lists.
    Node* startPoint = &tileMap[0][0];
    Node* endPoint = &tileMap[mapHeight - 1][mapWidth - 1];
    startPoint->parent = nullptr;
    startPoint->goal = endPoint;

    openList.push(startPoint);

    Node* q = nullptr;
    std::vector<Node*> successors;
    bool pathFound = false;
    sf::Clock timer;
    timer.restart();

    while (!openList.empty() && !pathFound) {
      q = openList.top();
      openList.pop();
      closedList[q->uniqueID] = q;

      successors = AddSuccessorsToList(tileMap,
                                       openList, closedList,
                                       q->tileIDX, q->tileIDY,
                                       mapWidth, mapHeight);

      for (auto& node : successors) {
        if (node == endPoint) {
          std::cout << "Goal found" << std::endl;
          PrintPath(node); std::cout << std::endl;
          pathFound = true;
        }
      }

      successors.clear();

    }
    float totaltime = timer.getElapsedTime().asSeconds();


    std::cout << "Pathfinding ended in " << totaltime << " seconds" << std::endl;
    std::cin;
  }
  if (false) {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Level editor");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = true;
    style.WindowBorderSize = 1.0f;

    sf::Color bgColor;
    float color[3] = { 0.f, 0.f, 0.f };

    sf::RenderTexture* RT_SceneViewer = new sf::RenderTexture;
    RT_SceneViewer->create(1280, 720);

    sf::FloatRect textBounds;
    textBounds.left = 0.0f;
    textBounds.top = 0.0f;
    textBounds.width = static_cast<float>(RT_SceneViewer->getSize().x);
    textBounds.height = static_cast<float>(RT_SceneViewer->getSize().y);

    sf::CircleShape circle;
    circle.setRadius(50.0f);
    circle.setOrigin(50.0f, 50.0f);
    circle.setPosition(0.0f, 0.0f);
    circle.setFillColor(sf::Color::Magenta);

    sf::Vector2f cameraSz = { 128.0f, 72.0f };
    float cameraScale = 1.0f;
    sf::View debugCamera({ 0.f, 0.f }, cameraSz);
    debugCamera.setCenter(0.0f, 0.0f);

    sf::Vector2f lastWndSz = {0.0f, 0.0f};

    window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
    sf::Clock deltaClock;
    float deltaTime = 0.0f;
    //float camSpeed = 250.0f;
    //float camZoom = 10.0f;
    sf::Vector2f lastMousePos;

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed) {
          window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::Escape) {
            window.close();
          }
        }
        if (event.type == sf::Event::MouseWheelMoved) {
          
        }
      }

      deltaTime = deltaClock.restart().asSeconds();
      ImGui::SFML::Update(window, deltaTime);

      RT_SceneViewer->setView(debugCamera);
      RT_SceneViewer->clear(bgColor);
      //Draw gameobjects
      RT_SceneViewer->draw(circle);
      RT_SceneViewer->display();

      ImGui::Begin("Example window");
      {
        if (ImGui::ColorEdit3("Background color", color)) {
          bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
          bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
          bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
        }
      }
      ImGui::End(); //Example window

      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
      ImGui::Begin("Scene viewer");
      {
        const float titleBarHeight = ImGui::GetTextLineHeight() +
                                     (ImGui::GetStyle().FramePadding.y * 2.0f);

        sf::Vector2f contentSz = ImGui::GetWindowContentRegionMax();
        contentSz.y -= titleBarHeight;

        //std::cout << ImGui::GetIO().MouseWheel << std::endl;
        if (ImGui::IsWindowHovered()) {
          if (ImGui::GetIO().MouseWheel != 0) {
            if (-ImGui::GetIO().MouseWheel > 0.5f) {
              cameraScale *= 1.1f;
            }
            else {
              cameraScale *= 0.9f;
            }
            debugCamera.setSize(cameraSz * cameraScale);
          }
        }

        sf::Vector2f mousePos = ImGui::GetMousePos();
        sf::Vector2f windowPos = ImGui::GetWindowPos();
        mousePos -= windowPos;
        mousePos.y -= titleBarHeight;
        //mousePos.x = clamp(mousePos.x, 0.0f, contentSz.x);
        //mousePos.y = clamp(mousePos.y, 0.0f, contentSz.y);

        //if mouse right pressed
        if (ImGui::IsMouseClicked(1, true)) {
          sf::Vector2f mouseDeltaPos = lastMousePos - mousePos;
          mouseDeltaPos.x /= contentSz.x;
          mouseDeltaPos.y /= contentSz.y;

          sf::Vector2f moveOffset = (debugCamera.getSize() * cameraScale);
          moveOffset.x *= mouseDeltaPos.x;
          moveOffset.y *= -mouseDeltaPos.y;
          debugCamera.move(moveOffset * 1.5f);
        }

        //if resized
        if (contentSz != lastWndSz) {
          lastWndSz = contentSz;

          //delete RT_SceneViewer;
          //RT_SceneViewer = new sf::RenderTexture;
          //RT_SceneViewer->create(contentSz.x, contentSz.y);
          cameraSz = contentSz;
          debugCamera.setSize(cameraSz * cameraScale);
        }

        ImGui::Image(RT_SceneViewer->getTexture(), contentSz);
        lastMousePos = mousePos;
      } 
      ImGui::End(); //Scene viewer
      ImGui::PopStyleVar();

      window.clear(sf::Color::Black);
      ImGui::SFML::Render(window);
      window.display();
    }

    ImGui::SFML::Shutdown();
  }

  return 0;
}