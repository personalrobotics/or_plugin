from .encoding import OpenRAVEEncoder
import openravepy
import json


class Module(object):
    """
    Proxy class representing an OpenRAVE module implemented using `or_plugin`.
    """
    def __init__(self, env, module_name):
        self._env = env
        self._encoder = OpenRAVEEncoder()
        self._module = openravepy.RaveCreateModule(env, module_name)
        if self._module is None:
            raise ImportError(
                "Unable to load '{:s}' module. Do you have it installed in "
                "your catkin workspace? Try `openrave --listplugins`."
                .format(module_name))

    def __getattr__(self, command):
        # Check if the module supports this operation.
        if not self._module.SupportsCommand(command):
            raise ValueError(
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
                input_str = self._encoder.encode(args)
            elif kwargs:
                input_str = self._encoder.encode(kwargs)
            else:
                input_str = '{}'

            output_str = self._module.SendCommand(command + " " + input_str)
            return json.loads(output_str)

        return command_wrapper
