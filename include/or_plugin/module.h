///
/// \file or_plugin.h
/// \brief Helpers for constructing OpenRAVE modules within a plugin.
///
#ifndef OR_PLUGIN_MODULE_H
#define OR_PLUGIN_MODULE_H

#include <boost/function.hpp>
#include <openrave/plugin.h>
#include <yaml-cpp/yaml.h>
#include <iostream>

namespace or_plugin {

typedef boost::function<bool (YAML::Node&, const YAML::Node&)> YAMLInterfaceCommandFn;

///
/// Mixin class for OpenRAVE Module implementations that allows registration of
/// simplified interface functions.
///
template <class Base>
class InterfaceMixin : public Base
{
public:
  using Base::Base;

private:
  ///
  /// Wrapper that converts std::stream IO into YAML documents for easier parsing.
  ///
  static bool YAMLCommandWrapper(YAMLInterfaceCommandFn command,
                                 std::ostream& sout, std::istream& sin)
  {
    // Parse input as a YAML document.
    const YAML::Node input = YAML::Load(sin);

    // Send decoded YAML to wrapper command function.
    YAML::Node output;
    bool success = command(output, input);

    // Print output to stream and return status.
    if (output.IsNull())
      sout << "~" << std::endl;
    else
      sout << output << std::endl;

    return success;
  }

  ///
  /// Implementation of the string version of the main loop for this module.
  /// This command is forwarded to the YAML version of `main()`.
  ///
  virtual int main(const std::string &args) final
  {
    // Parse input as a YAML document.
    const YAML::Node input = YAML::Load(args);

    // Call the internal YAML version of this main loop.
    return main(input);
  }

protected:
  ///
  /// Helper that registers a YAML command function on an OpenRAVE plugin.
  ///
  void RegisterYAMLCommand(const std::string &cmdname,
                           YAMLInterfaceCommandFn command,
                           const std::string &strhelp)
  {
    this->RegisterCommand(
      cmdname, boost::bind(YAMLCommandWrapper, command, _1, _2), strhelp);
  }

  ///
  /// YAML main loop for this module.
  ///
  virtual int main(const YAML::Node &args)
  {
    // Do nothing here by default.
  };
};

} // namespace or_plugin

#endif // OR_PLUGIN_MODULE_H
