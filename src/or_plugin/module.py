from . import encoding
import openravepy
import yaml


class CommandError(Exception):
    """
    Error indicating that an OpenRAVE SendCommand call returned failure.
    """
    pass


def Module(module_name):
    """
    Creates a module proxy class for the specified `or_plugin` module.

    :param module_name: the name of the OpenRAVE module to bind
    :type  module_name: str
    :returns: A class which can instantiate the module.
    :rtype:   or_plugin.module.ModuleWrapper.__class__
    """
    class ModuleWrapper(object):
        """
        Proxy representing an OpenRAVE module implemented using `or_plugin`.
        """
        def __init__(self, env):
            """
            Create and bind an OpenRAVE module of this type.

            :param env: the environment in which to create this module
            :type  env: openravepy.Environment
            """
            self._env = env
            self._module = openravepy.RaveCreateModule(env, module_name)
            if self._module is None:
                raise ImportError(
                    "Unable to load '{:s}' module. Do you have it installed "
                    "in your catkin workspace? Try `openrave --listplugins`."
                    .format(module_name))

        def __getattr__(self, command):
            # Check if the module supports this operation.
            if not self._module.SupportsCommand(command):
                raise AttributeError(
                    "Command '{:s}' is not supported by '{:s}'."
                    .format(command, self._module.GetPluginName()))

            # If the operation is supported, create a wrapper function that
            # forwards the args to the module itself.
            def command_wrapper(*args, **kwargs):
                if args and kwargs:
                    raise ValueError(
                        "`or_plugin` commands can either take ordered "
                        " or keyword arguments, but not both.")
                elif args:
                    input_str = yaml.dump(
                        args, Dumper=encoding.OpenRAVEDumper)
                elif kwargs:
                    input_str = yaml.dump(
                        kwargs, Dumper=encoding.OpenRAVEDumper)
                else:
                    input_str = '{}'

                output_str = self._module.SendCommand(
                    command + " \n" + input_str)
                if output_str:
                    return yaml.load(
                        output_str, Loader=encoding.OpenRAVELoader)
                else:
                    raise CommandError("`{:s}` failed to complete."
                                       .format(command))

            return command_wrapper

    return ModuleWrapper
