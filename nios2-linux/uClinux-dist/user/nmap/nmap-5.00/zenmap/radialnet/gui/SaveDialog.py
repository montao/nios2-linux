# vim: set encoding=utf-8 :

# Copyright (C) 2009 Adriano Monteiro Marques
#
# Author: João Paulo de Souza Medeiros <ignotus21@gmail.com>
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
import os.path
import radialnet.gui.RadialNet as RadialNet
import zenmapGUI.FileChoosers

from zenmapGUI.higwidgets.higboxes import HIGHBox
from zenmapGUI.higwidgets.higdialogs import HIGAlertDialog


TYPES = (("By extension", None, None),
         ("PDF", RadialNet.FILE_TYPE_PDF, ".pdf"),
         ("PNG", RadialNet.FILE_TYPE_PNG, ".png"),
         ("PostScript", RadialNet.FILE_TYPE_PS, ".ps"),
         ("SVG", RadialNet.FILE_TYPE_SVG, ".svg"))
# Build a reverse index of extensions to file types, for the "By extension" file
# type.
EXTENSIONS = {}
for type in TYPES:
    if type[2] is not None:
        EXTENSIONS[type[2]] = type[1]


class SaveDialog(zenmapGUI.FileChoosers.UnicodeFileChooserDialog):
    def __init__(self):
        """
        """
        super(SaveDialog, self).__init__(title=_("Save Topology"),
                action=gtk.FILE_CHOOSER_ACTION_SAVE,
                buttons=(gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
                    gtk.STOCK_SAVE, gtk.RESPONSE_OK))

        types_store = gtk.ListStore(str, object, str)
        for type in TYPES:
            types_store.append(type)

        self.__combo = gtk.ComboBox(types_store)
        cell = gtk.CellRendererText()
        self.__combo.pack_start(cell, True)
        self.__combo.add_attribute(cell, "text", 0)

        self.__combo.connect("changed", self.__combo_changed_cb)
        self.__combo.set_active(0)

        self.connect("response", self.__response_cb)

        hbox = HIGHBox()
        label = gtk.Label(_("Select File Type:"))
        hbox.pack_end(self.__combo, False)
        hbox.pack_end(label, False)

        self.set_extra_widget(hbox)
        self.set_do_overwrite_confirmation(True)

        hbox.show_all()

    def __combo_changed_cb(self, widget):
        filename = self.get_filename() or ""
        dir, basename = os.path.split(filename)
        if dir != self.get_current_folder():
            self.set_current_folder(dir)

        # Find the recommended extension.
        new_ext = self.__combo.get_model().get_value(self.__combo.get_active_iter(), 2)
        if new_ext is not None:
            # Change the filename to use the recommended extension.
            root, ext = os.path.splitext(basename)
            if len(ext) == 0 and root.startswith("."):
                root = ""
            self.set_current_name(root + new_ext)

    def __response_cb(self, widget, response_id):
        """Intercept the "response" signal to check if someone used the "By
        extension" file type with an unknown extension."""
        if response_id == gtk.RESPONSE_OK and self.get_filetype() is None:
            ext = self.__get_extension()
            if ext == "":
                filename = self.get_filename() or ""
                dir, basename = os.path.split(filename)
                alert = HIGAlertDialog(message_format=_("No filename extension"),
                    secondary_text=_("""\
The filename "%s" does not have an extension, and no specific file type was chosen.
Enter a known extension or select the file type from the list.\
""" % basename))

            else:
                alert = HIGAlertDialog(message_format=_("Unknown filename extension"),
                    secondary_text=_("""\
There is no file type known for the filename extension "%s".
Enter a known extension or select the file type from the list.\
""") % self.__get_extension())
            alert.run()
            alert.destroy()
            # Go back to the dialog.
            self.emit_stop_by_name("response")

    def __get_extension(self):
        return os.path.splitext(self.get_filename())[1]

    def get_filetype(self):
        filetype = self.__combo.get_model().get_value(self.__combo.get_active_iter(), 1)
        if filetype is None:
            # Guess based on extension.
            return EXTENSIONS.get(self.__get_extension())
        return filetype
