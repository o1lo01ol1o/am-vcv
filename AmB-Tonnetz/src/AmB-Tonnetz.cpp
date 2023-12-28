#include "../../sdk/Rack-SDK/dep/include/nanovg.h"
#include "plugin.hpp"
#include "rack.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

// Use nlohmann/json for JSON parsing
using json = nlohmann::json;

template <uint MOD> class ZZ_Mod {
private:
  uint value;

  uint mod(int input) const {
    int result = input % MOD;
    return result >= 0 ? result : result + MOD;
  }

public:
  ZZ_Mod() : value(0) {}
  explicit ZZ_Mod(uint v) : value(mod(v)) {}

  uint unMod() const { return value; }

  ZZ_Mod operator+(const ZZ_Mod &other) const {
    return ZZ_Mod(mod(value + other.value));
  }
  ZZ_Mod operator-(const ZZ_Mod &other) const {
    return ZZ_Mod(mod(value - other.value));
  }
  ZZ_Mod operator*(const ZZ_Mod &other) const {
    return ZZ_Mod(mod(value * other.value));
  }
  bool operator==(const ZZ_Mod &other) const { return value == other.value; }
  bool operator<(const ZZ_Mod &other) const { return value < other.value; }
  std::string to_string() const {
    return "{" + std::to_string(value) + " % " + std::to_string(MOD) + "}";
  }
};

using ZZ_7 = ZZ_Mod<7>;
using ZZ_12 = ZZ_Mod<12>;

// struct Vertex
// {
// 	int x, y, z, w;
// 	bool operator<(const Vertex &other) const
// 	{
// 		if (x != other.x)
// 			return x < other.x;
// 		if (y != other.y)
// 			return y < other.y;
// 		if (z != other.z)
// 			return z < other.z;
// 		return w < other.w;
// 	}
// 	bool operator==(const Vertex &other) const
// 	{
// 		return x == other.x && y == other.y && z == other.z && w ==
// other.w;
// 	}
// };

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

std::string toStringVertex(const Vertex &v) {
  return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " +
         std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
}

struct Polygon {
  std::vector<std::pair<float, float>> faceVerts; // 2D vertices
  std::vector<Vertex> vertexCoords;               // 4D coordinates
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

// // Define a simple representation for the HeptatonicScale
// // Define the HeptatonicScale struct
// struct HeptatonicScale
// {
// 	std::vector<int> notes;

// 	HeptatonicScale(std::initializer_list<int> init) : notes(init) {}

// 	HeptatonicScale succMode() const
// 	{
// 		HeptatonicScale result = *this;
// 		int first = result.notes[0];
// 		for (size_t i = 0; i < result.notes.size() - 1; ++i)
// 		{
// 			result.notes[i] = result.notes[i + 1];
// 		}
// 		result.notes[result.notes.size() - 1] = first;
// 		return result;
// 	}

// 	int operator[](int index) const
// 	{
// 		if (index < 0 || index >= static_cast<int>(notes.size()))
// 			throw std::out_of_range("Index out of range");
// 		return notes[index];
// 	}
// };

// Define the HeptatonicScale struct using ZZ_12
struct HeptatonicScale {
  std::vector<ZZ_12> notes;

  HeptatonicScale(std::initializer_list<ZZ_12> init) : notes(init) {}

  HeptatonicScale succMode() const {
    HeptatonicScale result = *this;
    ZZ_12 first = result.notes[0];
    for (size_t i = 0; i < result.notes.size() - 1; ++i) {
      result.notes[i] = result.notes[i + 1];
    }
    result.notes[result.notes.size() - 1] = first;
    return result;
  }

  ZZ_12 operator[](int index) const {
    if (index < 0 || index >= static_cast<int>(notes.size()))
      throw std::out_of_range("Index out of range");
    return notes[index];
  }
};

std::string toStringHeptatonicScale(const HeptatonicScale &scale) {
  std::string result = "{";
  for (size_t i = 0; i < scale.notes.size(); ++i) {
    result += scale.notes[i].to_string();
    if (i < scale.notes.size() - 1) {
      result += ", ";
    }
  }
  result += "}";
  return result;
}

// Define the scales as global constants
// Define constants for each mode
const HeptatonicScale cIonian = {ZZ_12(0), ZZ_12(2), ZZ_12(4), ZZ_12(5),
                                 ZZ_12(7), ZZ_12(9), ZZ_12(11)};
const HeptatonicScale cDorian = cIonian.succMode();
const HeptatonicScale cPhrygian = cDorian.succMode();
const HeptatonicScale cLydian = cPhrygian.succMode();
const HeptatonicScale cMixolydian = cLydian.succMode();
const HeptatonicScale cAeolian = cMixolydian.succMode();
const HeptatonicScale cLocrian = cAeolian.succMode();

// Function to get scale by mode using switch case
HeptatonicScale getScaleByMode(ZZ_7 mode) {
  switch (mode.unMod()) {
  case 0:
    return cIonian;
  case 1:
    return cDorian;
  case 2:
    return cPhrygian;
  case 3:
    return cLydian;
  case 4:
    return cMixolydian;
  case 5:
    return cAeolian;
  case 6:
    return cLocrian;
  default:
    throw std::out_of_range("Invalid mode");
  }
}

// Function to compute the pitch:  compute the index in the underlaying scale
// (eg, 3rd) to get the number of semitones for that interval; then multiply by
// the position in that coordinate
int computePitch(const HeptatonicScale &scale, const VertexZZ_7 &intervals,
                 const Vertex &coord) {
  return (scale[intervals.x.unMod()].unMod() * coord.x) +
         (scale[intervals.y.unMod()].unMod() * coord.y) +
         (scale[intervals.z.unMod()].unMod() * coord.z) +
         (scale[intervals.w.unMod()].unMod() * coord.w);
}

// Enum representing the 12-tone chromatic scale
enum class Chromatic { C, Cs, D, Eb, E, F, Fs, G, Gs, A, Bb, B };

// Function to convert Chromatic to string
std::string toString(Chromatic note) {
  switch (note) {
  case Chromatic::C:
    return "C";
  case Chromatic::Cs:
    return "C#";
  case Chromatic::D:
    return "D";
  case Chromatic::Eb:
    return "Eb";
  case Chromatic::E:
    return "E";
  case Chromatic::F:
    return "F";
  case Chromatic::Fs:
    return "F#";
  case Chromatic::G:
    return "G";
  case Chromatic::Gs:
    return "G#";
  case Chromatic::A:
    return "A";
  case Chromatic::Bb:
    return "Bb";
  case Chromatic::B:
    return "B";
  default:
    throw std::out_of_range("Invalid Chromatic note");
  }
}

// Function to convert uint to Chromatic (assuming 0 corresponds to C)
Chromatic toChromatic(ZZ_12 num) {
  uint n = num.unMod();
  if (n >= 12) {
    throw std::out_of_range(
        "Number must be in the range 0-11 for Chromatic conversion");
  }
  return static_cast<Chromatic>(n);
}

// Function to convert Chromatic to ZZ_12
ZZ_12 toZZ_12(Chromatic note) { return ZZ_12(static_cast<uint>(note)); }

void centerRuledLabel(NVGcontext *vg, float x0, float y0, float w,
                      const char *label, int size = 14) {
  nvgBeginPath(vg);
  // nvgFontFaceId(vg, fontId(vg));
  nvgFontSize(vg, size);
  nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
  nvgFillColor(vg, nvgRGBf(0.5, 0.5, 0.5));
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

  std::vector<Vertex> coordinatesOn;
  std::unordered_map<std::vector<Vertex>, size_t, VectorHash> polygonIDs;
  std::pair<float, float> minExtent;
  std::pair<float, float> maxExtent;
  float padding;

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
  }
  size_t getPolygonsCount() const { return polygons.size(); }

  void draw(Vec widgetSize, const std::map<Vertex, std::string> &labelMap,
            NVGcontext *vg)

  {
    // Check if NanoVG context is valid
    if (!vg) {
      std::cerr << "NanoVG context is not initialized." << std::endl;
      return;
    }

    // Determine x, y scaling for the drawing
    float xScale =
        (widgetSize.x - padding) / (maxExtent.first - minExtent.first);
    float yScale =
        (widgetSize.y - padding) / (maxExtent.second - minExtent.second);

    float xOffset = widgetSize.x / 2;
    float yOffset = widgetSize.y / 2;

    // Iterate over each polygon
    for (const auto &poly : polygons) {
      // Check if the polygon is clicked
      bool isClicked = std::all_of(
          poly.vertexCoords.begin(), poly.vertexCoords.end(),
          [this](const Vertex &v) {
            return std::find(coordinatesOn.begin(), coordinatesOn.end(), v) !=
                   coordinatesOn.end();
          });
      nvgStrokeColor(vg, nvgRGBf(0.88, 0.88, 0.88));
      nvgStrokeWidth(vg, 1.0);
      nvgLineJoin(vg, NVG_MITER);

      // Start drawing a path
      nvgBeginPath(vg);
      bool firstVertex = true;

      // Draw each vertex of the polygon
      for (const auto &vert : poly.faceVerts) {
        float x = xOffset + padding + vert.first * xScale;
        float y = yOffset + padding + vert.second * yScale;

        if (firstVertex) {
          nvgMoveTo(vg, x, y);
          firstVertex = false;
        } else {
          nvgLineTo(vg, x, y);
        }
      }

      // Close the path and fill
      nvgClosePath(vg);
      nvgStroke(vg);
      NVGcolor fillColor =
          isClicked
              ? nvgRGBA(200, 200, 200, 255)
              : nvgRGBA(255, 255, 255, 255); // Grey if clicked, white otherwise
      nvgFillColor(vg, fillColor);
      nvgFill(vg);
    }
    // Optionally, draw labels for each vertex
    float r = 7;
    std::set<Vertex> seenVertices;
    for (const auto &poly : polygons) {
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
          nvgCircle(vg, x, y, r);
          nvgFillColor(vg, nvgRGBA(255, 255, 255, 255)); // White fill
          nvgFill(vg);
          centerRuledLabel(vg, x - (r / 2), y, r, it->second.c_str(), 11);
          seenVertices.insert(vertex);
        }
      }
    }
  }

  // Method to add polygons' coordinates to coordinatesOn if intersected by
  // point
  void checkAndAddPolygon(Vec widgetSize, float x, float y) {

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

        std::vector<Vertex> coordinatesOn_update;
        for (const auto &vertex : poly.vertexCoords) {
          coordinatesOn_update.push_back(vertex);
        }
        if (coordinatesOn != coordinatesOn_update) {
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

struct TileUIWidget : OpaqueWidget {
  AmmannBeeknerTileUI tileUI;
  VertexZZ_7 intervals;
  ZZ_7 mode;
  DragState dragState;
  std::map<Vertex, std::string> labelMap;
  TileUIWidget() {
    // Load JSON data into tileUI
    std::string jsonPath =
        asset::plugin(pluginInstance, "res/ammbeen3_data.json");
    tileUI.loadJSON(jsonPath);
    intervals = VertexZZ_7({0, 2, 4, 6});
    mode = ZZ_7(0);
    dragState = DragState({0, 0});
    computeLableMap();
  }
  void computeLableMap() {
    labelMap.clear();
    HeptatonicScale scale = getScaleByMode(mode);
    for (const Polygon &poly : tileUI.polygons) {
      for (const Vertex &vertex : poly.vertexCoords) {
        if (labelMap.find(vertex) == labelMap.end()) {

          int pitch = computePitch(scale, intervals, vertex);
          int octave = 4 + pitch / 12;
          Chromatic chromatic = toChromatic(ZZ_12(pitch));
          std::string label = toString(chromatic);
          std::string octaveStr = std::to_string(octave);
          labelMap[vertex] = (label + octaveStr);
        }
      }
    }
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
    tileUI.draw(widgetSize, labelMap, args.vg);
  }
  void onButton(const event::Button &e) override {
    if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
      if (e.action == GLFW_PRESS) {
        math::Vec pos = e.pos;
        dragState.init(pos);

        Vec widgetSize = {box.size.x, box.size.y};
        tileUI.checkAndAddPolygon(widgetSize, pos.x, pos.y);
      }
      if (e.action == GLFW_RELEASE) {
        tileUI.coordinatesOn.clear();
      }
    }
    OpaqueWidget::onButton(e);
  }

  void onDragMove(const event::DragMove &e) override {
    dragState.updateDragState(e.mouseDelta);
    Vec pos = dragState.current();
    Vec widgetSize = {box.size.x, box.size.y};
    tileUI.checkAndAddPolygon(widgetSize, pos.x, pos.y);
  }
  void onDragEnd(const event::DragEnd &e) override { dragState.init({0, 0}); }
};

struct AmB_Tonnetz : Module {
  TileUIWidget *tileUIWidget;
  int maxChannels;

  enum ParamId { PARAMS_LEN };
  enum InputId { INPUTS_LEN };
  enum OutputId { OUTPUT_OUTPUT, OUTPUT_GATE, OUTPUTS_LEN };
  enum LightId { LIGHTS_LEN };

  AmB_Tonnetz() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configOutput(OUTPUT_GATE, "GATE");
    configOutput(OUTPUT_OUTPUT, "CV");
    maxChannels = 4; // Max polyphony, adjust as needed
  }
  void setTileUIWidget(TileUIWidget *w) {
    DEBUG("setTileUIWidget in AmB_Tonnetz", "");
    tileUIWidget = w;
    DEBUG("done in AmB_Tonnetz", "");
  }

  std::vector<int> generateMidiNote() {
    HeptatonicScale scale = getScaleByMode(tileUIWidget->mode);
    std::vector<int> midiNotes;
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

  void process(const ProcessArgs &args) override {

    if (tileUIWidget) {
      std::vector<int> midiNotes = generateMidiNote(); // Vector of MIDI notes
      size_t notes_on = midiNotes.size();
      outputs[OUTPUT_OUTPUT].setChannels(maxChannels);
      outputs[OUTPUT_GATE].setChannels(maxChannels);
      for (size_t i = 0; i < notes_on; ++i) {
        float cv = (midiNotes[i]) / 12.0f;
        float ccv = outputs[OUTPUT_OUTPUT].getVoltage(i);
        if (cv != ccv) {
          outputs[OUTPUT_OUTPUT].setVoltage(cv, i); // Set CV for each channel}
        }
      }

      // Set remaining channels to 0V if there are fewer notes than maxChannels
      for (size_t i = notes_on; i < maxChannels; ++i) {
        outputs[OUTPUT_OUTPUT].setVoltage(0, i);
      }
      if (notes_on > 0) {
        for (size_t i = 0; i < notes_on && i < maxChannels; ++i) {
          float gv = outputs[OUTPUT_GATE].getVoltage(i);
          if (gv != 5) {
            outputs[OUTPUT_GATE].setVoltage(5, i);
          }
        }
      } else {
        for (size_t i = 0; i < maxChannels; ++i) {
          outputs[OUTPUT_GATE].setVoltage(0, i);
        }
      }
    }
  }
};

struct AmB_TonnetzWidget : ModuleWidget {
  TileUIWidget *tileUIWidget;
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

    addOutput(createOutputCentered<PJ301MPort>(Vec(694.804, 335.201), module,
                                               AmB_Tonnetz::OUTPUT_GATE));
    addOutput(createOutputCentered<PJ301MPort>(Vec(544.804, 335.201), module,
                                               AmB_Tonnetz::OUTPUT_OUTPUT));

    // mm2px(Vec(460.335, 349.721))
    tileUIWidget = createWidget<TileUIWidget>(mm2px(Vec(6.257, 6.71)));
    tileUIWidget->setSize(mm2px(Vec(162.396, 123.374)));
    addChild(tileUIWidget);
    if (module) {
      module->setTileUIWidget(tileUIWidget);
    }
  }
  void appendContextMenu(Menu *menu) override {

    menu->addChild(new MenuSeparator);

    menu->addChild(createMenuLabel("Tonnnetz Settings"));

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

    // // Controls int Module::mode
    // menu->addChild(createIndexPtrSubmenuItem("Mode",
    // 										 {"Hi-fi",
    // "Mid-fi", "Lo-fi"},
    // &module->mode));
  }
};

Model *modelAmB_Tonnetz =
    createModel<AmB_Tonnetz, AmB_TonnetzWidget>("AmB-Tonnetz");