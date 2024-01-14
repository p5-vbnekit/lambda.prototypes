#!/usr/bin/env python3
# -*- coding: utf-8 -*-

assert "__main__" != __name__


def _private():
    import os
    import sys
    import typing
    import select
    import pathlib
    import platform
    import traceback
    import subprocess

    from . import _snapshot as _snapshot_module
    from .. import common as _common_module

    _make_snapshot = _snapshot_module.make
    _make_property_collector = _common_module.property_collector.make

    _known_executables = dict(windows = "hlds.exe", linux = "hlds_linux")

    _system = platform.system().lower()
    assert isinstance(_system, str)
    if str is not type(_system): _system = str(_system)
    assert _system in _known_executables

    _current_executable_ = _known_executables[_system]

    def _run(snapshot: pathlib.Path, arguments: typing.Iterable[str], as_child: bool):
        assert isinstance(snapshot, pathlib.Path)
        snapshot = snapshot.resolve(strict = True)
        assert snapshot.is_dir()
        _executable = (snapshot / _current_executable_).resolve(strict = True)
        assert _executable.is_file()
        snapshot = snapshot.as_posix()
        arguments = (_executable, *arguments)
        _executable = _executable.as_posix()
        _environment = os.environ.copy()
        if "linux" == _system:
            try: _ld_library_path = _environment["LD_LIBRARY_PATH"].strip(":")
            except KeyError: _ld_library_path = ""
            _ld_library_path = f".:{_ld_library_path}" if _ld_library_path else "."
            _environment["LD_LIBRARY_PATH"] = _ld_library_path
        if as_child: subprocess.check_call(arguments, env = _environment, cwd = snapshot)
        else:
            os.chdir(snapshot)
            os.execve(_executable, arguments, _environment)

    def _validate_run_arguments(value: typing.Iterable[str]):
        def _generator():
            for _argument in value:
                assert isinstance(_argument, str)
                if str is not type(_argument): _argument = str(_argument)
                yield _argument

        return tuple(value)

    class _Class(object):
        @property
        def state(self) -> bool: return self.__state

        @property
        def snapshot(self) -> typing.Optional[str]:
            _snapshot = self.__snapshot.path
            if _snapshot is None: return None
            return _snapshot.as_posix()

        def install(self) -> str:
            _snapshot = self.__snapshot
            _snapshot.install()
            self.__snapshot = _snapshot
            return _snapshot.path.as_posix()

        def remove(self) -> None:
            _snapshot = self.__snapshot
            if _snapshot.state: _snapshot.remove()

        def run(self, *arguments: str, install: bool = None, as_child: bool = None) -> None:
            arguments = _validate_run_arguments(value = arguments)

            if install is None: install = self.__install
            else:
                assert isinstance(install, bool)
                if bool is not type(install): install = bool(install)

            # if `as_child`: `popen`
            # else: `fork/exec` (`linux` required)
            # default: `fork/exec` for linux, otherwise `popen`
            if as_child is None: as_child = ("linux" != _system)
            else:
                assert isinstance(as_child, bool)
                if bool is not type(as_child): as_child = bool(as_child)
                if "linux" != _system: assert as_child

            assert not self.__state

            _snapshot = self.__snapshot
            assert _snapshot.system == _system

            _content = _snapshot.path
            if _content is not None:
                assert isinstance(_content, pathlib.Path)
                if not _content.exists(): _content = None
                else:
                    _content = _content.resolve(strict = True)
                    assert _content.is_dir()
                    _content = os.listdir(_content)
                    if not _content: _content = None
            if _content is None:
                assert install
                assert not _snapshot.state
                _snapshot.install()
            _content = _snapshot.path
            assert isinstance(_content, pathlib.Path)
            _content = _content.resolve(strict = True)
            assert _content.is_dir()
            _content = set(os.listdir(_content))
            _content.remove("valve")
            try: _content.remove("hlds.exe")
            except KeyError:
                if "windows" == _system: raise
            else: assert "windows" == _system
            try: _content.remove("hlds_linux")
            except KeyError:
                if "linux" == _system: raise
            else: assert "linux" == _system
            assert _content
            del _content

            _options = dict(snapshot = _snapshot.path, arguments = arguments, as_child = as_child)

            try:
                self.__state = True

                if as_child: return _run(**_options)

                # Linux only `fork/exec` case.
                _pid = os.fork()

                # For debug flexibility: `exec` in parent, "waiter" in child.
                # This means the child process (`python`) will be detached
                # and wait (via `getppid`,`pidfd_open`) for the parent process (`hlds`)
                if 0 == _pid:
                    # parent process pid
                    _pid = os.getppid()
                    assert isinstance(_pid, int)
                    assert 0 <= _pid
                    # detach this child
                    assert os.setsid() is None
                    # waiting for parent
                    _pid = os.pidfd_open(_pid, 0)
                    assert isinstance(_pid, int)
                    assert 0 <= _pid
                    try:
                        while True:
                            _r, _w, _x = select.select((_pid, ), [], [], None)
                            del _w, _x
                            if _pid in set(_r): break
                    finally: os.close(_pid)
                    return

                try:
                    assert isinstance(_pid, int)
                    assert 0 <= _pid
                    _run(**_options)

                except BaseException:
                    print(traceback.format_exc(), file = sys.stderr, flush = True)
                    raise

                finally: os.abort()

            finally: self.__state = False

        def __call__(self, *args, **kwargs) -> None: return self.run(*args, **kwargs)

        def __enter__(self):
            _counter = self.__counter
            assert 0 <= _counter
            self.__counter = 1 + _counter
            return self

        def __exit__(self, exception_type, exception_instance, exception_traceback):
            _counter = self.__counter
            assert 0 < _counter
            _counter = self.__counter = _counter - 1
            if 0 < _counter: return
            _snapshot = self.__snapshot
            if _snapshot.state: _snapshot.remove()

        def __init__(
            self, path: str = None,
            system: typing.Literal["linux", "windows"] = None,
            metamod: typing.Iterable[str] = True
        ):
            super().__init__()

            if path is not None:
                assert isinstance(path, str)
                if str is not type(path): path = str(path)
                assert path
                path = pathlib.Path(path)

            if system is None: system = _system

            _snapshot = _make_snapshot(path = path, system = system, metamod = metamod)

            self.__state = False
            self.__counter = 0
            self.__install = path is None
            self.__snapshot = _snapshot

    return _make_property_collector(Class = _Class)


try: Class = _private().Class
finally: del _private


def make(*args, **kwargs): return Class(*args, **kwargs)
