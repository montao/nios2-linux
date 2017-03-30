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

import locale
import sys

from zenmapCore.Name import APP_NAME
from zenmapCore.Paths import Path

def get_locales():
    """Get a list of locales to use based on system configuration."""
    locales = []
    loc, enc = locale.getdefaultlocale()
    if loc is not None:
        locales.append(loc)
    return locales

try:
    locale.setlocale(locale.LC_ALL, '')
except locale.Error:
    # This can happen if the LANG environment variable is set to something
    # invalid, like LANG=nothing or LANG=en_US/utf8 or LANG=us-ascii. Continue
    # without internationalization.
    pass

try:
    import gettext
except ImportError:
    # define _() so program will not fail
    import __builtin__
    __builtin__.__dict__["_"] = str
else:
    t = gettext.translation(APP_NAME, Path.locale_dir, languages = get_locales(), fallback = True)
    t.install(unicode = True)
