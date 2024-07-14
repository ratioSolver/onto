#pragma once

#include "description.hpp"

namespace onto
{
  class class_axiom
  {
  public:
    class_axiom(const std::string &id, const std::string &name, std::vector<std::unique_ptr<description>> &&descriptions) : id(id), name(name), descriptions(std::move(descriptions)) {}

  private:
    json::json to_json() const
    {
      json::json j{{"type", "class"}, {"id", id}, {"name", name}};
      if (!descriptions.empty())
      {
        json::json j_axiom(json::json_type::array);
        for (const auto &desc : descriptions)
          j_axiom.push_back(onto::to_json(*desc));
        j["descriptions"] = std::move(j_axiom);
      }
      return j;
    }

  private:
    std::string id;
    std::string name;
    std::vector<std::unique_ptr<description>> descriptions;
  };

  class enumerated_class
  {
  public:
    enumerated_class(const std::string &id, const std::string &name, std::vector<std::string> &&values) : id(id), name(name), values(std::move(values)) {}

  private:
    json::json to_json() const
    {
      json::json j_values(json::json_type::array);
      for (const auto &value : values)
        j_values.push_back(value);
      return json::json{{"type", "enumerated"}, {"id", id}, {"name", name}, {"values", std::move(j_values)}};
    }

  private:
    std::string id;
    std::string name;
    std::vector<std::string> values;
  };

  class disjoint_classes
  {
  public:
    disjoint_classes(std::vector<std::unique_ptr<description>> &&descriptions) : descriptions(std::move(descriptions)) {}

  private:
    json::json to_json() const
    {
      json::json j_disjoint(json::json_type::array);
      for (const auto &desc : descriptions)
        j_disjoint.push_back(onto::to_json(*desc));
      return json::json{{"type", "disjoint"}, {"descriptions", std::move(j_disjoint)}};
    }

  private:
    std::vector<std::unique_ptr<description>> descriptions;
  };

  class equivalent_classes
  {
  public:
    equivalent_classes(std::vector<std::unique_ptr<description>> &&descriptions) : descriptions(std::move(descriptions)) {}

  private:
    json::json to_json() const
    {
      json::json j_equivalent(json::json_type::array);
      for (const auto &desc : descriptions)
        j_equivalent.push_back(onto::to_json(*desc));
      return json::json{{"type", "equivalent"}, {"descriptions", std::move(j_equivalent)}};
    }

  private:
    std::vector<std::unique_ptr<description>> descriptions;
  };

  class sub_class_of
  {
  public:
    sub_class_of(std::unique_ptr<description> sub, std::unique_ptr<description> super) : sub(std::move(sub)), super(std::move(super)) {}

  private:
    json::json to_json() const { return json::json{{"type", "subClassOf"}, {"sub", onto::to_json(*sub)}, {"super", onto::to_json(*super)}}; }

  private:
    std::unique_ptr<description> sub;
    std::unique_ptr<description> super;
  };
} // namespace onto