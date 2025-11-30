#include <catch2/catch_test_macros.hpp>
#include "finirig/audio/AudioProcessor.h"
#include <array>

namespace finirig::audio::tests {

// Test processor implementation
class TestProcessor : public AudioProcessor {
public:
    explicit TestProcessor(float multiplier = 2.0f)
        : multiplier_(multiplier) {}

    [[nodiscard]] float processSample(float input) noexcept override {
        return input * multiplier_;
    }

private:
    float multiplier_;
};

TEST_CASE("AudioProcessor - processSample", "[audio]") {
    TestProcessor processor(2.0f);

    SECTION("Processes single sample correctly") {
        REQUIRE(processor.processSample(0.5f) == 1.0f);
        REQUIRE(processor.processSample(-0.5f) == -1.0f);
        REQUIRE(processor.processSample(0.0f) == 0.0f);
    }
}

TEST_CASE("AudioProcessor - processBlock", "[audio]") {
    TestProcessor processor(2.0f);

    SECTION("Processes buffer correctly") {
        constexpr int numSamples = 4;
        constexpr int numChannels = 2;
        std::array<float, numSamples * numChannels> buffer = {
            0.1f, 0.1f,
            0.2f, 0.2f,
            0.3f, 0.3f,
            0.4f, 0.4f
        };

        processor.processBlock(buffer.data(), numChannels, numSamples);

        REQUIRE(buffer[0] == 0.2f);
        REQUIRE(buffer[1] == 0.2f);
        REQUIRE(buffer[2] == 0.4f);
        REQUIRE(buffer[3] == 0.4f);
        REQUIRE(buffer[4] == 0.6f);
        REQUIRE(buffer[5] == 0.6f);
        REQUIRE(buffer[6] == 0.8f);
        REQUIRE(buffer[7] == 0.8f);
    }
}

TEST_CASE("AudioProcessor - prepare and reset", "[audio]") {
    TestProcessor processor;

    SECTION("Can be prepared with sample rate") {
        REQUIRE_NOTHROW(processor.prepare(44100.0));
        REQUIRE_NOTHROW(processor.prepare(48000.0));
    }

    SECTION("Can be reset") {
        REQUIRE_NOTHROW(processor.reset());
    }
}

} // namespace finirig::audio::tests

