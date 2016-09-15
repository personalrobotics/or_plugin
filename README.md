# or_plugin

`or_plugin` wraps a number of convenience methods that standardize the implementation of OpenRAVE plugins.

## Creating an OpenRAVE plugin - *C++*

One of the main features of `or_plugin` is built-in helpers for creating a new OpenRAVE plugin.  These helpers wrap the existing OpenRAVE module construction and function registration and convert much of the command system to use YAML documents for transferring data to and from modules.

[tests/example_plugin.cpp] is a simple example of an OpenRAVE module constructed using `or_plugin`.

The first major difference is that when the module is declared, it inherits from `or_plugin::InterfaceMixin<>`:

```c++
class ExampleModule : public virtual or_plugin::InterfaceMixin<OpenRAVE::ModuleBase>
{
public:
  ExampleModule(OpenRAVE::EnvironmentBasePtr env) : or_plugin::InterfaceMixin<ModuleBase>(env)
  [...]
```

This adds `or_plugin` functionality that is accessible from within the function.  The main piece of functionality that is used is `RegisterYAMLCommand()`.

```c++
RegisterYAMLCommand("Succeed", boost::bind(&ExampleModule::SucceedCommand, this, _1, _2), "Return success.");
[...]
/// Return success for any input.
bool SucceedCommand(YAML::Node &output, const YAML::Node &input)
{
  return true;
}
```

This registers an OpenRAVE command named `"Succeed"`.  Note that this function accepts two `YAML::Node` objects, one for input and one for output, instead of `std::iostream` like bare OpenRAVE command bindings.  The function must return a bool which indicates if it completed or not: failure to complete will ignore the output YAML document and throw an exception.

Because `or_plugin` is using `YAML` documents, it can automatically cast to certain OpenRAVE types.  For example, the following command simply accepts a KinBody as input, and returns the same KinBody as output:

```c++
bool KinbodyCommand(YAML::Node &output, const YAML::Node &input)
{
  const OpenRAVE::KinBodyPtr kinbody = input[0].as<OpenRAVE::KinBodyPtr>();
  output = kinbody;
  return true;
}
```

This works because `or_plugin` has defined implicit casts from `YAML::Node` to `OpenRAVE::KinBodyPtr`.  These casts are matched on the corresponding python interface.

## Creating an OpenRAVE plugin - *Python*

Once a plugin is defined in C++, it can be accessed via a helper wrapper in Python.

[test_plugin.py](tests/test_plugin.py) uses this to test the above example plugin.  The core of the example is as follows:

```python
import openravepy
import or_plugin

env = openravepy.Environment()

[...]

TestModule = or_plugin.Module('or_plugin_test')
test_module = TestModule(env)

[...]

kinbody = env.GetBodies()[0]
output = test_module.Kinbody(kinbody)
```

By creating an `or_plugin.Module()`, we are able to use `TestModule` as if it were a full python binding to the C++ implementation.  The wrapper automatically dispatched any function to the corresponding C++ call, then returns the result.  OpenRAVE native Python types can be passed and returned, as they will be automatically converted.

# Contributors

`or_plugin` was developed by the
[Personal Robotics Lab](https://personalrobotics.ri.cmu.edu) in the
[Robotics Institute](https://www.ri.cmu.edu) at
[Carnegie Mellon University](http://www.cmu.edu).
This library is developed and maintained by
[Michael Koval](https://github.com/mkoval) and
[Pras Velagapudi](https://github.com/psigen).
