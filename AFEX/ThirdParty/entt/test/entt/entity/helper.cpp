#include <gtest/gtest.h>
#include <entt/core/hashed_string.hpp>
#include <entt/entity/helper.hpp>
#include <entt/entity/registry.hpp>

TEST(Helper, Dependency) {
    entt::DefaultRegistry registry;
    const auto entity = registry.create();
    entt::connect<double, float>(registry.construction<int>());

    ASSERT_FALSE(registry.has<double>(entity));
    ASSERT_FALSE(registry.has<float>(entity));

    registry.assign<char>(entity);

    ASSERT_FALSE(registry.has<double>(entity));
    ASSERT_FALSE(registry.has<float>(entity));

    registry.assign<int>(entity);

    ASSERT_TRUE(registry.has<double>(entity));
    ASSERT_TRUE(registry.has<float>(entity));
    ASSERT_EQ(registry.get<double>(entity), .0);
    ASSERT_EQ(registry.get<float>(entity), .0f);

    registry.get<double>(entity) = .3;
    registry.get<float>(entity) = .1f;
    registry.remove<int>(entity);
    registry.assign<int>(entity);

    ASSERT_EQ(registry.get<double>(entity), .3);
    ASSERT_EQ(registry.get<float>(entity), .1f);

    registry.remove<int>(entity);
    registry.remove<float>(entity);
    registry.assign<int>(entity);

    ASSERT_TRUE(registry.has<float>(entity));
    ASSERT_EQ(registry.get<double>(entity), .3);
    ASSERT_EQ(registry.get<float>(entity), .0f);

    registry.remove<int>(entity);
    registry.remove<double>(entity);
    registry.remove<float>(entity);
    entt::disconnect<double, float>(registry.construction<int>());
    registry.assign<int>(entity);

    ASSERT_FALSE(registry.has<double>(entity));
    ASSERT_FALSE(registry.has<float>(entity));
}

TEST(Helper, Label) {
    entt::DefaultRegistry registry;
    const auto entity = registry.create();
    registry.assign<entt::label<"foobar"_hs>>(entity);
    registry.assign<int>(entity, 42);
    int counter{};

    ASSERT_FALSE(registry.has<entt::label<"barfoo"_hs>>(entity));
    ASSERT_TRUE(registry.has<entt::label<"foobar"_hs>>(entity));

    for(auto entity: registry.view<int, entt::label<"foobar"_hs>>()) {
        (void)entity;
        ++counter;
    }

    ASSERT_NE(counter, 0);

    for(auto entity: registry.view<entt::label<"foobar"_hs>>()) {
        (void)entity;
        --counter;
    }

    ASSERT_EQ(counter, 0);
}
