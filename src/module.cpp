#include <boost/bind.hpp>
#include <or_plugin/module.h>
#include <or_plugin/yaml.h>

using namespace or_plugin;

//=============================================================================
///
/// Wrapper that converts std::stream IO into YAML documents for easier parsing.
///
bool YAMLCommandWrapper(YAMLInterfaceCommandFn command, std::ostream& sout, std::istream& sin)
{
  // Parse input as a YAML document.
  const YAML::Node input = YAML::Load(sin);
  if (!input)
  {
    RAVELOG_ERROR("Received malformed YAML input.\n");
    return false;
  }

  // Send decoded YAML to wrapper command function.
  YAML::Node output;
  bool success = command(output, input);

  // Print output to stream and return status.
  sout << output << std::endl;
  return success;
}

//=============================================================================
void ModuleMixin::RegisterYAMLCommand(const std::string &cmdname, YAMLInterfaceCommandFn command, const std::string &strhelp)
{
  RegisterCommand(cmdname, boost::bind(YAMLCommandWrapper, command, _1, _2), strhelp);
}
