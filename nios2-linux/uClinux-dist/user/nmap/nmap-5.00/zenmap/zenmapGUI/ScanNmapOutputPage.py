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

import gtk
import gobject
import pango
import os

from zenmapGUI.higwidgets.higboxes import HIGHBox, HIGVBox

from zenmapGUI.NmapOutputViewer import NmapOutputViewer
from zenmapGUI.ScanRunDetailsPage import ScanRunDetailsPage
from zenmapGUI.ScansListStore import ScansListStore
from zenmapCore.Paths import Path
from zenmapCore.UmitLogging import log
import zenmapCore.I18N

def scan_entry_data_func(widget, cell_renderer, model, iter):
    """Set the properties of a cell renderer for a scan entry."""
    cell_renderer.set_property("ellipsize", pango.ELLIPSIZE_END)
    cell_renderer.set_property("style", pango.STYLE_NORMAL)
    cell_renderer.set_property("strikethrough", False)
    entry = model.get_value(iter, 0)
    if entry is None:
        return
    if entry.running:
        cell_renderer.set_property("style", pango.STYLE_ITALIC)
    elif entry.finished:
        pass
    elif entry.failed or entry.canceled:
        cell_renderer.set_property("strikethrough", True)
    cell_renderer.set_property("text", entry.get_command_string())

class Throbber(gtk.Image):
    """This is a little progress indicator that animates while a scan is
    running."""
    try:
        still = gtk.gdk.pixbuf_new_from_file(os.path.join(Path.pixmaps_dir, "throbber.png"))
        anim = gtk.gdk.PixbufAnimation(os.path.join(Path.pixmaps_dir, "throbber.gif"))
    except Exception, e:
        log.debug("Error loading throbber images: %s." % str(e))
        still = None
        anim = None

    def __init__(self):
        gtk.Image.__init__(self)
        self.set_from_pixbuf(self.still)
        self.animating = False

    def go(self):
        # Don't change anything if we're already animating.
        if not self.animating and self.anim is not None:
            self.set_from_animation(self.anim)
        self.animating = True

    def stop(self):
        if self.animating and self.still is not None:
            self.set_from_pixbuf(self.still)
        self.animating = False

class ScanNmapOutputPage(HIGVBox):
    """This is the "Nmap Output" scan results tab. It holds a text view of Nmap
    output. The constructor takes a ScansListStore, the contents of which are
    made selectable through a combo box. Details for completed scans are
    available and shown in separate windows. It emits the "changed" signal when
    the combo box selection changes."""

    __gsignals__ = {
        "changed": (gobject.SIGNAL_RUN_FIRST, gobject.TYPE_NONE, ())
    }

    def __init__(self, scans_store):
        HIGVBox.__init__(self)

        # This is a cache of details windows we have open.
        self._details_windows = {}

        self.set_spacing(0)

        hbox = HIGHBox()

        self.scans_list = gtk.ComboBox(scans_store)
        cell = gtk.CellRendererText()
        self.scans_list.pack_start(cell, True)
        self.scans_list.set_cell_data_func(cell, scan_entry_data_func)
        hbox._pack_expand_fill(self.scans_list)

        self.scans_list.connect("changed", self._selection_changed)
        scans_store.connect("row-changed", self._row_changed)
        scans_store.connect("row-deleted", self._row_deleted)

        self.throbber = Throbber()
        hbox._pack_noexpand_nofill(self.throbber)

        self.details_button = gtk.Button(_("Details"))
        self.details_button.connect("clicked", self._show_details)
        hbox._pack_noexpand_nofill(self.details_button)

        self._pack_noexpand_nofill(hbox)

        self.nmap_output = NmapOutputViewer()
        self._pack_expand_fill(self.nmap_output)

        self._update()

    def set_active_iter(self, i):
        """Set the active entry to an interator into the ScansListStore referred
        to by this object."""
        self.scans_list.set_active_iter(i)

    def get_active_entry(self):
        iter = self.scans_list.get_active_iter()
        if iter is None:
            return None
        return self.scans_list.get_model().get_value(iter, 0)

    def _selection_changed(self, widget):
        """This callback is called when a scan in the list of scans is
        selected."""
        self._update()
        self.emit("changed")

    def _row_changed(self, model, path, i):
        """This callback is called when a row in the underlying scans store is
        changed."""
        # If the currently selected entry was changed, update the interface.
        if path[0] == self.scans_list.get_active():
            self._update()

    def _row_deleted(self, model, path):
        """This callback is called when a row in the underlying scans store is
        deleted."""
        self._update()

    def _update(self):
        """Update the interface based on the currently selected entry."""
        entry = self.get_active_entry()
        if entry is None:
            self.nmap_output.show_nmap_output("")
            self.details_button.set_sensitive(False)
            self.throbber.stop()
            return

        if entry.parsed is not None:
            self.nmap_output.set_command_execution(None)
            nmap_output = entry.parsed.get_nmap_output()
            if nmap_output is not None:
                self.nmap_output.show_nmap_output(nmap_output)
            self.details_button.set_sensitive(True)
        elif entry.command is not None:
            self.nmap_output.set_command_execution(entry.command)
            self.nmap_output.refresh_output()
            self.details_button.set_sensitive(False)

        if entry.running:
            self.throbber.go()
        else:
            self.throbber.stop()

    def _show_details(self, button):
        """Show a details window for the currently selected scan, if it is
        finished."""
        entry = self.get_active_entry()
        if entry is None:
            return
        if not entry.finished:
            return
        if self._details_windows.get(entry) is None:
            window = gtk.Window()
            window.add(ScanRunDetailsPage(entry.parsed))
            def close_details(details, event, entry):
                details.destroy()
                del self._details_windows[entry]
            window.connect("delete-event", close_details, entry)
            window.show_all()
            self._details_windows[entry] = window
        self._details_windows[entry].present()
