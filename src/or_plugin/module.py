import openravepy
import yaml


class CommandError(Exception):
    pass


class Module(object):
    """
    Proxy class representing an OpenRAVE module implemented using `or_plugin`.
    """
    def __init__(self, env, module_name):
        self._env = env
        self._module = openravepy.RaveCreateModule(env, module_name)
        if self._module is None:
            raise ImportError(
                "Unable to load '{:s}' module. Do you have it installed in "
                "your catkin workspace? Try `openrave --listplugins`."
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
                    "`or_plugin` commands can either take ordered arguments "
                    "or keyword arguments, but not both.")
            elif args:
                input_str = yaml.dump(args)
            elif kwargs:
                input_str = yaml.dump(kwargs)
            else:
                input_str = '{}'

            output_str = self._module.SendCommand(command + " " + input_str)
            if output_str:
                return yaml.load(output_str)
            else:
                raise CommandError("`{:s}` failed to complete."
                                   .format(command))

        return command_wrapper
