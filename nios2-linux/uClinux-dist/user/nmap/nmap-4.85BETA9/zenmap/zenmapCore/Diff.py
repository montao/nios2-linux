#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2005 Insecure.Com LLC.
#
# Author: Adriano Monteiro Marques <py.adriano@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

import datetime
import os
import subprocess
import sys
import tempfile
import xml.sax

from zenmapCore.Name import APP_NAME
from zenmapCore.NmapParser import NmapParserSAX
from zenmapCore.UmitConf import PathsConfig
from zenmapCore.UmitLogging import log
import zenmapCore.Paths

# The [paths] configuration from zenmap.conf, used to get ndiff_command_path.
paths_config = PathsConfig()

class NdiffParseException(Exception):
    pass

def get_path():
    """Return a value for the PATH environment variable that is appropriate
    for the current platform. It will be the PATH from the environment plus
    possibly some platform-specific directories."""
    path_env = os.getenv("PATH")
    if path_env is None:
        search_paths = []
    else:
        search_paths = path_env.split(os.pathsep)
    for path in zenmapCore.Paths.get_extra_executable_search_paths():
        if path not in search_paths:
            search_paths.append(path)
    return os.pathsep.join(search_paths)

class NdiffCommand(subprocess.Popen):
    def __init__(self, filename_a, filename_b, temporary_filenames = []):
        self.temporary_filenames = temporary_filenames

        search_paths = get_path()
        env = dict(os.environ)
        env["PATH"] = search_paths

        command_list = [paths_config.ndiff_command_path, "--verbose", "--", filename_a, filename_b]
        self.stdout_file = tempfile.TemporaryFile(mode = "rb", prefix = APP_NAME + "-ndiff-", suffix = ".xml")

        log.debug("Running command: %s" % repr(command_list))
        # See zenmapCore.NmapCommand.py for an explanation of the shell argument.
        subprocess.Popen.__init__(self, command_list, stdout = self.stdout_file, stderr = subprocess.PIPE, env = env, shell = (sys.platform == "win32"))

    def get_scan_diff(self):
        self.wait()
        self.stdout_file.seek(0)

        return self.stdout_file.read()

    def close(self):
        """Clean up temporary files."""
        self.stdout_file.close()
        for filename in self.temporary_filenames:
            log.debug("Remove temporary diff file %s." % filename)
            os.remove(filename)
        self.temporary_filenames = []

    def kill(self):
        self.close()

def ndiff(scan_a, scan_b):
    """Run Ndiff on two scan results, which may be filenames or NmapParserSAX
    objects, and return a running NdiffCommand object."""
    temporary_filenames = []

    if isinstance(scan_a, NmapParserSAX):
        fd, filename_a = tempfile.mkstemp(prefix = APP_NAME + "-diff-", suffix = ".xml")
        temporary_filenames.append(filename_a)
        f = os.fdopen(fd, "wb")
        scan_a.write_xml(f)
        f.close()
    else:
        filename_a = scan_a

    if isinstance(scan_b, NmapParserSAX):
        fd, filename_b = tempfile.mkstemp(prefix = APP_NAME + "-diff-", suffix = ".xml")
        temporary_filenames.append(filename_b)
        f = os.fdopen(fd, "wb")
        scan_b.write_xml(f)
        f.close()
    else:
        filename_b = scan_b

    return NdiffCommand(filename_a, filename_b, temporary_filenames)
