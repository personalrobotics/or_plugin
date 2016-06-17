///
/// \file example_plugin.cpp
/// \brief Implements a very simple OpenRAVE plugin to test or_plugin.
///

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <or_plugin/yaml.h>
#include <yaml-cpp/yaml.h>

//=============================================================================
OpenRAVE::InterfaceBasePtr CreateInterfaceValidated(
    OpenRAVE::InterfaceType type, const std::string& interfacename,
    std::istream& sinput, OpenRAVE::EnvironmentBasePtr env)
{
  if (type == OpenRAVE::PT_Module && interfacename == "or_plugin_test")
  {
    return OpenRAVE::InterfaceBasePtr(new ExampleModule(env));
  }
  else
  {
    return OpenRAVE::InterfaceBasePtr();
  }
}

//=============================================================================
void GetPluginAttributesValidated(OpenRAVE::PLUGININFO& info)
{
  info.interfacenames[OpenRAVE::PT_Module].push_back("or_plugin_test");
}

//=============================================================================
class ExampleModule
: public virtual or_plugin::InterfaceMixin<OpenRAVE::ModuleBase>
{
public:
  ExampleModule(OpenRAVE::EnvironmentBasePtr env)
  : or_plugin::InterfaceMixin<ModuleBase>(env)
  , env_(env)
  {
    __description = "TF: Plugin that synchronizes TF frames with OpenRAVE bodies.";

    RegisterYAMLCommand("Succeed", boost::bind(&ExampleModule::SucceedCommand, this, _1, _2),
                        "Return success.");

    RegisterYAMLCommand("Fail", boost::bind(&ExampleModule::FailCommand, this, _1, _2),
                        "Return failure.");

    RegisterYAMLCommand("Kinbody", boost::bind(&ExampleModule::KinbodyCommand, this, _1, _2),
                        "Take a Kinbody and return it.");

    RegisterYAMLCommand("Link", boost::bind(&ExampleModule::LinkCommand, this, _1, _2),
                        "Take a Link and return it.");

    RegisterYAMLCommand("Transform", boost::bind(&ExampleModule::TransformCommand, this, _1, _2),
                        "Take a Transforms and return it.");
  }

  virtual ~ExampleModule()
  {
    // Do nothing.
  }

  /// Return success for any input.
  bool SucceedCommand(YAML::Node &output, const YAML::Node &input)
  {
    return true;
  }

  /// Return failure for any input.
  bool FailCommand(YAML::Node &output, const YAML::Node &input)
  {
    return false;
  }

  /// Serialize and deserialize the input kinbodies.
  bool KinbodyCommand(YAML::Node &output, const YAML::Node &input)
  {
    const OpenRAVE::KinBodyPtr kinbody = input[0].as<OpenRAVE::KinBody>();
    output = kinbody;
    return true;
  }

  /// Serialize and deserialize the input links.
  bool LinkCommand(YAML::Node &output, const YAML::Node &input)
  {
    const OpenRAVE::KinBody::LinkPtr link = input[0].as<OpenRAVE::KinBody::Link>();
    output = link;
    return true;
  }

  /// Serialize and deserialize the input transforms.
  bool TransformCommand(YAML::Node &output, const YAML::Node &input)
  {
    const OpenRAVE::Transform xform = input[0].as<OpenRAVE::Transform>();
    output = xform;
    return true;
  }

  /// Called when this plugin is unloaded from the environment.
  void Destroy()
  {
    // Do nothing.
  }

  /// Called on env.LoadProblem(m, 'command')
  int main(const YAML::Node &args) override
  {
    return 0;
  }

protected:
  /// Reference to OpenRAVE environment, filled in on construction
  OpenRAVE::EnvironmentBasePtr env_;
};