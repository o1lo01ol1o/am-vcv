#include "BaconModule.hpp"
#include "BaconModuleWidget.h"
#include "plugin.hpp"

#define SCREW_WIDTH 15
#define RACK_HEIGHT 380
namespace bp = baconpaul::rackplugs;

struct BaconTest : bp::BaconModule {
  static constexpr int nParams{8}, nPorts{4};
  enum ParamIds { PARAM_0, NUM_PARAMS = PARAM_0 + nParams };

  enum InputIds { INPUT_0, NUM_INPUTS = INPUT_0 + nPorts };

  enum OutputIds { OUTPUT_0, NUM_OUTPUTS = OUTPUT_0 + nPorts };

  enum LightIds { NUM_LIGHTS };

  BaconTest() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

    for (int i = 0; i < nParams; ++i)
      configParam(PARAM_0 + i, 0, 1, 0, "Param " + std::to_string(i));
    for (int i = 0; i < nPorts; ++i) {
      configInput(INPUT_0 + i, "Input " + std::to_string(i));
      configInput(OUTPUT_0 + i, "Output " + std::to_string(i));
    }
  }

  void process(const ProcessArgs &args) override {}
};

struct BaconTestWidget : bp::BaconModuleWidget {
  BaconTestWidget(BaconTest *model);
};

struct PolyWidget : public rack::Widget {
  int64_t dc{0};
  void draw(const DrawArgs &args) override {
    auto s = box.size;

    auto vg = args.vg;

    auto dcm = dc % (int)(s.x - 40);

    nvgBeginPath(vg);
    nvgFillColor(vg, nvgRGB(255, 0, 0));
    nvgRect(vg, dcm, dcm, 40, 40);
    nvgFill(vg);

    typedef std::vector<std::pair<float, float>> poly_t;
    std::vector<poly_t> polys;

    std::map<poly_t, NVGcolor> colm;
    for (int nsides = 3; nsides < 11; ++nsides) {
      poly_t p;
      for (auto i = 0; i < nsides; ++i) {
        auto x = std::sin(-i * 2.0 * M_PI / nsides) + 1;
        auto y = std::cos(i * 2.0 * M_PI / nsides) + 1;
        p.emplace_back(x, y);
      }
      polys.push_back(p);
      auto idx = nsides - 3;
      colm[p] = nvgRGB((255 - dcm * 20) * (idx < 5), idx * 15, dcm * 20);
    }

    int idx = 0;
    for (auto &poly : polys) {
      auto first{true};
      nvgBeginPath(vg);
      for (const auto &[x, y] : poly) {
        if (first) {
          nvgMoveTo(vg, 15 * x + dcm, 15 * y + dcm + 40 + idx * 18);
        } else {
          nvgLineTo(vg, 15 * x + dcm, 15 * y + dcm + 40 + idx * 18);
        }
        first = false;
      }
      nvgClosePath(vg);
      // nvgFillColor(vg, nvgRGB((255 - dcm * 20) * (idx < 5), idx * 15, dcm *
      // 20));
      nvgFillColor(vg, colm[poly]);
      nvgFill(vg);
      nvgStrokeColor(vg, nvgRGB(0, 0, 50));
      nvgStroke(vg);
      idx++;
    }

    dc++;
  }
};

BaconTestWidget::BaconTestWidget(BaconTest *model) {
  setModule(model);
  box.size = Vec(SCREW_WIDTH * 15, RACK_HEIGHT);

  // addChild(bg->wrappedInFramebuffer());

  auto layoutSize = box.size;
  layoutSize.x /= 2;

  Vec cr(5, 35);
  auto dSp = (layoutSize.x - 10) / BaconTest::nPorts;
  for (int i = 0; i < BaconTest::nPorts; ++i) {
    addInput(createInput<PJ301MPort>(cr, module, BaconTest::INPUT_0 + i));
    cr.x += dSp;
  }

  cr = Vec(10, 90);
  auto kSp = (layoutSize.x - 20) / 2;
  for (int i = 0; i < BaconTest::nParams; ++i) {
    addParam(
        createParam<RoundLargeBlackKnob>(cr, module, BaconTest::PARAM_0 + i));
    if (i % 2 == 0)
      cr.x += kSp;
    else {
      cr.x = 10;
      cr.y += kSp;
    }
  }

  cr = Vec(5, 300);
  for (int i = 0; i < BaconTest::nPorts; ++i) {
    addOutput(createOutput<PJ301MPort>(cr, module, BaconTest::OUTPUT_0 + i));
    cr.x += dSp;
  }

  auto p = new PolyWidget();
  p->box.pos = rack::Vec(box.size.x / 2, 20);
  p->box.size = rack::Vec(box.size.x / 2 - 10, box.size.y - 40);

  addChild(p);
}

Model *modelBaconTest = createModel<BaconTest, BaconTestWidget>("BaconTest");
