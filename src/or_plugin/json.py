import json


class OpenRAVEEncoder(json.JSONEncoder):
    """
    Implements a JSON encoder that can handle certain OpenRAVE objects.
    """
    def default(self, obj):
        if isinstance(obj, complex):
            return [obj.real, obj.imag]
        # Let the base class default method raise the TypeError
        return json.JSONEncoder.default(self, obj)
