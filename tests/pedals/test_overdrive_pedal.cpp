#include <catch2/catch_test_macros.hpp>
#include "finirig/pedals/OverdrivePedal.h"
#include <cmath>

namespace finirig::pedals::tests {

TEST_CASE("OverdrivePedal - parameter setting", "[pedals]") {
    OverdrivePedal pedal;

    SECTION("Drive parameter") {
        REQUIRE(pedal.getDrive() == 0.5f);
        
        pedal.setDrive(0.75f);
        REQUIRE(pedal.getDrive() == 0.75f);
        
        pedal.setDrive(1.5f); // Should clamp to 1.0
        REQUIRE(pedal.getDrive() == 1.0f);
        
        pedal.setDrive(-0.5f); // Should clamp to 0.0
        REQUIRE(pedal.getDrive() == 0.0f);
    }

    SECTION("Tone parameter") {
        REQUIRE(pedal.getTone() == 0.5f);
        
        pedal.setTone(0.25f);
        REQUIRE(pedal.getTone() == 0.25f);
        
        pedal.setTone(1.5f); // Should clamp
        REQUIRE(pedal.getTone() == 1.0f);
    }

    SECTION("Level parameter") {
        REQUIRE(pedal.getLevel() == 0.7f);
        
        pedal.setLevel(0.5f);
        REQUIRE(pedal.getLevel() == 0.5f);
    }
}

TEST_CASE("OverdrivePedal - processing", "[pedals]") {
    OverdrivePedal pedal;
    pedal.prepare(44100.0);

    SECTION("Processes audio samples") {
        float input = 0.1f;
        float output = pedal.processSample(input);
        
        // Output should be different from input (processed)
        REQUIRE(output != input);
        // Output should be in valid range
        REQUIRE(std::abs(output) <= 1.0f);
    }

    SECTION("Respects level control") {
        pedal.setLevel(0.5f);
        float output1 = pedal.processSample(0.5f);
        
        pedal.setLevel(1.0f);
        float output2 = pedal.processSample(0.5f);
        
        // Higher level should produce higher output (or equal)
        REQUIRE(std::abs(output2) >= std::abs(output1));
    }

    SECTION("Respects enabled state") {
        float input = 0.5f;
        float outputEnabled = pedal.processSample(input);
        
        pedal.setEnabled(false);
        float outputDisabled = pedal.processSample(input);
        
        REQUIRE(outputDisabled == input);
        REQUIRE(outputEnabled != input);
    }
}

TEST_CASE("OverdrivePedal - prepare and reset", "[pedals]") {
    OverdrivePedal pedal;

    SECTION("Can prepare with different sample rates") {
        REQUIRE_NOTHROW(pedal.prepare(44100.0));
        REQUIRE_NOTHROW(pedal.prepare(48000.0));
        REQUIRE_NOTHROW(pedal.prepare(96000.0));
    }

    SECTION("Can reset state") {
        pedal.prepare(44100.0);
        REQUIRE_NOTHROW(pedal.reset());
    }
}

} // namespace finirig::pedals::tests

