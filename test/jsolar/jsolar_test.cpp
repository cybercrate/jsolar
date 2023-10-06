#include <jsolar/jsolar.h>
#include <gtest/gtest.h>

#include <fstream>

TEST(wingmann_serializing_jsolar, create_new) {
    wingmann::ser::json::Jsolar obj;
    obj["array"] = wingmann::ser::json::array(true, "two", 3, 4.9213798);
    obj["object"] = wingmann::ser::json::object();
    obj["object"]["inner"] = "inside";
    obj["new"]["some"]["deep"]["key"] = "value";
    obj["another_array"].append(false, "three");
    obj["parsed"] = wingmann::ser::json::Jsolar::load(R"([{"key": "value"}, false])");

    std::ofstream file{"test_data_1.json"};
    if (file.is_open()) {
        file << obj.dump();
        file.close();
    }
    EXPECT_TRUE(true);
}

TEST(wingmann_serializing_jsolar, init_from_string) {
    auto obj = wingmann::ser::json::Jsolar::load(R"({
        "array": [true, "two", 3, 4.921380],
        "another_array": [false, "three"],
        "new": {
            "some": {
                "deep": {
                    "key": "value"
                }
            }
        },
        "object": {
            "inner": "inside"
        },
        "parsed": [{ "key": "value" }, false]
    })");

    std::ofstream file{"test_data_2.json"};
    if (file.is_open()) {
        file << obj.dump();
        file.close();
    }
    EXPECT_TRUE(true);
}

TEST(wingmann_serializing_jsolar, load_from_file) {
    std::ifstream file;
    std::string f1;
    std::string f2;

    file.open("test_data_1.json");
    if (file.is_open()) {
        f1 = std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
        file.close();
    }

    file.open("test_data_2.json");
    if (file.is_open()) {
        f2 = std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
        file.close();
    }
    EXPECT_EQ(
            wingmann::ser::json::Jsolar::load(f1).dump(),
        wingmann::ser::json::Jsolar::load(f2).dump());
}
