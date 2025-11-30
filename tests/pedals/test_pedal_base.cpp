#include <catch2/catch_test_macros.hpp>
#include "finirig/pedals/PedalBase.h"

namespace finirig::pedals::tests {

// Test pedal implementation
class TestPedal : public PedalBase {
protected:
    [[nodiscard]] float processSampleImpl(float input) noexcept override {
        return input * 2.0f;
    }
};

TEST_CASE("PedalBase - enabled state", "[pedals]") {
    TestPedal pedal;

    SECTION("Pedal is enabled by default") {
        REQUIRE(pedal.isEnabled() == true);
    }

    SECTION("Can disable pedal") {
        pedal.setEnabled(false);
        REQUIRE(pedal.isEnabled() == false);
    }

    SECTION("Can enable pedal") {
        pedal.setEnabled(false);
        pedal.setEnabled(true);
        REQUIRE(pedal.isEnabled() == true);
    }
}

TEST_CASE("PedalBase - processing", "[pedals]") {
    TestPedal pedal;

    SECTION("Processes when enabled") {
        REQUIRE(pedal.processSample(0.5f) == 1.0f);
    }

    SECTION("Passes through when disabled") {
        pedal.setEnabled(false);
        REQUIRE(pedal.processSample(0.5f) == 0.5f);
    }
}

} // namespace finirig::pedals::tests

