#!/usr/bin/env python3
# -*- coding: utf-8 -*-

assert "__main__" != __name__


def _private():
    import re
    import os
    import sys
    import git
    import lzma
    import shutil
    import tarfile
    import pathlib
    import tempfile
    import requests
    import contextlib
    import subprocess

    from . import common as _common_module

    _make_property_collector = _common_module.property_collector.make

    def _make_goldsrc():
        _url = "https://github.com/p5-vbnekit/goldsrc.archive.git"
        _path = pathlib.Path(".hlds/goldsrc.archive")

        if _path.exists():
            _path = _path.resolve(strict = True)
            assert _path.is_dir()
            _repo = git.Repo(_path)
            for _remote in _repo.remotes: _remote.fetch()

        else:
            try:
                _repo = git.Repo.clone_from(
                    _url, to_path = _path, depth = 1,
                    branch = "linux.90.steamcmd.patched.storage"
                )
                _path = _path.resolve(strict = True)

            except BaseException:
                if _path.exists(): shutil.rmtree(_path.as_posix())
                raise

        assert _path.as_posix() == _repo.working_dir

        _path = (_path / "contents").resolve(strict = True)
        assert _path.is_dir()
        return _path.as_posix()

    _git_tag_pattern = re.compile(r"^[\+\-\.0-\:\=\?-\[\]_a-~]+$")

    @contextlib.contextmanager
    def _open_metamod_archive():
        _path = ".hlds/metamod.tar.xz"
        _repo = "p5-vbnekit/metamod-p.forks"
        _response = requests.get(f"https://api.github.com/repos/{_repo}/releases/latest").json()["name"]
        assert isinstance(_response, str)
        assert _git_tag_pattern.match(_response) is not None
        _response = requests.get(
            f"https://github.com/{_repo}/releases/download/{_response}/i686-linux-gnu.tar.xz",
            allow_redirects = True
        ).content
        assert isinstance(_response, bytes)
        assert _response
        with open(_path, "wb") as _stream: _stream.write(_response)
        with (
            lzma.open(_path, "rb") as _stream,
            tarfile.TarFile(mode = "r", fileobj = _stream) as _result
        ): yield _result

    @contextlib.contextmanager
    def _open_snapshot():
        _goldsrc = _make_goldsrc()
        with (
            _open_metamod_archive() as _metamod,
            tempfile.TemporaryDirectory(prefix = "snapshot.", dir = ".hlds") as _path
        ):
            _path = pathlib.Path(_path).resolve(strict = True)
            assert _path.is_dir()
            shutil.copytree(_goldsrc, _path, symlinks = False, dirs_exist_ok = True)
            _valve_path = (_path / "valve").resolve(strict = True)
            assert _valve_path.is_dir()
            _metamod.extractall(_valve_path)
            yield _path

    def _routine():
        with _open_snapshot() as _directory:
            _executable = (_directory / "hlds_linux").resolve(strict = True)
            assert _executable.is_file()
            _executable = _executable.as_posix()
            _environment = os.environ.copy()
            try: _ld_library_path = _environment["LD_LIBRARY_PATH"].strip(":")
            except KeyError: _ld_library_path = ""
            _ld_library_path = f".:{_ld_library_path}" if _ld_library_path else "."
            _environment["LD_LIBRARY_PATH"] = _ld_library_path
            subprocess.check_call((_executable, *sys.argv[1:]), env = _environment, cwd = _directory)

    return _make_property_collector(run = _routine)


try: run = _private().run
finally: del _private
