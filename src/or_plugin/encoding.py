import openravepy
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
    return dumper.represent_sequence("!numpy.ndarray", data.tolist())


def matrix_constructor(loader, node):
    matrix_node = loader.construct_sequence(node)
    return numpy.array(matrix_node)


# Note that this mapping is ASYMMETRIC w.r.t. node tags.
yaml.add_multi_representer(numpy.ndarray, matrix_representer,
                           Dumper=OpenRAVEDumper)
yaml.add_constructor("!numpy.ndarray", matrix_constructor,
                     Loader=OpenRAVELoader)
yaml.add_constructor("!OpenRAVE::Transform", matrix_constructor,
                     Loader=OpenRAVELoader)


def kinbody_representer(dumper, kinbody):
    return dumper.represent_sequence("!OpenRAVE::KinBody", [
        openravepy.RaveGetEnvironmentId(kinbody.GetEnv()),
        kinbody.GetName()
    ])


def kinbody_constructor(loader, node):
    data = loader.construct_sequence(node)
    environment = openravepy.RaveGetEnvironment(data[0])
    kinbody = environment.GetKinBody(data[1])
    return kinbody


yaml.add_multi_representer(OpenRAVE.KinBody, kinbody_representer,
                           Dumper=OpenRAVEDumper)
yaml.add_constructor("!OpenRAVE::KinBody", kinbody_constructor,
                     Loader=OpenRAVELoader)


def link_representer(dumper, link):
    return dumper.represent_sequence("!OpenRAVE::KinBody::Link", [
        openravepy.RaveGetEnvironmentId(link.GetParent().GetEnv()),
        link.GetParent().GetName(),
        link.GetName()
    ])


def link_constructor(loader, node):
    data = loader.construct_sequence(node)
    environment = openravepy.RaveGetEnvironment(data[0])
    kinbody = environment.GetKinBody(data[1])
    link = kinbody.GetLink(data[2])
    return link


yaml.add_multi_representer(OpenRAVE.KinBody.Link, link_representer,
                           Dumper=OpenRAVEDumper)
yaml.add_constructor("!OpenRAVE::KinBody::Link", link_constructor,
                     Loader=OpenRAVELoader)
