#include "../utk_vector.h"
#include <catch2/catch_all.hpp>

TEST_CASE("vector construction", "[vector]") {
    SECTION("construct with size and value") {
        utk::vector<int> v(5, 42);
        REQUIRE(v.begin() != nullptr);
    }
    
    SECTION("construct with size only") {
        utk::vector<int> v(3);
        REQUIRE(v.begin() != nullptr);
    }
}

TEST_CASE("vector copy semantics", "[vector]") {
    SECTION("copy constructor") {
        utk::vector<int> v1(3, 10);
        utk::vector<int> v2 = v1;
        REQUIRE(v2.begin() != nullptr);
    }
    
    SECTION("copy assignment") {
        utk::vector<int> v1(3, 10);
        utk::vector<int> v2(2, 5);
        v2 = v1;
        REQUIRE(v2.begin() != nullptr);
    }
}

TEST_CASE("vector move semantics", "[vector]") {
    SECTION("move constructor") {
        utk::vector<int> v1(3, 20);
        utk::vector<int> v2 = std::move(v1);
        REQUIRE(v2.begin() != nullptr);
    }
    
    SECTION("move assignment") {
        utk::vector<int> v1(3, 40);
        utk::vector<int> v2(2, 5);
        v2 = std::move(v1);
        REQUIRE(v2.begin() != nullptr);
    }
}