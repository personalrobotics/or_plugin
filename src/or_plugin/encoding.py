import openravepy.openravepy_int as OpenRAVE
import numpy
import yaml

# Import the native serializers if they are available.
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper


class OpenRAVEDumper(Dumper):
    """
    Custom dumper that serializes native OpenRAVE types.

    To add new types to this mapping, register them using `yaml`:
    ```
    import yaml
    yaml.add_representer(MyClass, myclass_representer, Dumper=OpenRAVEDumper)
    ```
    """
    pass


class OpenRAVELoader(Loader):
    """
    Custom loader that deserializes native OpenRAVE types.

    To add new types to this mapping, register them using `yaml`:
    ```
    import yaml
    yaml.add_constructor("!Namespace::MyClass", myclass_constructor,
                         Loader=OpenRAVELoader)
    ```
    """
    pass


def matrix_representer(dumper, data):
    pass


def matrix_constructor(loader, node):
    pass


yaml.add_representer(numpy.ndarray, matrix_representer,
                     Dumper=OpenRAVEDumper)
yaml.add_constructor("!numpy.ndarray", matrix_constructor,
                     Loader=OpenRAVELoader)


def kinbody_representer(dumper, data):
    pass


def kinbody_constructor(loader, node):
    pass


yaml.add_multi_representer(OpenRAVE.KinBody,
                           kinbody_representer)
yaml.add_constructor("!OpenRAVE::KinBody", kinbody_constructor)


def link_representer(dumper, data):
    return dumper.represent_scalar(u'!dice', u'%sd%s' % data)


def link_constructor(loader, node):
    pass

yaml.add_representer(OpenRAVE.KinBody.Link,
                     link_representer)
yaml.add_constructor("!OpenRAVE::KinBody::Link", link_representer)
