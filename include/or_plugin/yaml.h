///
/// \file or_plugin.h
/// \brief Helpers for converting OpenRAVE datatypes to and from YAML.
///
#ifndef OR_PLUGIN_YAML_H
#define OR_PLUGIN_YAML_H

#include <openrave/openrave.h>
#include <yaml-cpp/yaml.h>
#include <string>

namespace YAML {

///
/// YAML Converter for OpenRAVE Kinbodies
///
/// Kinbodies are stored in the format:
/// [ env_id, kinbody_name ]
/// 
template<>
struct convert<OpenRAVE::KinBodyPtr>
{
  static Node encode(const OpenRAVE::KinBodyPtr kinbody)
  {
    Node node;
    node.push_back(kinbody->GetEnvironmentId());
    node.push_back(kinbody->GetName());
    node.SetTag("!OpenRAVE::KinBody");
    return node;
  }

  static bool decode(const Node& node, OpenRAVE::KinBodyPtr &kinbody)
  {
    if(!node.IsSequence() || node.size() != 2)
    {
      RAVELOG_WARN("Kinbody must have format [env, kinbody].");
      return false;
    }

    int env_id = node[0].as<int>();
    OpenRAVE::EnvironmentBasePtr env = OpenRAVE::RaveGetEnvironment(env_id);
    if (!env)
    {
      RAVELOG_WARN("Invalid environment ID: %d.\n", env_id);
      return false;
    }

    const auto kinbody_name = node[1].as<std::string>();
    kinbody = env->GetKinBody(kinbody_name);
    if (!kinbody)
    {
      RAVELOG_WARN("No kinbody '%s' found in environment '%d'.\n",
                   kinbody_name.c_str(), env_id);
      return false;
    }

    return true;
  }
};

///
/// YAML Converter for OpenRAVE Links
///
/// Links are stored in the format:
/// [ env_id, kinbody_name, link_name ]
/// 
template<>
struct convert<OpenRAVE::KinBody::LinkPtr>
{
  static Node encode(const OpenRAVE::KinBody::LinkPtr link)
  {
    const auto kinbody = link->GetParent();

    Node node;
    node.push_back(kinbody->GetEnvironmentId());
    node.push_back(kinbody->GetName());
    node.push_back(link->GetName());
    node.SetTag("!OpenRAVE::KinBody::Link");
    return node;
  }

  static bool decode(const Node& node, OpenRAVE::KinBody::LinkPtr &link)
  {
    if(!node.IsSequence() || node.size() != 3)
    {
      RAVELOG_ERROR("Link must have format [env, kinbody, link].\n");
      return false;
    }

    int env_id = node[0].as<int>();
    OpenRAVE::EnvironmentBasePtr env = OpenRAVE::RaveGetEnvironment(env_id);
    if (!env)
    {
      RAVELOG_ERROR("Invalid environment ID: %d.\n", env_id);
      return false;
    }

    const auto kinbody_name = node[1].as<std::string>();
    auto kinbody = env->GetKinBody(kinbody_name);
    if (!kinbody)
    {
      RAVELOG_ERROR("No kinbody '%s' in environment '%d'.\n",
                   kinbody_name.c_str(), env_id);
      return false;
    }

    const auto link_name = node[2].as<std::string>();
    link = kinbody->GetLink(link_name);
    if (!link)
    {
      RAVELOG_ERROR("No link '%s' in kinbody '%s' in environment '%d'.\n",
                   link_name.c_str(), kinbody_name.c_str(), env_id);
      return false;
    }

    return true;
  }
};

///
/// YAML Converter for OpenRAVE Transforms
///
/// Transforms are stored as a list of lists in row-major form:
/// [[m00, m01, m02, m03],
///  [m10, m11, m12, m13],
///  [m20, m21, m22, m23],
///  [m30, m31, m32, m33]]
///
template<>
struct convert<OpenRAVE::Transform>
{
  static Node encode(const OpenRAVE::Transform &transform)
  {
    OpenRAVE::TransformMatrix matrix(transform);

    // Convert a 4x4 matrix into a list of lists.
    Node node;
    for (unsigned row = 0; row < 4; ++row)
    {
      for (unsigned col = 0; col < 4; ++col)
      {
        node[row][col] = matrix.m[4*row + col];
      }
    }

    node.SetTag("!OpenRAVE::Transform");
    return node;
  }

  static bool decode(const Node& node, OpenRAVE::Transform &transform)
  {
    if(!node.IsSequence() || node.size() != 4)
    {
      RAVELOG_ERROR("Transform must have 4 rows.\n");
      return false;
    }

    // Convert a list of lists into a 4x4 matrix.
    OpenRAVE::TransformMatrix matrix;
    for (unsigned row = 0; row < 4; ++row)
    {
      YAML::Node row_node = node[row];
      if(!row_node.IsSequence() || row_node.size() != 4)
      {
        RAVELOG_ERROR("Transform must have 4 columns.\n");
        return false;
      }

      for (unsigned col = 0; col < 4; ++col)
        matrix.m[4*row + col] = row_node[col].as<OpenRAVE::dReal>();
    }

    transform = matrix;
    return true;
  }
};

} // namespace YAML

#endif // OR_PLUGIN_YAML_H
