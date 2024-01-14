#!/usr/bin/env python3
# -*- coding: utf-8 -*-

assert "__main__" != __name__


def _private():
    import sys

    from . import _class as _instance_module
    from .. import common as _common_module

    _make_instance = _instance_module.make
    _make_property_collector = _common_module.property_collector.make

    def _routine():
        with _make_instance() as _instance: _instance(
            *sys.argv[1:]
        )

    return _make_property_collector(run = _routine)


try: run = _private().run
finally: del _private
