#include "../../sdk/Rack-SDK/dep/include/nanovg.h"
#include "RGBLinearInterpolator.hpp"
#include "chord.hpp"
#include "plugin.hpp"
#include "rack.hpp"
#include "tps.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <mutex>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

// Use nlohmann/json for JSON parsing
using json = nlohmann::json;

inline bool operator==(const NVGcolor &lhs, const NVGcolor &rhs) {
  return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
};

inline bool operator<(const NVGcolor &lhs, const NVGcolor &rhs) {
  if (lhs.r != rhs.r)
    return lhs.r < rhs.r;
  if (lhs.g != rhs.g)
    return lhs.g < rhs.g;
  if (lhs.b != rhs.b)
    return lhs.b < rhs.b;
  return lhs.a < rhs.a;
};

std::string nvcolor_to_string(const NVGcolor &color) {
  std::ostringstream stream;
  stream << std::fixed << std::setprecision(2);
  stream << color.r << ", ";
  stream << color.g << ", ";
  stream << color.b;
  return stream.str();
}

template <typename T> struct Vertex_ {
  T x, y, z, w;
  Vertex_() : x(T()), y(T()), z(T()), w(T()) {}

  Vertex_(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

  // This constructor allows initialization from an initializer list of integers
  Vertex_(std::initializer_list<int> list) {
    auto it = list.begin();
    x = it != list.end() ? T(*it++) : T();
    y = it != list.end() ? T(*it++) : T();
    z = it != list.end() ? T(*it++) : T();
    w = it != list.end() ? T(*it) : T();
  }

  bool operator<(const Vertex_ &other) const {
    if (x != other.x)
      return x < other.x;
    if (y != other.y)
      return y < other.y;
    if (z != other.z)
      return z < other.z;
    return w < other.w;
  }

  bool operator==(const Vertex_ &other) const {
    return x == other.x && y == other.y && z == other.z && w == other.w;
  }

  std::string to_string() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
           std::to_string(z) + ", " + std::to_string(w) + ")";
  }
};

using Vertex = Vertex_<int>;
using VertexZZ_7 = Vertex_<ZZ_7>;
using VertexZZ_12 = Vertex_<ZZ_12>;

// Function to compute the pitch:  compute the index in the underlaying scale
// (eg, 3rd) to get the number of semitones for that interval; then multiply by
// the position in that coordinate
int computePitch(const HeptatonicScale &scale, const VertexZZ_7 &intervals,
                 const Vertex &coord) {
  return (static_cast<int>(scale[intervals.x.unMod()].unMod()) * coord.x) +
         (static_cast<int>(scale[intervals.y.unMod()].unMod()) * coord.y) +
         (static_cast<int>(scale[intervals.z.unMod()].unMod()) * coord.z) +
         (static_cast<int>(scale[intervals.w.unMod()].unMod()) * coord.w);
}

std::string toStringVertex(const Vertex &v) {
  return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " +
         std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
}

struct Polygon {
  std::vector<std::pair<float, float>> faceVerts; // 2D vertices
  std::vector<Vertex> vertexCoords;               // 4D coordinates
  bool operator==(const Polygon &other) const {
    return faceVerts == other.faceVerts && vertexCoords == other.vertexCoords;
  }

  bool operator<(const Polygon &other) const {
    if (faceVerts != other.faceVerts)
      return faceVerts < other.faceVerts;
    return vertexCoords < other.vertexCoords;
  }
};

// Hash function for Vertex
struct VertexHash {
  size_t operator()(const Vertex &v) const {
    return std::hash<int>()(v.x) ^ std::hash<int>()(v.y) ^
           std::hash<int>()(v.z) ^ std::hash<int>()(v.w);
  }
};

// Hash function for vector of Vertex
struct VectorHash {
  size_t operator()(const std::vector<Vertex> &vec) const {
    size_t hash = 0;
    for (const auto &v : vec) {
      hash ^= VertexHash()(v);
    }
    return hash;
  }
};

void centerRuledLabel(NVGcontext *vg, float x0, float y0, float w,
                      const char *label, int size = 14, float fill = 0.5) {
  nvgBeginPath(vg);
  // nvgFontFaceId(vg, fontId(vg));
  nvgFontSize(vg, size);
  nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
  nvgFillColor(vg, nvgRGBf(fill, fill, fill));
  nvgText(vg, x0 + w / 2, y0, label, NULL);

  float bounds[4];
  nvgTextBounds(vg, x0 + w / 2, y0, label, NULL, bounds);
};

class AmmannBeeknerTileUI {
public:
  AmmannBeeknerTileUI()
      : minExtent(
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max()), // Initialize to max float value
        maxExtent(std::numeric_limits<float>::lowest(),
                  std::numeric_limits<float>::lowest()), // Initialize to lowest
                                                         // float value
        padding(mm2px(0.5)){};
  std::vector<Polygon> polygons;

  std::set<Vertex> coordinatesOn;
  TIP tipOn;
  std::mutex coordinatesMutex;
  std::unordered_map<std::vector<Vertex>, size_t, VectorHash> polygonIDs;

  std::pair<float, float> minExtent;
  std::pair<float, float> maxExtent;
  float padding;
  // royal blue  to pink
  RGBLinearInterpolator eucInterp =
      RGBLinearInterpolator(127.0 / 255, 216.0 / 255, 230.0 / 255, 255.0 / 255,
                            192.0 / 255, 203.0 / 255);

  void loadJSON(const std::string &filename)

  {
    std::ifstream file(filename);

    if (!file.is_open()) {
      DEBUG("Unable to open JSON file: %s", filename.c_str());
      return;
    }

    try {
      json j;
      file >> j;
      for (auto &item : j) {
        Polygon poly;
        for (auto &fv : item["faceVerts"]) {
          poly.faceVerts.emplace_back(fv[0], fv[1]);

          for (int i = 0; i < 2; i++) {
            if (i == 0) // Accessing the first element
            {
              if (fv[i] < minExtent.first) {
                minExtent.first = fv[i];
              }
              if (fv[i] > maxExtent.first) {
                maxExtent.first = fv[i];
              }
            } else // Accessing the second element
            {
              if (fv[i] < minExtent.second) {
                minExtent.second = fv[i];
              }
              if (fv[i] > maxExtent.second) {
                maxExtent.second = fv[i];
              }
            }
          }
        }
        for (auto &vc : item["vertexCoords"]) {
          Vertex vertex{vc[0], vc[1], vc[2], vc[3]};
          poly.vertexCoords.push_back(vertex);
        }
        polygons.push_back(poly);
      }
    } catch (const json::parse_error &e) {
      DEBUG("JSON parsing error: %s", e.what());
    } catch (const std::exception &e) {
      DEBUG("General error: %s", e.what());
    }
    std::sort(polygons.begin(), polygons.end(),
              [](const Polygon &a, const Polygon &b) {
                if (a.faceVerts.size() != b.faceVerts.size())
                  return a.faceVerts.size() < b.faceVerts.size();
                for (size_t i = 0; i < a.faceVerts.size(); ++i) {
                  if (a.faceVerts[i] != b.faceVerts[i])
                    return a.faceVerts[i] < b.faceVerts[i];
                }
                return false; // They are equal
              });
    auto last = std::unique(polygons.begin(), polygons.end(),
                            [](const Polygon &a, const Polygon &b) {
                              return a.faceVerts == b.faceVerts;
                            });
    polygons.erase(last, polygons.end());
  }
  size_t getPolygonsCount() const { return polygons.size(); }

  void draw(Vec widgetSize, const std::map<Vertex, std::string> &labelMap,
            std::map<std::vector<Vertex>, TIP> &tips, NVGcontext *vg)

  {
    // Check if NanoVG context is valid
    if (!vg) {
      std::cerr << "NanoVG context is not initialized." << std::endl;
      return;
    }
    nvgSave(vg);

    // Determine x, y scaling for the drawing
    float xScale =
        (widgetSize.x - padding) / (maxExtent.first - minExtent.first);
    float yScale =
        (widgetSize.y - padding) / (maxExtent.second - minExtent.second);

    float xOffset = widgetSize.x / 2;
    float yOffset = widgetSize.y / 2;
    std::map<Polygon, NVGcolor> fillColors;
    {
      std::lock_guard<std::mutex> guard(coordinatesMutex);
      for (const auto &poly : polygons) {

        bool isClicked = false;
        NVGcolor polyFillColor = nvgRGBf(1.0f, 1.0f, 1.0f);

        // Check if the polygon is clicked
        if (!poly.vertexCoords.empty() && !coordinatesOn.empty()) {
          isClicked = std::all_of(
              poly.vertexCoords.begin(), poly.vertexCoords.end(),
              [this](const Vertex &v) {
                return std::find(coordinatesOn.begin(), coordinatesOn.end(),
                                 v) != coordinatesOn.end();
              });
        }
        if (!coordinatesOn.empty()) {
          const TIP thisTip = tips[poly.vertexCoords];
          const float euclideanD = thisTip.euclideanDistanceTo(tipOn);
          // These logs indicate that there are various values of euclideanD in
          // the loop.
          // DEBUG("Euclidean distance: %f", euclideanD);
          RGBLinearInterpolator bginterp =
              RGBLinearInterpolator(0.9f, 1.0f, 0.9f, 0.0f, 0.0f, 0.9f);
          const std::tuple<float, float, float> rgbf =
              bginterp.interpolate(euclideanD);
          polyFillColor =
              nvgRGBf(std::get<0>(rgbf), std::get<1>(rgbf), std::get<2>(rgbf));
          // NVGcolor nearlyWhiteGreen = nvgRGBf(0.9f, 1.0f, 0.9f);
          // NVGcolor deepBlue = nvgRGBf(0.0f, 0.0f, 0.9f);
          // polyFillColor = nvgLerpRGBA(deepBlue, nearlyWhiteGreen,
          // euclideanD);
        } else {
          polyFillColor = nvgRGBA(255, 255, 255, 255);
        }
        const NVGcolor isClickedFillColor =
            isClicked ? nvgRGBA(200, 200, 200, 255)
                      : polyFillColor; // Grey if clicked, distance based chroma
                                       // otherwise}
        fillColors[poly] = isClickedFillColor;
      }
    }

    std::set<Vertex> seenVertices;
    // Iterate over each polygon
    for (const auto &poly : polygons) {
      const NVGcolor fillColor = fillColors[poly];
      nvgSave(vg);

      // Start drawing a path
      nvgBeginPath(vg);
      nvgLineJoin(vg, NVG_MITER);

      bool firstVertex = true;

      // Draw each vertex of the polygon
      float x, y;
      for (const auto &vert : poly.faceVerts) {

        x = xOffset + padding + vert.first * xScale;
        y = yOffset + padding + vert.second * yScale;

        if (firstVertex) {
          nvgMoveTo(vg, x, y);
          firstVertex = false;
        } else {
          nvgLineTo(vg, x, y);
        }
      }

      // Close the path and fill
      // These logs indicate there are more than one color being accessed
      // accross the loop by fillColors[poly]
      // DEBUG("filling color: %s", nvcolor_to_string(fillColor).c_str());

      nvgFillColor(vg, fillColor);
      nvgClosePath(vg);
      nvgFill(vg);

      nvgRestore(vg);
      nvgSave(vg);
      // Start drawing a path
      nvgBeginPath(vg);

      nvgStrokeColor(vg, nvgRGBf(0.88, 0.88, 0.88));
      nvgStrokeWidth(vg, 1.0);
      nvgLineJoin(vg, NVG_MITER);

      firstVertex = true;

      for (const auto &vert : poly.faceVerts) {
        x = xOffset + padding + vert.first * xScale;
        y = yOffset + padding + vert.second * yScale;

        if (firstVertex) {
          nvgMoveTo(vg, x, y);
          firstVertex = false;
        } else {
          nvgLineTo(vg, x, y);
        }
      }
      nvgClosePath(vg);
      nvgStroke(vg);

      nvgRestore(vg);
    }
    // draw labels for each vertex
    float r = 7;

    for (const auto &poly : polygons) {
      const NVGcolor fillColor = fillColors[poly];
      for (size_t i = 0; i < poly.vertexCoords.size(); i++) {

        const Vertex &vertex = poly.vertexCoords[i];
        const auto &face = poly.faceVerts[i];

        auto it = labelMap.find(vertex);
        if (it != labelMap.end() &&
            seenVertices.find(vertex) == seenVertices.end()) {
          float x = xOffset + padding + face.first * xScale;
          float y = yOffset + padding + face.second * yScale;

          // Draw a small white circle with grey stroke
          nvgBeginPath(vg);
          nvgStrokeColor(vg, nvgRGBf(0.88, 0.88, 0.88));
          nvgStrokeWidth(vg, 1.0);
          nvgCircle(vg, x, y, r);
          nvgFillColor(vg, nvgRGBA(255, 255, 255, 255)); // White fill
          nvgClosePath(vg);
          nvgFill(vg);
          nvgStroke(vg);
          centerRuledLabel(vg, x - (r / 2), y, r, it->second.c_str(), 11);
          seenVertices.insert(vertex);
          // Calculate the center of the polygon to place the label
          float labelX = 0.0f;
          float labelY = 0.0f;
          float r = 10;
          int vertexCount = 0;
          for (const auto &vert : poly.faceVerts) {
            labelX += xOffset + padding + vert.first * xScale;
            labelY += yOffset + padding + vert.second * yScale;
            vertexCount++;
          }
          labelX /= vertexCount;
          labelY /= vertexCount;

          // Set the label with the color string
          // std::string colorLabel = nvcolor_to_string(fillColor);
          // centerRuledLabel(vg, labelX - (r / 2), labelY, r,
          //                  colorLabel.c_str(), 11);
          nvgBeginPath(vg);
          float hw = 16;
          nvgRect(vg, labelX - (0.5 * hw), labelY - (0.5 * hw), hw, hw);
          nvgFillColor(vg, fillColor); // This rectangle will be filled with the
                                       // correct color.
          nvgFill(vg);
        }
      }
    }
    nvgRestore(vg);
  }

  // Method to add polygons' coordinates to coordinatesOn if intersected by
  // point
  void checkAndAddPolygon(HeptatonicScale scale, VertexZZ_7 intervals,
                          Vec widgetSize, float x, float y) {

    float xScale =
        (widgetSize.x - padding) / (maxExtent.first - minExtent.first);
    float yScale =
        (widgetSize.y - padding) / (maxExtent.second - minExtent.second);
    float xOffset = widgetSize.x / 2;
    float yOffset = widgetSize.y / 2;

    std::pair<float, float> point = {(x - padding - xOffset) / xScale,
                                     (y - padding - yOffset) / yScale};

    for (const auto &poly : polygons) {

      if (isInsidePolygon(point, poly.faceVerts)) {

        std::set<Vertex> coordinatesOn_update;
        for (const auto &vertex : poly.vertexCoords) {
          coordinatesOn_update.insert(vertex);
        }
        // TODO: need mutex guard here
        if (coordinatesOn != coordinatesOn_update) {

          std::set<ZZ_12> pitches;
          for (const auto &vertex : poly.vertexCoords) {
            int pitch = computePitch(scale, intervals, vertex);
            pitches.insert(ZZ_12(abs(pitch)));
          }
          std::vector<ZZ_12> pitchesVector(pitches.begin(), pitches.end());
          TIP tipOn_ = tk6(pitchesVector);
          std::lock_guard<std::mutex> guard(coordinatesMutex);
          tipOn = tipOn_;
          coordinatesOn = coordinatesOn_update;
        }
      }
    }
  }

private:
  bool isInsidePolygon(const std::pair<float, float> &point,
                       const std::vector<std::pair<float, float>> &polygon) {
    if (polygon.size() < 3)
      return false; // Not a polygon

    // Implement ray casting algorithm
    int count = 0;
    int n = polygon.size();
    for (int i = 0; i < n; i++) {
      int j = (i + 1) % n;
      if (doIntersect(point, polygon[i], polygon[j]))
        count++;
    }

    // Point is inside if count is odd
    return count % 2 == 1;
  }

  // Check if a ray from point to right intersects with line segment
  bool doIntersect(const std::pair<float, float> &point,
                   const std::pair<float, float> &lineStart,
                   const std::pair<float, float> &lineEnd) {
    // Horizontal ray
    if (point.second == lineStart.second || point.second == lineEnd.second) {
      return true;
    }

    // Check if point is within y-coordinates of the line segment
    if (point.second < std::min(lineStart.second, lineEnd.second) ||
        point.second > std::max(lineStart.second, lineEnd.second)) {
      return false;
    }

    // Calculate x-coordinate where the ray intersects with the line segment
    float x = lineStart.first + (point.second - lineStart.second) *
                                    (lineEnd.first - lineStart.first) /
                                    (lineEnd.second - lineStart.second);

    // Check if this x-coordinate is to the right of the point
    return x > point.first;
  }
};

using namespace rack;

struct DragState {
  math::Vec position, delta;
  void init(math::Vec pos) {
    position = pos;
    delta = math::Vec({0, 0});
  }
  void updateDragState(const math::Vec &pos) { delta = delta.plus(pos); }
  math::Vec current() { return position.plus(delta); }
};

std::string pitchLabel(int pitch, int baseOctave) {
  int octave = baseOctave + (pitch / 12);
  Chromatic chromatic = toChromatic(ZZ_12(pitch));
  std::string label = toString(chromatic);
  std::string octaveStr = std::to_string(octave);
  return (label + octaveStr);
}

// return the number of semitones ot offset in in vcv rack
int getBaseOctaveOffset(int baseOctave) {
  int offset = baseOctave - 3;
  return offset * 12;
}

enum Articulation { PORTO, STACCATO };

struct TileUIWidget : OpaqueWidget {
  AmmannBeeknerTileUI tileUI;
  VertexZZ_7 intervals;
  int baseOctave;
  ZZ_7 mode;
  DragState dragState;

  std::map<Vertex, std::string> labelMap;
  std::map<std::vector<Vertex>, TIP> tips;
  TileUIWidget() {
    // Load JSON data into tileUI
    std::string jsonPath =
        asset::plugin(pluginInstance, "res/ammbeen3_data.json");
    tileUI.loadJSON(jsonPath);
    intervals = VertexZZ_7({0, 2, 4, 6});
    mode = ZZ_7(0);
    baseOctave = 4;

    dragState = DragState({0, 0});
    computeLableMap();
  }
  void computeLableMap() {
    labelMap.clear();
    tips.clear();
    HeptatonicScale scale = getScaleByMode(mode);
    for (const Polygon &poly : tileUI.polygons) {
      std::set<ZZ_12> pitches;
      for (const Vertex &vertex : poly.vertexCoords) {
        if (labelMap.find(vertex) == labelMap.end()) {

          int pitch = computePitch(scale, intervals, vertex);
          labelMap[vertex] = (pitchLabel(pitch, baseOctave));
          pitches.insert(ZZ_12(abs(pitch)));
        }
      }
      std::vector<ZZ_12> pitchVector(pitches.begin(), pitches.end());
      TIP tip = tk6(pitchVector);
      tips[poly.vertexCoords] = tip;
    }
  }

  void setBaseOctave(const int base) {
    baseOctave = base;
    computeLableMap();
  }
  void setIntervals(const VertexZZ_7 ivals) {
    intervals = ivals;
    computeLableMap();
  }
  void setMode(const ZZ_7 m) {
    mode = m;
    computeLableMap();
  }
  void draw(const DrawArgs &args) override {

    Vec widgetSize = {box.size.x, box.size.y};
    tileUI.draw(widgetSize, labelMap, tips, args.vg);
  }
  void onButton(const event::Button &e) override {
    if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
      if (e.action == GLFW_PRESS) {
        math::Vec pos = e.pos;
        dragState.init(pos);

        Vec widgetSize = {box.size.x, box.size.y};
        tileUI.checkAndAddPolygon(getScaleByMode(mode), intervals, widgetSize,
                                  pos.x, pos.y);
      }
      if (e.action == GLFW_RELEASE) {
        std::lock_guard<std::mutex> guard(tileUI.coordinatesMutex);
        tileUI.coordinatesOn.clear();
      }
    }
    OpaqueWidget::onButton(e);
  }

  void onDragMove(const event::DragMove &e) override {
    dragState.updateDragState(e.mouseDelta);
    Vec pos = dragState.current();
    Vec widgetSize = {box.size.x, box.size.y};
    tileUI.checkAndAddPolygon(getScaleByMode(mode), intervals, widgetSize,
                              pos.x, pos.y);
  }
  void onDragEnd(const event::DragEnd &e) override { dragState.init({0, 0}); }
};

struct AmB_Tonnetz : Module {
  TileUIWidget *tileUIWidget;
  int maxChannels;
  Articulation articulation;
  std::vector<int> midiNotes;

  enum ParamId { PARAMS_LEN };
  enum InputId { INPUTS_LEN };
  enum OutputId {
    OUTPUT_OUTPUT,
    OUTPUT_GATE,
    OUTPUT_DRAG_X,
    OUTPUT_DRAG_Y,
    OUTPUTS_LEN
  };
  enum LightId { LIGHTS_LEN };

  AmB_Tonnetz() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configOutput(OUTPUT_GATE, "GATE");
    configOutput(OUTPUT_DRAG_X, "Mouse X");
    configOutput(OUTPUT_DRAG_Y, "Mouse Y");
    configOutput(OUTPUT_OUTPUT, "CV");
    maxChannels = 4; // Max polyphony, adjust as needed
    articulation = PORTO;
  }
  void setTileUIWidget(TileUIWidget *w) { tileUIWidget = w; }

  void setArticulation(Articulation arto) { articulation = arto; }

  std::vector<int> generateMidiNote() {
    std::vector<int> currentMidiNotes;
    if (tileUIWidget) {
      HeptatonicScale scale = getScaleByMode(tileUIWidget->mode);

      std::lock_guard<std::mutex> guard(tileUIWidget->tileUI.coordinatesMutex);
      if (!tileUIWidget->tileUI.coordinatesOn.empty()) {

        std::set<Vertex> localCoords = tileUIWidget->tileUI.coordinatesOn;
        for (auto &coord : localCoords) {

          currentMidiNotes.push_back(
              computePitch(scale, tileUIWidget->intervals, coord));
        }
      };
      if (tileUIWidget->tileUI.coordinatesOn.empty()) {
        currentMidiNotes.clear();
      }
    }
    return currentMidiNotes;
  }

  void process(const ProcessArgs &args) override {
    const float epsilon = 1e-6;

    if (tileUIWidget) {

      bool needsRetrigger = false;
      std::vector<int> newMidiNotes =
          generateMidiNote(); // Vector of MIDI notes
      if (articulation == STACCATO && newMidiNotes != midiNotes) {
        needsRetrigger = true;
      }
      midiNotes = newMidiNotes;

      size_t notes_on = midiNotes.size();
      outputs[OUTPUT_OUTPUT].setChannels(notes_on);
      outputs[OUTPUT_GATE].setChannels(notes_on);
      for (size_t i = 0; i < notes_on; ++i) {

        std::string noteString =
            pitchLabel(midiNotes[i], tileUIWidget->baseOctave);
        float cv =
            (midiNotes[i] + getBaseOctaveOffset(tileUIWidget->baseOctave)) /
            12.0f;
        float ccv = outputs[OUTPUT_OUTPUT].getVoltage(i);

        if (std::abs(cv - ccv) > epsilon) {
          outputs[OUTPUT_OUTPUT].setVoltage(cv, i); // Set CV for each channel}
        }
      }

      // Set remaining channels to 0V if there are fewer notes than maxChannels
      for (size_t i = notes_on; i < maxChannels; ++i) {
        outputs[OUTPUT_OUTPUT].setVoltage(0, i);
      }
      if (notes_on > 0 && needsRetrigger == false) {
        for (size_t i = 0; i < notes_on && i < maxChannels; ++i) {
          float gv = outputs[OUTPUT_GATE].getVoltage(i);
          if (std::abs(gv - 10) > epsilon) {
            outputs[OUTPUT_GATE].setVoltage(10, i);
          }
        }
      } else {
        for (size_t i = 0; i < maxChannels; ++i) {
          outputs[OUTPUT_GATE].setVoltage(0, i);
        }
      }
      outputs[OUTPUT_DRAG_X].setVoltage(tileUIWidget->dragState.delta.x);
      outputs[OUTPUT_DRAG_Y].setVoltage(tileUIWidget->dragState.delta.y);
    }
  }
};

struct ChordNameUIWidget : OpaqueWidget {
  TileUIWidget *tileUIWidget;
  void setTileUIWidget(TileUIWidget *newTileUIWidget) {
    tileUIWidget = newTileUIWidget;
  }
  std::vector<int> generateMidiNote() {
    HeptatonicScale scale = getScaleByMode(tileUIWidget->mode);
    std::vector<int> midiNotes;
    std::lock_guard<std::mutex> guard(tileUIWidget->tileUI.coordinatesMutex);
    if (tileUIWidget && !tileUIWidget->tileUI.coordinatesOn.empty()) {
      for (auto &coord : tileUIWidget->tileUI.coordinatesOn) {
        midiNotes.push_back(
            computePitch(scale, tileUIWidget->intervals, coord));
      }
    };
    if (tileUIWidget && tileUIWidget->tileUI.coordinatesOn.empty()) {
      midiNotes.clear();
    }
    return midiNotes;
  }
  void draw(const DrawArgs &args) override {
    std::vector<int> midiNotes = generateMidiNote(); // Vector of MIDI notes
    if (!midiNotes.empty()) {
      int root = *std::min_element(midiNotes.begin(), midiNotes.end());
      midiNotes.erase(std::remove(midiNotes.begin(), midiNotes.end(), root),
                      midiNotes.end());
      Chromatic root_prime = toChromatic(ZZ_12(root));
      std::vector<Chromatic> chromaticNotes;
      for (int note : midiNotes) {
        chromaticNotes.push_back(toChromatic(ZZ_12(note)));
      }
      try {
        std::vector<std::string> chordNames;
        chordNames = getChordNames(
            root_prime,
            std::set<Chromatic>(chromaticNotes.begin(), chromaticNotes.end()));
        std::string chordName = "Unnamed";
        if (!chordNames.empty()) {
          auto shortest =
              std::min_element(chordNames.begin(), chordNames.end(),
                               [](const std::string &a, const std::string &b) {
                                 return a.size() < b.size();
                               });
          chordName = toString(root_prime) + " " + *shortest;
        }

        centerRuledLabel(args.vg, 0, 0, 12, chordName.c_str(), 24);
      } catch (const std::exception &e) {
        std::string errorMessage = e.what();
        centerRuledLabel(args.vg, 0, 0, 12, errorMessage.c_str(), 12);
      }
    }
  }
};

struct AmB_TonnetzWidget : ModuleWidget {
  TileUIWidget *tileUIWidget;
  ChordNameUIWidget *chordNameWidget;
  AmB_Tonnetz *tmodule;
  AmB_TonnetzWidget(AmB_Tonnetz *module)

  {

    setModule(module);

    setPanel(createPanel(asset::plugin(pluginInstance, "res/AmB-Tonnetz.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(
        createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(
        Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(Vec(
        box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addOutput(createOutputCentered<PJ301MPort>(Vec(694.804, 235.201), module,
                                               AmB_Tonnetz::OUTPUT_DRAG_X));
    addOutput(createOutputCentered<PJ301MPort>(Vec(544.804, 235.201), module,
                                               AmB_Tonnetz::OUTPUT_DRAG_Y));
    addOutput(createOutputCentered<PJ301MPort>(Vec(694.804, 335.201), module,
                                               AmB_Tonnetz::OUTPUT_GATE));
    addOutput(createOutputCentered<PJ301MPort>(Vec(544.804, 335.201), module,
                                               AmB_Tonnetz::OUTPUT_OUTPUT));

    // mm2px(Vec(460.335, 349.721))
    tileUIWidget = createWidget<TileUIWidget>(mm2px(Vec(1, 1)));
    tileUIWidget->setSize(mm2px(Vec(163, 125)));
    addChild(tileUIWidget);
    if (module) {
      module->setTileUIWidget(tileUIWidget);
      tmodule = module;
    }
    chordNameWidget = createWidget<ChordNameUIWidget>(Vec(600, 100));

    chordNameWidget->setSize(mm2px(Vec(45, 40)));
    addChild(chordNameWidget);
    chordNameWidget->setTileUIWidget(tileUIWidget);
  }
  void appendContextMenu(Menu *menu) override {

    menu->addChild(new MenuSeparator);

    menu->addChild(createMenuLabel("Tonnnetz Settings"));

    menu->addChild(createSubmenuItem("Base Octave", "", [=](Menu *menu) {
      menu->addChild(
          createMenuItem("2", "", [=]() { tileUIWidget->setBaseOctave(2); }));
      menu->addChild(
          createMenuItem("3", "", [=]() { tileUIWidget->setBaseOctave(3); }));
      menu->addChild(
          createMenuItem("4", "", [=]() { tileUIWidget->setBaseOctave(4); }));
      menu->addChild(
          createMenuItem("5", "", [=]() { tileUIWidget->setBaseOctave(5); }));
    }));

    menu->addChild(createSubmenuItem("Mode", "", [=](Menu *menu) {
      menu->addChild(
          createMenuItem("I", "", [=]() { tileUIWidget->setMode(ZZ_7(0)); }));
      menu->addChild(
          createMenuItem("II", "", [=]() { tileUIWidget->setMode(ZZ_7(1)); }));
      menu->addChild(
          createMenuItem("III", "", [=]() { tileUIWidget->setMode(ZZ_7(2)); }));
      menu->addChild(
          createMenuItem("IV", "", [=]() { tileUIWidget->setMode(ZZ_7(3)); }));
      menu->addChild(
          createMenuItem("V", "", [=]() { tileUIWidget->setMode(ZZ_7(4)); }));
      menu->addChild(
          createMenuItem("VI", "", [=]() { tileUIWidget->setMode(ZZ_7(5)); }));
      menu->addChild(
          createMenuItem("VII", "", [=]() { tileUIWidget->setMode(ZZ_7(6)); }));
    }));

    menu->addChild(createSubmenuItem("Intervals", "", [=](Menu *menu) {
      menu->addChild(createMenuItem("(0, 2, 5, 5)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({0, 2, 5, 5}));
      }));
      menu->addChild(createMenuItem("(2, 2, 4, 4)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({2, 2, 4, 4}));
      }));
      menu->addChild(createMenuItem("(1, 1, 4, 6)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({1, 1, 4, 6}));
      }));
      menu->addChild(createMenuItem("(0, 3, 3, 6)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({0, 3, 3, 6}));
      }));
      menu->addChild(createMenuItem("(2, 2, 2, 6)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({2, 2, 2, 6}));
      }));
      menu->addChild(createMenuItem("(3, 3, 3, 3)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({3, 3, 3, 3}));
      }));
      menu->addChild(createMenuItem("(2, 3, 3, 4)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({2, 3, 3, 4}));
      }));
      menu->addChild(createMenuItem("(0, 4, 4, 4)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({0, 4, 4, 4}));
      }));
      menu->addChild(createMenuItem("(0, 0, 6, 6)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({0, 0, 6, 6}));
      }));
      menu->addChild(createMenuItem("(0, 1, 5, 6)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({0, 1, 5, 6}));
      }));
      menu->addChild(createMenuItem("(1, 1, 5, 5)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({1, 1, 5, 5}));
      }));
      menu->addChild(createMenuItem("(1, 3, 4, 4)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({1, 3, 4, 4}));
      }));
      menu->addChild(createMenuItem("(2, 2, 3, 5)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({2, 2, 3, 5}));
      }));
      menu->addChild(createMenuItem("(0, 3, 4, 5)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({0, 3, 4, 5}));
      }));
      menu->addChild(createMenuItem("(1, 3, 3, 5)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({1, 3, 3, 5}));
      }));
      menu->addChild(createMenuItem("(1, 2, 3, 6)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({1, 2, 3, 6}));
      }));
      menu->addChild(createMenuItem("(1, 2, 4, 5)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({1, 2, 4, 5}));
      }));
      menu->addChild(createMenuItem("(0, 2, 4, 6)", "", [=]() {
        tileUIWidget->setIntervals(VertexZZ_7({0, 2, 4, 6}));
      }));
    }));

    if (tmodule) {
      menu->addChild(createSubmenuItem("Articulation", "", [=](Menu *menu) {
        menu->addChild(createMenuItem(
            "Portomento", "", [=]() { tmodule->setArticulation(PORTO); }));
        menu->addChild(createMenuItem(
            "Staccato", "", [=]() { tmodule->setArticulation(STACCATO); }));
      }));
    }

    // // Controls int Module::mode
    // menu->addChild(createIndexPtrSubmenuItem("Mode",
    // 										 {"Hi-fi",
    // "Mid-fi", "Lo-fi"},
    // &module->mode));
  }
};

Model *modelAmB_Tonnetz =
    createModel<AmB_Tonnetz, AmB_TonnetzWidget>("AmB-Tonnetz");