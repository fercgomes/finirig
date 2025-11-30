#include <catch2/catch_test_macros.hpp>
#include "finirig/amps/AmpModel.h"

namespace finirig::amps::tests {

// Test amp implementation
class TestAmp : public AmpModel {
public:
    TestAmp() : gain_(0.5f), masterVolume_(0.7f) {}

    [[nodiscard]] float processSample(float input) noexcept override {
        return input * gain_ * masterVolume_;
    }

    void setGain(float gain) noexcept override {
        gain_ = gain;
    }

    [[nodiscard]] float getGain() const noexcept override {
        return gain_;
    }

    void setMasterVolume(float volume) noexcept override {
        masterVolume_ = volume;
    }

    [[nodiscard]] float getMasterVolume() const noexcept override {
        return masterVolume_;
    }

private:
    float gain_;
    float masterVolume_;
};

TEST_CASE("AmpModel - gain control", "[amps]") {
    TestAmp amp;

    SECTION("Can set and get gain") {
        amp.setGain(0.75f);
        REQUIRE(amp.getGain() == 0.75f);
    }

    SECTION("Gain affects processing") {
        amp.setGain(0.5f);
        float output1 = amp.processSample(1.0f);
        
        amp.setGain(1.0f);
        float output2 = amp.processSample(1.0f);
        
        REQUIRE(std::abs(output2) > std::abs(output1));
    }
}

TEST_CASE("AmpModel - master volume", "[amps]") {
    TestAmp amp;

    SECTION("Can set and get master volume") {
        amp.setMasterVolume(0.8f);
        REQUIRE(amp.getMasterVolume() == 0.8f);
    }

    SECTION("Master volume affects processing") {
        amp.setMasterVolume(0.5f);
        float output1 = amp.processSample(1.0f);
        
        amp.setMasterVolume(1.0f);
        float output2 = amp.processSample(1.0f);
        
        REQUIRE(std::abs(output2) > std::abs(output1));
    }
}

} // namespace finirig::amps::tests

