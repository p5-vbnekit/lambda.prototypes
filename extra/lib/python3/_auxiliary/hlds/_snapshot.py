#!/usr/bin/env python3
# -*- coding: utf-8 -*-

assert "__main__" != __name__


def _private():
    import os
    import re
    import io
    import lzma
    import typing
    import shutil
    import pathlib
    import tarfile
    import requests
    import tempfile
    import subprocess

    from .. import common as _common_module

    _make_property_collector = _common_module.property_collector.make

    _known_metamod_plugins = {"trace", "wdmisc", "stub", "lambda"}
    _metamod_version_pattern = re.compile(r"^[\+\-\.0-\:\=\?-\[\]_a-~]+$")

    def _download_goldsrc(path: pathlib.Path, system: str):
        _url = "https://github.com/p5-vbnekit/goldsrc.archive.git"

        subprocess.check_call((
            "git", "clone", "--single-branch", "--depth=1",
            f"--branch={system}.90.steamcmd.patched.storage",
            "--", _url, path.as_posix()
        ), stdin = subprocess.DEVNULL, stdout = subprocess.DEVNULL)

        _temporary = path / ".git"
        assert _temporary.as_posix() == _temporary.resolve(strict = True).as_posix()
        assert _temporary.is_dir()
        shutil.rmtree(_temporary)

        for _temporary in os.listdir(path):
            if "contents" == _temporary:
                _temporary = path / _temporary
                assert _temporary.as_posix() == _temporary.resolve(strict = True).as_posix()
                assert _temporary.is_dir()
                _temporary = set(os.listdir(_temporary))
                _temporary.remove("valve")
                try: _temporary.remove("hlds.exe")
                except KeyError:
                    if "windows" == system: raise
                else: assert "windows" == system
                try: _temporary.remove("hlds_linux")
                except KeyError:
                    if "linux" == system: raise
                else: assert "linux" == system
                assert _temporary
                continue
            _temporary = path / _temporary
            if _temporary.is_dir(): shutil.rmtree(_temporary)
            else: _temporary.unlink()

        for _temporary in os.listdir(path / "contents"): (
            path / "contents" / _temporary
        ).replace(path / _temporary)

        (path / "contents").rmdir()

    def _download_metamod(path: pathlib.Path):
        _repo = "p5-vbnekit/metamod-p.forks"

        _response = requests.get(f"https://api.github.com/repos/{_repo}/releases/latest").json()["name"]
        assert isinstance(_response, str)
        assert _metamod_version_pattern.match(_response) is not None

        _response = requests.get(
            f"https://github.com/{_repo}/releases/download/{_response}/i686-linux-gnu.tar.xz",
            allow_redirects = True
        ).content
        assert isinstance(_response, bytes)
        assert _response

        with io.BytesIO() as _buffer:
            _buffer.write(_response)
            _response = _buffer.getvalue()

        del _buffer

        with (
            io.BytesIO(lzma.decompress(_response)) as _response,
            tarfile.TarFile(mode = "r", fileobj = _response) as _response
        ): _response.extractall(path)

    def _patch_metamod(path: pathlib.Path, system: str, plugins: typing.Iterable[str]): pass

    def _install(path: pathlib.Path, system: str, metamod):
        os.makedirs(path, exist_ok = True)
        path = path.resolve(strict = True)
        assert path.is_dir()

        _download_goldsrc(path = path, system = system)

        _collector = path / "valve"
        assert _collector.as_posix() == _collector.resolve(strict = True).as_posix()
        assert _collector.is_dir()

        if metamod is False: return

        _download_metamod(path = _collector)
        _patch_metamod(
            path = _collector, system = system,
            plugins = tuple(_known_metamod_plugins) if (metamod is True) else metamod
        )

    def _remove(path: pathlib.Path, with_root: bool):
        if with_root:
            shutil.rmtree(path)
            return

        for _path in path.iterdir():
            if _path.is_dir(): shutil.rmtree(_path)
            else: _path.unlink()

    def _validate_metamod_option(value: typing.Union[bool, typing.Iterable[str]]):
        if isinstance(value, bool): return value if (bool is type(value)) else bool(value)

        _set = _known_metamod_plugins.copy()

        def _generator():
            for _argument in value:
                assert isinstance(_argument, str)
                if str is not type(_argument): _argument = str(_argument)
                _set.remove(_argument)
                yield _argument

        return tuple(value)

    class _Class(object):
        @property
        def state(self) -> typing.Optional[bool]: return self.__state

        @property
        def path(self) -> typing.Optional[pathlib.Path]: return self.__path

        @property
        def system(self) -> str: return self.__system

        def install(self) -> pathlib.Path:
            assert self.__state is False
            self.__state = None

            _path = self.__path

            if self.__temp:
                assert _path is None
                _path = pathlib.Path(tempfile.mkdtemp(prefix = ".hlds_snapshot.", dir = "."))

            else:
                assert isinstance(_path, pathlib.Path)
                os.makedirs(_path, exist_ok = True)

            _path = _path.resolve(strict = True)
            assert _path.is_dir()
            assert not os.listdir(_path)

            try: _install(path = _path, system = self.__system, metamod = self.__metamod)
            except BaseException:
                _remove(path = _path, with_root = self.__temp)
                raise

            if self.__temp: self.__path = _path.resolve(strict = True)
            self.__state = True
            return self.__path

        def remove(self) -> None:
            assert self.__state is True
            self.__state = None

            _path = self.__path
            _temp = self.__temp

            if _temp: self.__path = None
            _remove(path = _path, with_root = _temp)

            self.__state = False

        def __init__(
            self, path: typing.Optional[pathlib.Path],
            system: typing.Literal["linux", "windows"],
            metamod: typing.Union[bool, typing.Iterable[str]]
        ):
            super().__init__()

            if (path) is not None:
                assert isinstance(path, pathlib.Path)
                if pathlib.Path is not type(path): path = pathlib.Path(path)
                assert path.as_posix()

            assert isinstance(system, str)
            if str is not type(system): system = str(system)
            assert system in {"linux", "windows"}

            metamod = _validate_metamod_option(value = metamod)

            self.__path = path
            self.__temp = path is None
            self.__state = False
            self.__system = system
            self.__metamod = metamod

    return _make_property_collector(Class = _Class)


try: Class = _private().Class
finally: del _private


def make(*args, **kwargs): return Class(*args, **kwargs)
