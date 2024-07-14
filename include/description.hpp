#pragma once

#include "json.hpp"
#include <memory>

namespace onto
{
  class description
  {
  public:
    virtual ~description() = default;

  private:
    virtual json::json to_json() const = 0;
    friend json::json to_json(const description &desc);
  };

  inline json::json to_json(const description &desc) { return desc.to_json(); }

  class class_description final : public description
  {
  public:
    class_description(const std::string &id) : id(id) {}

    const std::string &get_id() const { return id; }

  private:
    json::json to_json() const override { return json::json{{"type", "class"}, {"id", id}}; }

  private:
    std::string id;
  };

  class union_description final : public description
  {
  public:
    union_description(std::vector<std::unique_ptr<description>> &&descriptions) : descriptions(std::move(descriptions)) {}

  private:
    json::json to_json() const override
    {
      json::json j_union(json::json_type::array);
      for (const auto &desc : descriptions)
        j_union.push_back(onto::to_json(*desc));
      return json::json{{"type", "unionOf"}, {"descriptions", std::move(j_union)}};
    }

  private:
    std::vector<std::unique_ptr<description>> descriptions;
  };

  class intersection_description final : public description
  {
  public:
    intersection_description(std::vector<std::unique_ptr<description>> &&descriptions) : descriptions(std::move(descriptions)) {}

  private:
    json::json to_json() const override
    {
      json::json j_intersection(json::json_type::array);
      for (const auto &desc : descriptions)
        j_intersection.push_back(onto::to_json(*desc));
      return json::json{{"type", "intersectionOf"}, {"descriptions", std::move(j_intersection)}};
    }

  private:
    std::vector<std::unique_ptr<description>> descriptions;
  };

  class complement_description final : public description
  {
  public:
    complement_description(std::unique_ptr<description> &&description) : description(std::move(description)) {}

  private:
    json::json to_json() const override { return json::json{{"type", "complementOf"}, {"description", onto::to_json(*description)}}; }

  private:
    std::unique_ptr<description> description;
  };

  class one_of_description final : public description
  {
  public:
    one_of_description(std::vector<std::string> &&ids) : ids(std::move(ids)) {}

  private:
    json::json to_json() const override
    {
      json::json j_ids(json::json_type::array);
      for (const auto &id : ids)
        j_ids.push_back(id);
      return json::json{{"type", "oneOf"}, {"ids", std::move(j_ids)}};
    }

  private:
    std::vector<std::string> ids;
  };

  class restriction : public description
  {
  public:
    virtual ~restriction() = default;
  };

  class all_values_from final : public restriction
  {
  public:
    all_values_from(const std::string &property, std::unique_ptr<description> &&description) : property(property), description(std::move(description)) {}

  private:
    json::json to_json() const override { return json::json{{"type", "allValuesFrom"}, {"property", property}, {"description", onto::to_json(*description)}}; }

  private:
    std::string property;
    std::unique_ptr<description> description;
  };

  class some_values_from final : public restriction
  {
  public:
    some_values_from(const std::string &property, std::unique_ptr<description> &&description) : property(property), description(std::move(description)) {}

  private:
    json::json to_json() const override { return json::json{{"type", "someValuesFrom"}, {"property", property}, {"description", onto::to_json(*description)}}; }

  private:
    std::string property;
    std::unique_ptr<description> description;
  };

  class has_value final : public restriction
  {
  public:
    has_value(const std::string &property, const std::string &value) : property(property), value(value) {}

  private:
    json::json to_json() const override { return json::json{{"type", "hasValue"}, {"property", property}, {"value", value}}; }

  private:
    std::string property;
    std::string value;
  };

  class min_cardinality final : public restriction
  {
  public:
    min_cardinality(const std::string &property, INT_TYPE cardinality) : property(property), cardinality(cardinality) {}

  private:
    json::json to_json() const override { return json::json{{"type", "minCardinality"}, {"property", property}, {"cardinality", cardinality}}; }

  private:
    std::string property;
    INT_TYPE cardinality;
  };

  class max_cardinality final : public restriction
  {
  public:
    max_cardinality(const std::string &property, INT_TYPE cardinality) : property(property), cardinality(cardinality) {}

  private:
    json::json to_json() const override { return json::json{{"type", "maxCardinality"}, {"property", property}, {"cardinality", cardinality}}; }

  private:
    std::string property;
    INT_TYPE cardinality;
  };

  class exact_cardinality final : public restriction
  {
  public:
    exact_cardinality(const std::string &property, INT_TYPE cardinality) : property(property), cardinality(cardinality) {}

  private:
    json::json to_json() const override { return json::json{{"type", "exactCardinality"}, {"property", property}, {"cardinality", cardinality}}; }

  private:
    std::string property;
    INT_TYPE cardinality;
  };
} // namespace onto
