///
/// \file or_plugin.h
/// \brief Helpers for constructing OpenRAVE modules within a plugin.
///
#ifndef OR_PLUGIN_MODULE_H
#define OR_PLUGIN_MODULE_H

#include <boost/function.hpp>
#include <openrave/plugin.h>
#include <yaml-cpp/yaml.h>

namespace or_plugin {

typedef boost::function<bool (YAML::Node, const YAML::Node)> YAMLInterfaceCommandFn;

///
/// Mixin class for OpenRAVE Module implementations that allows registration of
/// simplified interface functions.
///
class ModuleMixin : public virtual OpenRAVE::ModuleBase
{
  ///
  /// Helper that registers a YAML command function on an OpenRAVE plugin.
  ///
  void RegisterYAMLCommand(const std::string &cmdname, YAMLInterfaceCommandFn command,
                           const std::string &strhelp);
};

} // namespace or_plugin

#endif // OR_PLUGIN_MODULE_H
