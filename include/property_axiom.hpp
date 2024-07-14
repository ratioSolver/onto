#pragma once

#include "description.hpp"
#include <optional>

namespace onto
{
  class property_axiom
  {
  public:
    property_axiom(const std::string &id, const std::string &name, std::optional<std::string> &&super, std::optional<std::string> &&inverse, bool symmetric, bool functional, bool inverse_functional, bool transitive, std::unique_ptr<description> &&domain, std::unique_ptr<description> &&range) : id(id), name(name), super(std::move(super)), inverse(std::move(inverse)), symmetric(symmetric), functional(functional), inverse_functional(inverse_functional), transitive(transitive), domain(std::move(domain)), range(std::move(range)) {}

  private:
    json::json to_json() const
    {
      json::json j_axiom(json::json_type::object);
      j_axiom["type"] = "property";
      j_axiom["id"] = id;
      j_axiom["name"] = name;
      if (super)
        j_axiom["super"] = *super;
      if (inverse)
        j_axiom["inverse"] = *inverse;
      j_axiom["symmetric"] = symmetric;
      j_axiom["functional"] = functional;
      j_axiom["inverseFunctional"] = inverse_functional;
      j_axiom["transitive"] = transitive;
      j_axiom["domain"] = onto::to_json(*domain);
      j_axiom["range"] = onto::to_json(*range);
      return j_axiom;
    }

  private:
    std::string id;
    std::string name;
    std::optional<std::string> super;
    std::optional<std::string> inverse;
    bool symmetric;
    bool functional;
    bool inverse_functional;
    bool transitive;
    std::unique_ptr<description> domain;
    std::unique_ptr<description> range;
  };

  class equivalent_properties
  {
  public:
    equivalent_properties(std::vector<std::string> &&properties) : properties(std::move(properties)) {}

  private:
    json::json to_json() const
    {
      json::json j_properties(json::json_type::array);
      for (const auto &property : properties)
        j_properties.push_back(property);
      return json::json{{"type", "equivalent"}, {"properties", std::move(j_properties)}};
    }

  private:
    std::vector<std::string> properties;
  };

  class subproperty_of
  {
  public:
    subproperty_of(const std::string &property, const std::string &super) : property(property), super(super) {}

  private:
    json::json to_json() const { return json::json{{"type", "subproperty"}, {"property", property}, {"super", super}}; }

  private:
    std::string property;
    std::string super;
  };
} // namespace onto