#
# ****************************************************************************
# @attention
#
# Copyright (c) 2026 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
#
# ****************************************************************************

class Singleton(type):
    """
    A metaclass that implements the Singleton design pattern.
    This metaclass ensures that only one instance of a class exists for each unique set of initialization arguments.
    If a class using this metaclass is instantiated multiple times with the same arguments, the same instance is returned.

    Use `metaclass=Singleton` in the class definition to apply this pattern.
    Example:
        ```
        class MyClass(metaclass=Singleton):
            pass

        obj1 = MyClass()
        obj2 = MyClass()
        assert obj1 is obj2  # Both references point to the same instance
        ```

    """
    _instances = {}

    def __call__(cls, *args, **kwargs):
        # Helper to make arguments hashable (convert lists to tuples recursively)
        def make_hashable(obj):
            if isinstance(obj, list):
                return tuple(make_hashable(item) for item in obj)
            elif isinstance(obj, dict):
                return tuple(sorted((k, make_hashable(v)) for k, v in obj.items()))
            else:
                return obj

        hashable_args = tuple(make_hashable(arg) for arg in args)
        hashable_kwargs = tuple(sorted((k, make_hashable(v)) for k, v in kwargs.items()))
        key = (cls, hashable_args, hashable_kwargs)

        if key not in cls._instances:
            cls._instances[key] = super(Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[key]
