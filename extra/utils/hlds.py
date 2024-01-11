#!/usr/bin/env python3

assert "__main__" == __name__


def _private():
    import sys
    import pathlib
    import importlib

    def _auxiliary_module():
        _path = pathlib.Path(__file__).resolve(strict = True)
        assert _path.is_file()
        _path = (_path.parent / "../lib/python3/_auxiliary").resolve(strict = True)
        assert _path.is_dir()
        _base = _path.parent.as_posix()
        _path = (_path / "__init__.py").resolve(strict = True)
        assert _path.is_file()
        _path = _path.as_posix()
        assert f"{_base}/_auxiliary/__init__.py" == _path
        sys.path.append(_base)
        try: _module = importlib.import_module("_auxiliary")
        finally: sys.path.remove(_base)
        assert _module.__file__ == _path
        return _module

    _auxiliary_module = _auxiliary_module()

    return _auxiliary_module.common.property_collector.make(run = _auxiliary_module.hlds.cli.run)


try: _private().run()
finally: del _private
